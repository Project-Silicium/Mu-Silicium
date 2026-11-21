#!/usr/bin/env python3
"""Generate the onyx PIL, PMIC and USB configuration drivers from Binaries/onyx.

SPDX-License-Identifier: BSD-2-Clause-Patent
"""
import argparse
import hashlib
import os
import struct
import subprocess
from pathlib import Path
from tempfile import TemporaryDirectory

import pefile


USB = Path(__file__).resolve().parent
DRIVERS = USB.parents[3] / 'Binaries/onyx/QcomPkg/Drivers'
ROLE_RVA = 0x17000


def branch_to_role(site, offset):
    return struct.pack('<I', 0x14000000 | ((ROLE_RVA + offset - site) // 4))


# Each entry describes the reviewed input and its fixed-width replacements.
IMAGES = {
    'PilOnyx': (
        'PILDxe/PILDxe.efi',
        '2e3c8f6c8795814b1dbbbb631800ade7498c0245d1983d1686bfe8a3daead016',
        ((0x1978, bytes.fromhex('a80000d0'), bytes.fromhex('17000014')),),
    ),
    'PmicOnyx': (
        'PmicDxe/PmicDxeLa.patched.efi',
        'f75059699531c135f8e31c857339cea1fa1f5acf776389c210c1973c416396d0',
        # First of seven protected SPMI writes in EFI_PmicSchgGetChargerPortType.
        ((0x24374, bytes.fromhex('ed9aff97'), bytes.fromhex('1f2003d5')),),
    ),
    'UsbConfigOnyx': (
        'UsbConfigDxe/UsbConfigDxe.patched.efi',
        '33c788263ce67ba31e8106a9a07fa298c4c9db3e0de7883aa55ec1184fad2520',
        (
            (0xe8ba, b'\x00', b'\x01'),  # Automatic native Host start.
            (0x4990, bytes.fromhex('1f511e39'), bytes.fromhex('1f2003d5')),  # Keep dual-role mode.
            (0x782c, bytes.fromhex('881240b9'), branch_to_role(0x782c, 0)),
            (0x4e54, bytes.fromhex('880000b0'), branch_to_role(0x4e54, 0x70)),  # RetireDeviceHandle
            (0x4e9c, bytes.fromhex('9f6201f9'), branch_to_role(0x4e9c, 0x88)),  # RetireHostHandle
        ),
    ),
}


def checked_bytes(path, expected):
    data = path.read_bytes()
    if hashlib.sha256(data).hexdigest() != expected:
        raise ValueError(f'Unexpected SHA-256 for {path}')
    return data


def assemble_role():
    """Use the build's LLVM tools to assemble the position-independent hooks."""
    tools = Path(os.environ.get('CLANG_BIN', ''))
    with TemporaryDirectory() as temp:
        obj = Path(temp) / 'role.o'
        code = obj.with_suffix('.bin')
        subprocess.run([tools / 'clang', '--target=aarch64-none-elf', '-c',
                        USB / 'UsbConfigRoleEdge.S', '-o', obj], check=True)
        subprocess.run([tools / 'llvm-objcopy', '-O', 'binary', '--only-section=.text',
                        obj, code], check=True)
        return checked_bytes(code, 'b9b5bc5680077e2cc0f2a72759bd8faa4ea920644becd4650aafc61626b9715b')


def append_role_section(data, pe):
    code = assemble_role()
    optional = pe.OPTIONAL_HEADER
    size = optional.SectionAlignment
    last = pe.sections[-1]
    offset = last.get_file_offset() + last.sizeof()
    if (len(data) != ROLE_RVA or optional.SizeOfImage != ROLE_RVA or
            size != 0x1000 or optional.FileAlignment != size or len(code) > size or
            offset + last.sizeof() > optional.SizeOfHeaders or any(data[offset:offset + last.sizeof()])):
        raise ValueError('Unexpected UsbConfig section layout')

    header = pe.FILE_HEADER
    struct.pack_into('<H', data, header.get_field_absolute_offset('NumberOfSections'), header.NumberOfSections + 1)
    struct.pack_into('<I', data, optional.get_field_absolute_offset('SizeOfCode'), optional.SizeOfCode + size)
    struct.pack_into('<I', data, optional.get_field_absolute_offset('SizeOfImage'), ROLE_RVA + size)
    struct.pack_into('<8s6I2HI', data, offset, b'.role', len(code), ROLE_RVA, size, len(data),
                     0, 0, 0, 0, 0x60000020)
    data.extend(code.ljust(size, b'\0'))


def prepared_files():
    outputs = {}
    for name, (relative_path, digest, patches) in IMAGES.items():
        source = DRIVERS / relative_path
        original = checked_bytes(source, digest)
        data = bytearray(original)
        with pefile.PE(data=original, fast_load=True) as pe:
            for rva, before, after in patches:
                offset = pe.get_offset_from_rva(rva)
                if len(before) != len(after) or data[offset:offset + len(before)] != before:
                    raise ValueError(f'{source.name}: unexpected patch at RVA {rva:#x}')
                data[offset:offset + len(after)] = after
            if name == 'UsbConfigOnyx':
                append_role_section(data, pe)
        outputs[USB / f'{name}.efi'] = data
        depex = source.with_name(source.name.split('.')[0] + '.depex')
        if name == 'UsbConfigOnyx':
            outputs[USB / f'{name}.depex'] = checked_bytes(
                depex, '1f7bfcea6bbf6b396d64df59146c19d4c315845edda71ae13ff92aa2bc6e2695')
        else:
            outputs[USB / f'{name}.depex'] = depex.read_bytes()
    return outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true', help='Verify the generated files without writing them')
    args = parser.parse_args()
    for path, data in prepared_files().items():
        if path.is_file() and path.read_bytes() == data:
            continue
        if args.check:
            parser.exit(1, f'{path} is missing or stale\n')
        with TemporaryDirectory(dir=path.parent) as temp:
            staged = Path(temp) / path.name
            staged.write_bytes(data)
            staged.replace(path)


if __name__ == '__main__':
    main()
