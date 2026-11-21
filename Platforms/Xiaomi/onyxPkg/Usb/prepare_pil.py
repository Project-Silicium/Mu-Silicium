#!/usr/bin/env python3
"""Prepare the onyx PIL image for the ADSP startup sequence."""
import hashlib
from pathlib import Path
import struct

USB = Path(__file__).resolve().parent
ROOT = USB.parents[3]
SOURCE = ROOT / 'Binaries/onyx/QcomPkg/Drivers/PILDxe/PILDxe.efi'
EXPECTED = '2e3c8f6c8795814b1dbbbb631800ade7498c0245d1983d1686bfe8a3daead016'
# 0x1978 -> 0x19d4 skips the count/list comparison and its status-5 return.
# Context creation, metadata validation, memory setup, PAS authentication
# and all subsequent error handling remain byte-identical to the input.
PATCHES = ((0x1978, bytes.fromhex('a80000d0'), bytes.fromhex('17000014')),)

def prepared_image():
    data = bytearray(SOURCE.read_bytes())
    if hashlib.sha256(data).hexdigest() != EXPECTED:
        raise RuntimeError('Native PIL changed; re-review before patching')
    pe = struct.unpack_from('<I', data, 0x3c)[0]
    sections = struct.unpack_from('<H', data, pe + 6)[0]
    optional_size = struct.unpack_from('<H', data, pe + 20)[0]
    table = pe + 24 + optional_size
    for rva, before, after in PATCHES:
        for index in range(sections):
            _, va, size, raw = struct.unpack_from('<IIII', data, table + index * 40 + 8)
            if va <= rva and rva + len(before) <= va + size:
                offset = raw + rva - va
                if data[offset:offset + len(before)] != before:
                    raise RuntimeError(f'Unexpected instruction at RVA {rva:x}')
                data[offset:offset + len(after)] = after
                break
        else:
            raise RuntimeError(f'RVA {rva:x} outside file-backed section')
    return bytes(data)

if __name__ == '__main__':
    destination = USB / 'PilOnyx.efi'
    data = prepared_image()
    destination.write_bytes(data)
    destination.with_suffix('.depex').write_bytes(SOURCE.with_suffix('.depex').read_bytes())
    print(f'{destination}: {hashlib.sha256(data).hexdigest()}')
