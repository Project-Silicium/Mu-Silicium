#!/usr/bin/env python3
"""Test the packaged supply/IPCC drivers; optionally check EDK metadata.

SPDX-License-Identifier: BSD-2-Clause-Patent
"""
import argparse
import hashlib
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import uuid

USB = Path(__file__).resolve().parents[1]
PKG = USB.parent
ROOT = PKG.parents[2]
INCLUDES = [
    ROOT / "Mu_Basecore/MdePkg/Include",
    ROOT / "Mu_Basecore/MdePkg/Include/AArch64",
    ROOT / "Mu_Basecore/MdeModulePkg/Include",
    ROOT / "Mu_Basecore/EmbeddedPkg/Include",
    ROOT / "Silicon/Qualcomm/QcomPkg/Include",
]
PACKAGES = [
    "Platforms/Xiaomi", "Common/Mu", "Common/Mu_OEM_Sample",
    "Mu_Basecore", "Silicon/Qualcomm", "Silicon/Silicium",
    "Silicium-ACPI", "Silicium-ACPI/Platforms/Xiaomi",
    "Silicium-ACPI/Silicon/Qualcomm",
]


def active_source_infs():
    """Only select source modules actually present in both the DSC and FV."""
    dsc = (PKG / "onyx.dsc").read_text()
    dxe = (PKG / "Include/DXE.inc").read_text()
    result = []
    for relative in re.findall(r"^\s*INF\s+(onyxPkg/Usb/\S+\.inf)", dxe, re.M):
        inf = PKG.parent / relative
        assert relative in dsc, f"FV module is missing from DSC: {relative}"
        if re.search(r"^\[Sources\]", inf.read_text(), re.M):
            result.append(inf)
    assert len(result) == len(set(result)), "Duplicate active USB source module"
    assert {
        "OnyxIpccDxe.inf", "OnyxUsbSupplyDxe.inf", "OnyxUsbSupplyBootstrapDxe.inf"
    } <= {p.name for p in result}
    return result


def check_adsp_memory():
    source = (PKG / "Library/MemoryMapLib/MemoryMapLib.c").read_text()
    regions = [(name, int(base, 16), int(size, 16), flags) for name, base, size, flags in
               re.findall(r'\{"([^"\n]+)",\s*(0x[0-9A-Fa-f]+),\s*(0x[0-9A-Fa-f]+),([^}]+)\}', source)]
    pil = next(r for r in regions if r[0] == "PIL_Reserved")
    assert {"AddMem", "MEM_RES", "Reserv", "UNCACHED_UNBUFFERED_XN"} <= {v.strip() for v in pil[3].split(",")}
    # Independent hardware evidence: running Android reg values and XBL
    # configuration. DT node names are stale and must not supply addresses.
    for start, size in ((0x9a300000, 0x04080000),
                        (0x99d80000, 0x80000), (0x9a280000, 0x80000)):
        assert pil[1] <= start and start + size <= pil[1] + pil[2], "ADSP range outside reservation"
    ordered = sorted(regions, key=lambda r: r[1])
    for a, b in zip(ordered, ordered[1:]):
        if a[1] >= 0x80000000:
            assert a[1] + a[2] <= b[1], f"Memory overlap: {a[0]} / {b[0]}"
    print("PASS: runtime ADSP/DTB ranges reserved and board DDR regions do not overlap", flush=True)


