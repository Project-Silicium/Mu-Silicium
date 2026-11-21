#!/usr/bin/env python3
"""Enable persistent native dual-role dispatch for onyx.

SPDX-License-Identifier: BSD-2-Clause-Patent
"""
import argparse
import hashlib
import struct
from pathlib import Path

# Reuse the checked PE RVA mapper used by the board's PMIC preparation.
from prepare_pmic import file_offset

USB = Path(__file__).resolve().parent
ROOT = USB.parents[3]
SOURCE = ROOT / 'Binaries/onyx/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.patched.efi'
DEPEX = SOURCE.with_name('UsbConfigDxe.depex')
EXPECTED = '33c788263ce67ba31e8106a9a07fa298c4c9db3e0de7883aa55ec1184fad2520'
EXPECTED_DEPEX = '1f7bfcea6bbf6b396d64df59146c19d4c315845edda71ae13ff92aa2bc6e2695'
ROLE_CODE_SHA256 = 'b9b5bc5680077e2cc0f2a72759bd8faa4ea920644becd4650aafc61626b9715b'
ROLE_RVA = 0x17000
PHYSICAL_DISPATCH = 0x782c


def branch_to_role_code(site, offset):
    return struct.pack('<I', 0x14000000 | ((ROLE_RVA + offset - site) // 4))

# Keep the original native role query, PHY, Start/StopController and driver
# binding. The first change permits the existing attach handler to dispatch
# its selected mode. The second keeps dual-role policy active after the first
# attach, so subsequent real DFP/UFP changes select the appropriate mode.
# No change sets a data role, a simulation flag, or a fixed Host mode.
PATCHES = (
    (0xe8ba, bytes.fromhex('00'), bytes.fromhex('01')),
    (0x4990, bytes.fromhex('1f511e39'), bytes.fromhex('1f2003d5')),
    (PHYSICAL_DISPATCH, bytes.fromhex('881240b9'), branch_to_role_code(PHYSICAL_DISPATCH, 0)),
    # StopController success paths: unpublish before forgetting the handle.
    (0x4e54, bytes.fromhex('880000b0'), branch_to_role_code(0x4e54, 0x70)),  # RetireDeviceHandle
    (0x4e9c, bytes.fromhex('9f6201f9'), branch_to_role_code(0x4e9c, 0x88)),  # RetireHostHandle
)


def append_role_section(data):
    """Map the position-independent code in UsbConfigRoleEdge.S as executable PE code."""
    code = (USB / 'UsbConfigRoleEdge.bin').read_bytes()
    if hashlib.sha256(code).hexdigest() != ROLE_CODE_SHA256:
        raise RuntimeError('Unexpected compiled USB role handler')
    pe, = struct.unpack_from('<I', data, 0x3c)
    optional = pe + 24
    count, = struct.unpack_from('<H', data, pe + 6)
    opt_size, = struct.unpack_from('<H', data, pe + 20)
    section_header = optional + opt_size + count * 40
    alignment, file_alignment = struct.unpack_from('<II', data, optional + 32)
    image_size, headers_size = struct.unpack_from('<II', data, optional + 56)
    if (len(data) != ROLE_RVA or image_size != ROLE_RVA or
            alignment != 0x1000 or file_alignment != alignment or len(code) > alignment or
            section_header + 40 > headers_size or any(data[section_header:section_header + 40])):
        raise RuntimeError('Unexpected native section layout')
    code_size, = struct.unpack_from('<I', data, optional + 4)
    struct.pack_into('<H', data, pe + 6, count + 1)
    struct.pack_into('<I', data, optional + 4, code_size + alignment)
    struct.pack_into('<I', data, optional + 56, image_size + alignment)
    struct.pack_into('<8s6I2HI', data, section_header, b'.role', len(code), ROLE_RVA,
                     alignment, len(data), 0, 0, 0, 0, 0x60000020)
    data.extend(code.ljust(alignment, b'\0'))


def prepared_image(source=None):
    original = SOURCE.read_bytes() if source is None else bytes(source)
    if hashlib.sha256(original).hexdigest() != EXPECTED:
        raise RuntimeError('Native UsbConfig changed; re-review before patching')
    data = bytearray(original)
    for rva, before, after in PATCHES:
        if len(before) != len(after):
            raise RuntimeError('UsbConfig patch must preserve image size')
        offset = file_offset(data, rva, len(before))
        if data[offset:offset + len(before)] != before:
            raise RuntimeError(f'Unexpected UsbConfig bytes at RVA {rva:x}')
        data[offset:offset + len(after)] = after
    append_role_section(data)
    return bytes(data)


def prepared_depex():
    data = DEPEX.read_bytes()
    if hashlib.sha256(data).hexdigest() != EXPECTED_DEPEX:
        raise RuntimeError('Native UsbConfig dependencies changed; re-review before packaging')
    return data


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true', help='Verify board-local outputs')
    args = parser.parse_args()
    outputs = {USB / 'UsbConfigOnyx.efi': prepared_image(),
               USB / 'UsbConfigOnyx.depex': prepared_depex()}
    for destination, data in outputs.items():
        if args.check:
            if not destination.is_file() or destination.read_bytes() != data:
                raise RuntimeError(f'{destination.name} is missing or stale; run prepare_usbconfig.py')
        else:
            destination.write_bytes(data)
        print(f'{destination}: {hashlib.sha256(data).hexdigest()}')


if __name__ == '__main__':
    main()
