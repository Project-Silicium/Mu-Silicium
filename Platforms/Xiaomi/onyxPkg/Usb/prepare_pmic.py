#!/usr/bin/env python3
"""Complete the onyx native PMIC protected-write patch for USB role changes.

SPDX-License-Identifier: BSD-2-Clause-Patent
"""
import argparse
import hashlib
from pathlib import Path
import struct

USB = Path(__file__).resolve().parent
ROOT = USB.parents[3]
SOURCE = ROOT / 'Binaries/onyx/QcomPkg/Drivers/PmicDxe/PmicDxeLa.patched.efi'
DEPEX = SOURCE.with_name('PmicDxeLa.depex')
EXPECTED = 'f75059699531c135f8e31c857339cea1fa1f5acf776389c210c1973c416396d0'

# EFI_PmicSchgGetChargerPortType (RVA 0x23e2c), type >= 7 fallback:
# pm_comm_write_byte(7, 0x2a52, 0x0a) -> SPMI WriteLong(bus=0, priority=1).
# The six following writes are already NOPs in the input. This first call
# was missed; its result is unused, and the function sets x0=0 at 0x243e8.
# Keep the native output type, error handling, and USB role logic intact.
PATCHES = ((0x24374, bytes.fromhex('ed9aff97'), bytes.fromhex('1f2003d5')),)


def file_offset(data, rva, size):
    """Translate a verified PE RVA without assuming VA == file offset."""
    pe = struct.unpack_from('<I', data, 0x3c)[0]
    if data[pe:pe + 4] != b'PE\0\0':
        raise RuntimeError('Invalid PMIC PE signature')
    machine, sections = struct.unpack_from('<HH', data, pe + 4)
    if machine != 0xaa64:
        raise RuntimeError('PMIC image is not AArch64')
    optional_size = struct.unpack_from('<H', data, pe + 20)[0]
    table = pe + 24 + optional_size
    for index in range(sections):
        _, va, raw_size, raw = struct.unpack_from('<IIII', data, table + index * 40 + 8)
        if va <= rva and rva + size <= va + raw_size:
            offset = raw + rva - va
            if offset + size <= len(data):
                return offset
    raise RuntimeError(f'RVA {rva:x} outside file-backed section')


def prepared_image(source=None):
    original = SOURCE.read_bytes() if source is None else bytes(source)
    if hashlib.sha256(original).hexdigest() != EXPECTED:
        raise RuntimeError('Native PMIC changed; re-review before patching')
    data = bytearray(original)
    for rva, before, after in PATCHES:
        if len(before) != len(after):
            raise RuntimeError('PMIC patch must preserve instruction size')
        offset = file_offset(data, rva, len(before))
        if data[offset:offset + len(before)] != before:
            raise RuntimeError(f'Unexpected instruction at RVA {rva:x}')
        data[offset:offset + len(after)] = after
    return bytes(data)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true', help='Verify existing board-local outputs')
    args = parser.parse_args()
    outputs = {USB / 'PmicOnyx.efi': prepared_image(),
               USB / 'PmicOnyx.depex': DEPEX.read_bytes()}
    for destination, data in outputs.items():
        if args.check:
            if not destination.is_file() or destination.read_bytes() != data:
                raise RuntimeError(f'{destination.name} is missing or stale; run prepare_pmic.py')
        else:
            destination.write_bytes(data)
        print(f'{destination}: {hashlib.sha256(data).hexdigest()}')


if __name__ == '__main__':
    main()