def check_packaging():
    check_adsp_memory()
    check_pmic_usb_patch()
    check_usbconfig_patch()
    infs = active_source_infs()
    dsc = (PKG / "onyx.dsc").read_text()
    for inf in infs:
        source = re.search(r"\[Sources\]([^[]+)", inf.read_text()).group(1)
        for name in source.split():
            assert (USB / name).is_file(), f"{inf.name}: missing source {name}"
        assert f"onyxPkg/Usb/{inf.name}" in dsc
    assert "onyxPkg/Usb/PilOnyx.inf" in dsc
    assert "PcdMassStorageFile|" not in dsc
    qcom_dec = (ROOT / "Silicon/Qualcomm/QcomPkg/QcomPkg.dec").read_text()
    pcd_default = re.search(r"PcdMassStorageFile\|\{([^}]+)", qcom_dec).group(1)
    default_guid = uuid.UUID(bytes_le=bytes(int(b.strip(), 16) for b in pcd_default.split(",")))
    mass_storage_inf = ROOT / "Silicon/Qualcomm/QcomPkg/Applications/MassStorage/MassStorage.inf"
    mass_storage_guid = re.search(r"FILE_GUID\s*=\s*([\w-]+)", mass_storage_inf.read_text()).group(1)
    assert str(default_guid).lower() == mass_storage_guid.lower()
    fdf = (PKG / "onyx.fdf").read_text()
    assert "!include QcomPkg/Extra.fdf.inc" in fdf
    for name in ("APRIORI.inc", "DXE.inc"):
        contents = (PKG / "Include" / name).read_text()
        assert contents.count("INF onyxPkg/Usb/OnyxIpccDxe.inf") == 1
        assert contents.index("OnyxIpccDxe.inf") < contents.index("PmicGlinkDxe.inf")
        assert contents.count("INF onyxPkg/Usb/OnyxUsbSupplyDxe.inf") == 1
        assert "XhciPciEmulationDxe.inf" in contents
        assert contents.count("Binaries/onyx/QcomPkg/Drivers/XhciDxe/XhciDxe.inf") == 1
    apriori = (PKG / "Include/APRIORI.inc").read_text()
    startup_order = [
        "Binaries/onyx/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf",
        "onyxPkg/Usb/OnyxUsbSupplyDxe.inf",
        "Binaries/onyx/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf",
        "onyxPkg/Usb/OnyxUsbSupplyBootstrapDxe.inf",
        "Binaries/onyx/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf",
    ]
    positions = [apriori.index("INF " + name) for name in startup_order]
    assert positions == sorted(positions), "Supply adapter/bootstrap dispatch order is unsafe"
    print("PASS: resident adapter precedes PMIC and application bootstrap precedes USB power", flush=True)
    active_dxe = set(re.findall(r'^\s*INF\s+(\S+)', (PKG / 'Include/DXE.inc').read_text(), re.M))
    for module in ('GLinkDxe/GLinkDxe', 'SmemDxe/SmemDxe',
                   'PmicGlinkDxe/PmicGlinkDxe', 'UsbPwrCtrlDxe/UsbPwrCtrlDxe',
                   'PILProxyDxe/PILProxyDxe'):
        assert f'Binaries/onyx/QcomPkg/Drivers/{module}.inf' in active_dxe, (
            f'{module}: APRIORI ordering alone does not include a firmware file')
    assert 'onyxPkg/Usb/OnyxIpccDxe.inf' in active_dxe
    assert 'Binaries/onyx/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf' not in active_dxe
    print('PASS: GLINK/ADSP dependencies are active FV entries, not just APRIORI GUIDs', flush=True)
    print("PASS: onyx supply policy retains the native USB configuration and xHCI drivers", flush=True)
    binaries = {
        "PILDxe/PILDxe.efi": "2e3c8f6c8795814b1dbbbb631800ade7498c0245d1983d1686bfe8a3daead016",
        "QcomChargerDxe/QcomChargerDxeLA.efi": "7d4a2e921ed44bfbc69abad93c617912634905ca584b1516633e53eeea6d9db5",
        "IPCCDxe/IPCCDxe.efi": "79eb532d677434412b8bcc72432930eb2b76f7f6a69df4168c640e0495b3a3d9",
        "I2CDxe/I2CDxe.efi": "8aeff0f8bbab04d91bc3584b6f49c837bd305b06403e45934cc64526495c7dc5",
        "TLMMDxe/TLMMDxe.efi": "07014ab47a95b57662a2f8c946715ea0bbe2ef580b472210647eccf2c69ab6b5",
        "GLinkDxe/GLinkDxe.efi": "5d574d04ffd0374672fd7c6d9244426293ab6ccf6ca12d905818aaa62b0abdc0",
        "PmicGlinkDxe/PmicGlinkDxe.efi": "cf4aa890f69ba6a993afe0958900256d367ff0dce1dca139a05562ff841fb666",
        "UsbConfigDxe/UsbConfigDxe.patched.efi": "33c788263ce67ba31e8106a9a07fa298c4c9db3e0de7883aa55ec1184fad2520",
        "SPMIDxe/SPMIDxe.patched.efi": "c6008d56d4554e92170e5709e74f653d77bbd972e9415d5e0a7c940bfe6f27cb",
        "UsbPwrCtrlDxe/UsbPwrCtrlDxe.efi": "a0d7b03112fdc135ae66b3fb0ed712597645e265082826bca6b511fc07b7b3ef",
        "NpaDxe/NpaDxe.efi": "10bed3fcf08c66475279412fe7e566d398aff20a579da2aa38d27178fa4de133",
        "ClockDxe/ClockDxe.patched.efi": "342791d7176d5a538b9ee733bc42e90a7d9ac8f183c5b5b94c3c3be1a08e1356",
        "PmicDxe/PmicDxeLa.patched.efi": "f75059699531c135f8e31c857339cea1fa1f5acf776389c210c1973c416396d0",
        "XhciPciEmulationDxe/XhciPciEmulationDxe.efi": "b86509f52a54436dadfe42b4086a0d6cd45e3ff4ead03bae785ed2a0fff5afd7",
        "XhciDxe/XhciDxe.efi": "17c307fad7c1ab62a94035551793e64f4923d9c3312c3f38df122b62223f97fc",
    }
    for name, digest in binaries.items():
        binary = ROOT / "Binaries/onyx/QcomPkg/Drivers" / name
        assert hashlib.sha256(binary.read_bytes()).hexdigest() == digest, (
            f"{name} changed; revalidate the private USB/SPMI ABI before updating this hash")
    print("PASS: firmware binaries match the reviewed ABI", flush=True)
    import importlib.util
    spec_pil = importlib.util.spec_from_file_location('prepare_pil', USB / 'prepare_pil.py')
    pil = importlib.util.module_from_spec(spec_pil); spec_pil.loader.exec_module(pil)
    original = pil.SOURCE.read_bytes(); patched = pil.prepared_image()
    assert (USB / 'PilOnyx.efi').read_bytes() == patched
    assert (USB / 'PilOnyx.depex').read_bytes() == pil.SOURCE.with_suffix('.depex').read_bytes()
    assert len(original) == len(patched)
    assert sum(a != b for a, b in zip(original, patched)) == 2
    for name in ('APRIORI.inc', 'DXE.inc'):
        assert 'INF onyxPkg/Usb/PilOnyx.inf' in (PKG / 'Include' / name).read_text()
    print('PASS: hash-pinned PIL single-branch retail bypass', flush=True)


def check_pmic_usb_patch():
    """Keep the protected-write fix reproducible and restricted to onyx."""
    import importlib.util
    spec = importlib.util.spec_from_file_location('prepare_pmic', USB / 'prepare_pmic.py')
    pmic = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(pmic)
    original = pmic.SOURCE.read_bytes()
    patched = pmic.prepared_image()
    assert len(original) == len(patched) == 270336
    assert hashlib.sha256(original).hexdigest() == pmic.EXPECTED
    offset = pmic.file_offset(original, 0x24374, 4)
    assert original[offset:offset + 4] == bytes.fromhex('ed9aff97')
    assert patched[offset:offset + 4] == bytes.fromhex('1f2003d5')
    assert [i for i, (a, b) in enumerate(zip(original, patched)) if a != b] == list(range(offset, offset + 4))
    # Neighbouring calls were already disabled upstream; preserve their patch.
    for rva in (0x24388, 0x24398, 0x243a8, 0x243bc, 0x243d0, 0x243e4):
        at = pmic.file_offset(original, rva, 4)
        assert original[at:at + 4] == patched[at:at + 4] == bytes.fromhex('1f2003d5')
    assert (USB / 'PmicOnyx.efi').read_bytes() == patched
    assert (USB / 'PmicOnyx.depex').read_bytes() == pmic.DEPEX.read_bytes()
    changed_input = bytearray(original)
    changed_input[offset] ^= 1
    try:
        pmic.prepared_image(changed_input)
    except RuntimeError:
        pass
    else:
        raise AssertionError('An unreviewed firmware version was accepted')
    for name in ('APRIORI.inc', 'DXE.inc'):
        contents = (PKG / 'Include' / name).read_text()
        assert contents.count('INF onyxPkg/Usb/PmicOnyx.inf') == 1
        assert 'INF Binaries/onyx/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf' not in contents
    inf = (USB / 'PmicOnyx.inf').read_text()
    original_inf = pmic.SOURCE.with_name('PmicDxeLa.inf').read_text()
    pattern = r'FILE_GUID\s*=\s*([\w-]+)'
    assert re.search(pattern, inf).group(1) == re.search(pattern, original_inf).group(1)
    assert 'onyxPkg/Usb/PmicOnyx.inf' in (PKG / 'onyx.dsc').read_text()
    print('PASS: onyx PMIC patch changes only the missed protected write; source and DEPEX preserved',
          flush=True)


def check_usbconfig_patch():
    """Validate the native role patches and their board-only packaging."""
    import importlib.util
    spec = importlib.util.spec_from_file_location('prepare_usbconfig', USB / 'prepare_usbconfig.py')
    usbconfig = importlib.util.module_from_spec(spec)
    sys.path.insert(0, str(USB))
    try:
        spec.loader.exec_module(usbconfig)
    finally:
        sys.path.pop(0)
    original = usbconfig.SOURCE.read_bytes()
    patched = usbconfig.prepared_image()
    assert hashlib.sha256(original).hexdigest() == usbconfig.EXPECTED
    # Reconstruct the permitted PE header edits independently of the preparer.
    # The new read/execute section must follow .reloc inside the loaded image.
    pe, = struct.unpack_from('<I', original, 0x3c)
    optional = pe + 24
    section_count, = struct.unpack_from('<H', original, pe + 6)
    optional_size, = struct.unpack_from('<H', original, pe + 20)
    new_header = optional + optional_size + section_count * 40
    expected = bytearray(original)
    struct.pack_into('<H', expected, pe + 6, section_count + 1)
    for field in (optional + 4, optional + 56):  # SizeOfCode, SizeOfImage
        old, = struct.unpack_from('<I', original, field)
        struct.pack_into('<I', expected, field, old + 0x1000)
    name, size, rva, raw_size, raw_off, reloc, line, n_reloc, n_line, flags = struct.unpack_from(
        '<8s6I2HI', patched, new_header)
    assert name.rstrip(b'\0') == b'.role' and flags == 0x60000020
    assert 0 < size <= raw_size and raw_size == 0x1000
    assert rva == raw_off == len(original) == 0x17000
    assert (reloc, line, n_reloc, n_line) == (0, 0, 0, 0)
    expected[new_header:new_header + 40] = patched[new_header:new_header + 40]
    assert len(patched) == raw_off + raw_size
    assert patched[raw_off:raw_off + size] == (USB / 'UsbConfigRoleEdge.bin').read_bytes()
    assert not any(patched[raw_off + size:])
    for rva, before, after in ((0xe8ba, bytes.fromhex('00'), bytes.fromhex('01')),
                               (0x4990, bytes.fromhex('1f511e39'), bytes.fromhex('1f2003d5')),
                               (0x4e54, bytes.fromhex('880000b0'),
                                struct.pack('<I', 0x14000000 | ((0x17070 - 0x4e54) // 4))),
                               (0x4e9c, bytes.fromhex('9f6201f9'),
                                struct.pack('<I', 0x14000000 | ((0x17088 - 0x4e9c) // 4))),
                               (0x782c, bytes.fromhex('881240b9'),
                                struct.pack('<I', 0x14000000 | ((0x17000 - 0x782c) // 4)))):
        offset = usbconfig.file_offset(original, rva, len(before))
        assert original[offset:offset + len(before)] == before
        assert patched[offset:offset + len(after)] == after
        expected[offset:offset + len(after)] = after
    assert patched[:len(original)] == expected
    for offset in (0, usbconfig.file_offset(original, 0x782c, 4)):
        changed = bytearray(original)
        changed[offset] ^= 1
        try:
            usbconfig.prepared_image(changed)
        except RuntimeError:
            pass
        else:
            raise AssertionError('UsbConfig accepted unreviewed input bytes')
    assert usbconfig.prepared_depex() == usbconfig.DEPEX.read_bytes()
    pattern = r'FILE_GUID\s*=\s*([\w-]+)'
    assert re.search(pattern, (USB / 'UsbConfigOnyx.inf').read_text()).group(1) == re.search(
        pattern, usbconfig.SOURCE.with_name('UsbConfigDxe.inf').read_text()).group(1)
    local = 'onyxPkg/Usb/UsbConfigOnyx.inf'
    upstream = 'Binaries/onyx/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf'
    surfaces = [(PKG / 'onyx.dsc').read_text(),
                (PKG / 'Include/APRIORI.inc').read_text(), (PKG / 'Include/DXE.inc').read_text()]
    for surface in surfaces:
        assert surface.count(local) == 1 and upstream not in surface
    assert (USB / 'UsbConfigOnyx.efi').read_bytes() == patched
    assert (USB / 'UsbConfigOnyx.depex').read_bytes() == usbconfig.prepared_depex()
    print('PASS: packaged UsbConfig role dispatch, PE section bounds, hash rejection and GUID/DEPEX', flush=True)


def run(command, **kwargs):
    subprocess.run([str(x) for x in command], cwd=ROOT, check=True, **kwargs)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cc", default=os.environ.get("CC", shutil.which("gcc")),
                        help="Native GCC executable (required)")
    parser.add_argument("--zig", help="Optional Zig executable for AArch64 compilation")
    parser.add_argument("--metadata", action="store_true",
                        help="Run EDK II genc for all active USB source INFs using existing Conf setup")
    args = parser.parse_args()
    if not args.cc:
        parser.error("GCC was not found; supply --cc /path/to/gcc")
    check_packaging()
    includes = [arg for path in INCLUDES for arg in ("-I", str(path))]
    warnings = ["-Wall", "-Wextra", "-Werror", "-Wno-unused-parameter"]
    with tempfile.TemporaryDirectory(prefix="onyx-usb-tests-") as directory:
        for test in ("OnyxUsbSupplyTest", "OnyxIpccTest"):
            output = Path(directory) / (test + (".exe" if os.name == "nt" else ""))
            run([args.cc, "-std=c11", "-DMDEPKG_NDEBUG", "-fshort-wchar",
                 "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections", "-O2",
                 *warnings, *includes, USB / "Tests" / (test + ".c"), "-o", output])
            run([output])
        if args.zig:
            for inf in active_source_infs():
                metadata = inf.read_text()
                source = re.search(r"\[Sources\]([^[]+)", metadata).group(1)
                defines = sorted(set(re.findall(r"[-/]D\s*([A-Z_]\w*(?:=\w+)?)", metadata)))
                for name in source.split():
                    if name.endswith(".c"):
                        run([args.zig, "cc", "-target", "aarch64-freestanding", "-ffreestanding",
                             "-fshort-wchar", "-O2", "-c", *warnings, *includes,
                             *("-D" + define for define in defines), USB / name,
                             "-o", Path(directory) / (inf.stem + ".o")])
            print("PASS: active drivers compile as strict AArch64 objects (not linked UEFI images)",
                  flush=True)
    if args.metadata:
        env = os.environ.copy()
        tools = ROOT / "Mu_Basecore/BaseTools"
        env.update(WORKSPACE=str(ROOT), PACKAGES_PATH=os.pathsep.join(str(ROOT / p) for p in PACKAGES),
                   EDK_TOOLS_PATH=str(tools), BASE_TOOLS_PATH=str(tools),
                   PYTHONPATH=str(tools / "Source/Python"))
        for inf in active_source_infs():
            run([sys.executable, tools / "Source/Python/build/build.py", "genc",
                 "-p", PKG / "onyx.dsc", "-m", inf, "-a", "AARCH64",
                 "-t", "CLANGPDB", "-b", "DEBUG", "-D", "ENABLE_SECUREBOOT=0",
                 "-D", "FD_BASE=0xa7100000", "-D", "FD_SIZE=0x300000",
                 "-D", "FD_BLOCKS=0x300", "-D", "DEVICE_MODEL=0"], env=env)
        print("PASS: EDK II metadata and AutoGen for all active USB source drivers (no link)")


if __name__ == "__main__":
    main()
