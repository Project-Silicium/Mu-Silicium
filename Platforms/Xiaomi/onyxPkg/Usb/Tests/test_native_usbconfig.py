#!/usr/bin/env python3
"""Execute the hash-pinned native UsbConfig AArch64 role policy in Unicorn.

This is a binary state-machine regression test, not hardware validation. The
real QueryConnectionChange, HandlePortPartnerXtach, StartController and
StopController instructions run. Boot Services, Runtime Services, USB power
status, charger detection and hardware core operations are explicit stubs.

Install unicorn and pefile in a task-local directory and add that directory
to PYTHONPATH. This test never installs packages or writes firmware images.

SPDX-License-Identifier: BSD-2-Clause-Patent
"""
import hashlib
from pathlib import Path
import struct
import unittest

import pefile
from unicorn import Uc, UcError, UC_ARCH_ARM64, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm64_const import (
    UC_ARM64_REG_PC, UC_ARM64_REG_SP, UC_ARM64_REG_LR,
    UC_ARM64_REG_X0, UC_ARM64_REG_X1, UC_ARM64_REG_X2,
    UC_ARM64_REG_X3, UC_ARM64_REG_X4,
)

ROOT = Path(__file__).resolve().parents[5]
BINARY = ROOT / 'Binaries/onyx/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.patched.efi'
BOARD_BINARY = Path(__file__).resolve().parents[1] / 'UsbConfigOnyx.efi'
EXPECTED_SHA256 = '33c788263ce67ba31e8106a9a07fa298c4c9db3e0de7883aa55ec1184fad2520'
PREVIOUS_BOARD_SHA256 = 'f38dbc099af26909800a7daac13a030abcfe281e15d13ec09ea399d1eb3f5376'
BASE = 0x1000000
POOL = 0x2000000
STUB = 0x3000000
STACK = 0x4000000
RETURN = STUB + 0xFF0
EFI_ERROR = 1 << 63
EFI_DEVICE_ERROR = EFI_ERROR | 7
EFI_NOT_FOUND = EFI_ERROR | 14
INVALID = 0x10000
HOST = 1
DEVICE = 4


class NativeUsbConfig:
    """A single native image with a deterministic external firmware boundary."""

    # Restrict execution to reviewed policy functions. An unexpected native
    # call fails rather than silently returning success or touching MMIO.
    NATIVE_RANGES = (
        (0x3D14, 0x3DAC),       # UsbConfigGetSimulatePortType (real helper)
        (0x4290, 0x42B0),       # UsbConfigInit DualRole branch
        (0x44B8, 0x4B50),       # StartController / HandlePortPartnerXtach
        (0x4D34, 0x4F78),       # StopController
        (0x50DC, 0x51F0),       # Native CC orientation selection
        (0x7484, 0x7544),       # SetSimulate, guarded off by physical platform
        (0x75F4, 0x7930),       # QueryConnectionChange
        (0x79A4, 0x7A9C),       # GetCC / GetUsbPortType
    )

    def __init__(self, variant='board'):
        if variant in ('board', 'edge_without_retire'):
            # Run the exact board-local image selected by UsbConfigOnyx.inf.
            pe = pefile.PE(data=BOARD_BINARY.read_bytes())
            if variant == 'edge_without_retire':
                # Keep real role-edge dispatch; restore only the two original
                # StopController instructions to isolate the handle leak.
                original = pefile.PE(data=BINARY.read_bytes())
                for site in (0x4E54, 0x4E9C):
                    pe.set_bytes_at_rva(site, original.get_data(site, 4))
        else:
            if variant not in ('original', 'gate_only', 'previous_board'):
                raise ValueError(variant)
            raw = BINARY.read_bytes()
            if hashlib.sha256(raw).hexdigest() != EXPECTED_SHA256:
                raise AssertionError('Native binary changed: review instruction RVAs before testing')
            pe = pefile.PE(data=raw)
            if variant in ('gate_only', 'previous_board'):
                pe.set_bytes_at_rva(0xE8BA, bytes([1]))
            if variant == 'previous_board':
                pe.set_bytes_at_rva(0x4990, bytes.fromhex('1f 20 03 d5'))
                if hashlib.sha256(pe.__data__).hexdigest() != PREVIOUS_BOARD_SHA256:
                    raise AssertionError('Negative control must match the previously tested board image')
        if pe.FILE_HEADER.Machine != 0xAA64:
            raise AssertionError('Expected AArch64 PE image')
        self.native_ranges = self.NATIVE_RANGES
        if variant in ('board', 'edge_without_retire'):
            for section in pe.sections:
                if section.Name.rstrip(b'\0') == b'.role':
                    if section.VirtualAddress != 0x17000 or section.Characteristics != 0x60000020:
                        raise AssertionError('Unexpected role-dispatch section mapping or permissions')
                    self.native_ranges += ((section.VirtualAddress,
                                           section.VirtualAddress + section.Misc_VirtualSize),)
        pe.relocate_image(BASE)
        mapped = pe.get_memory_mapped_image()
        self.uc = Uc(UC_ARCH_ARM64, UC_MODE_ARM)
        self.uc.mem_map(BASE, (max(pe.OPTIONAL_HEADER.SizeOfImage, len(mapped)) + 0xFFF) & ~0xFFF)
        self.uc.mem_write(BASE, mapped)
        for address, size in ((POOL, 0x10000), (STUB, 0x2000), (STACK, 0x20000)):
            self.uc.mem_map(address, size)
        self.role = 0
        self.power_role = None
        self.getter_status = 0
        self.getter_calls = 0
        self.charger_status = 0
        self.connect_status = 0
        self.disconnect_status = 0
        self.install_status = 0
        self.uninstall_status = 0
        self.events = []
        self.start_calls = []
        self.stop_calls = []
        self.connected = {}
        self.protocols = {}  # Producers publish a GUID and live interface per handle.
        self.next_handle = POOL + 0x6000
        self.core = POOL
        self.bs = POOL + 0x1000
        self.rs = POOL + 0x2000
        self.pwr = POOL + 0x3000
        self.ctx = POOL + 0x4000
        self.stop_at = None
        self.stub_names = {}
        self.debug_formats = []
        self.last_pc = None
        self.steps = 0
        for offset, name in ((8, 'core_device_init'), (0x10, 'core_device_start'),
                             (0x48, 'core_orientation'), (0x58, 'core_host_reset'),
                             (0x60, 'core_device_detach')):
            self.w64(self.core + offset, self.stub(name))
        for offset, name in ((0x90, 'bs_uninstall'), (0x108, 'bs_connect'), (0x110, 'bs_disconnect'),
                             (0x140, 'bs_locate'), (0x148, 'bs_install')):
            self.w64(self.bs + offset, self.stub(name))
        self.w64(self.rs + 0x48, self.stub('rs_get_variable'))
        self.w64(self.pwr + 0x30, self.stub('pwr_get_typec'))
        self.w64(self.pwr + 0x50, self.stub('pwr_vbus_source'))
        for rva, value in ((0x152D0, self.core), (0x152D8, self.ctx),
                           (0x156F0, self.bs), (0x15750, self.rs), (0x157F0, self.pwr)):
            self.w64(BASE + rva, value)
        self.w32(self.ctx + 4, 1)  # Native constructor's detached state.
        self.w32(self.ctx + 0x10, 0)
        self.w32(BASE + 0x156A0, 99)  # Physical platform, outside emulation list.
        self.w32(BASE + 0x1536C, INVALID)
        for rva in (0x15370, 0x15790, 0x157A8, 0x157AC, 0x15820, 0x15824, 0x15828):
            self.uc.mem_write(BASE + rva, bytes([0]))
        self.uc.hook_add(UC_HOOK_CODE, self.on_code)
        # Run the actual DualRole initialization block: sets mode request 32
        # and the native role-pending flag. Do not emulate the driver entry's
        # clocks, GPIOs, protocols or NVRAM policy choice.
        self.invoke(0x4290, stop_at=0x42D4)

    def w32(self, address, value):
        self.uc.mem_write(address, struct.pack('<I', value & 0xFFFFFFFF))

    def w64(self, address, value):
        self.uc.mem_write(address, struct.pack('<Q', value & 0xFFFFFFFFFFFFFFFF))

    def r32(self, address):
        return struct.unpack('<I', self.uc.mem_read(address, 4))[0]

    def r64(self, address):
        return struct.unpack('<Q', self.uc.mem_read(address, 8))[0]

    def reg(self, n):
        return self.uc.reg_read((UC_ARM64_REG_X0, UC_ARM64_REG_X1,
                                 UC_ARM64_REG_X2, UC_ARM64_REG_X3, UC_ARM64_REG_X4)[n])

    def stub(self, name):
        address = STUB + len(self.stub_names) * 16
        self.stub_names[address] = name
        self.uc.mem_write(address, bytes.fromhex('c0 03 5f d6'))  # RET
        return address

    def return_from_stub(self, result=0):
        self.uc.reg_write(UC_ARM64_REG_X0, result)
        self.uc.reg_write(UC_ARM64_REG_PC, self.uc.reg_read(UC_ARM64_REG_LR))

    def on_code(self, uc, address, size, _):
        self.last_pc = address
        self.steps += 1
        if address == RETURN or address == self.stop_at:
            uc.emu_stop()
            return
        rva = address - BASE
        if rva == 0x44B8:
            self.start_calls.append((self.reg(0), self.reg(1)))
        elif rva == 0x4D34:
            self.stop_calls.append((self.reg(0), self.reg(1)))
        if rva == 0x22A8:  # DebugPrint is an external support library.
            self.return_from_stub()
            return
        if rva == 0xA1D4:  # Config lookup absent: native code uses NVRAM fallback.
            self.return_from_stub(0)
            return
        if rva == 0x35E0:  # Charger detection input; no PMIC transactions.
            if self.charger_status == 0:
                self.w32(self.reg(1), 1)  # SDP, relevant only for native gadget path.
            self.return_from_stub(self.charger_status)
            return
        if address in self.stub_names:
            name = self.stub_names[address]
            self.dispatch_stub(name)
            return
        if not any(lo <= rva < hi for lo, hi in self.native_ranges):
            raise AssertionError(f'Unreviewed native execution RVA 0x{rva:X}')

    def dispatch_stub(self, name):
        result = 0
        if name == 'pwr_get_typec':
            self.getter_calls += 1
            if self.reg(0) != 0:
                raise AssertionError('Native policy unexpectedly queried another port')
            result = self.getter_status
            if result == 0:
                # Six native 32-bit slots. Connect is BOOLEAN + zero padding.
                # These are external test inputs, not synthetic role changes
                # in the policy under test.
                connected = self.role in (1, 2)
                power = self.power_role if self.power_role is not None else self.role
                self.uc.mem_write(self.reg(1), struct.pack('<6I',
                    int(connected), power if connected else 0,
                    self.role, 0, 0, 1))
        elif name == 'rs_get_variable':
            result = EFI_NOT_FOUND
        elif name == 'bs_locate':
            self.w64(self.reg(2), self.pwr)
        elif name == 'bs_install':
            result = self.install_status
            if result == 0:
                output = self.reg(0)
                if self.r64(output) == 0:
                    self.w64(output, self.next_handle)
                    self.next_handle += 0x20
                handle = self.r64(output)
                guid, interface = self.reg(1), self.reg(2)
                if handle in self.protocols:
                    raise AssertionError('Duplicate UsbConfig protocol on the same handle')
                self.protocols[handle] = (guid, interface)
                mode = self.r32(interface + 0x8C)
                self.events.append(('install', mode))
        elif name == 'bs_uninstall':
            handle = self.reg(0)
            if self.protocols.get(handle) != (self.reg(1), self.reg(2)):
                raise AssertionError('Uninstall must use the exact published protocol')
            if handle in self.connected:
                raise AssertionError('Uninstall called before successful disconnect')
            result = self.uninstall_status
            if result == 0:
                del self.protocols[handle]
        elif name == 'bs_connect':
            handle = self.reg(0)
            mode = self.r32(self.protocols[handle][1] + 0x8C)
            result = self.connect_status
            self.events.append(('connect', mode, result))
            if result == 0:
                if handle in self.connected:
                    raise AssertionError('Duplicate ConnectController without disconnect')
                self.connected[handle] = mode
        elif name == 'bs_disconnect':
            handle = self.reg(0)
            mode = self.r32(self.protocols[handle][1] + 0x8C)
            result = self.disconnect_status
            self.events.append(('disconnect', mode, result))
            if result == 0:
                self.connected.pop(handle, None)
        else:
            self.events.append((name, self.reg(1)))
        self.return_from_stub(result)

    def invoke(self, rva, *args, stop_at=None):
        self.uc.reg_write(UC_ARM64_REG_SP, STACK + 0x1F000)
        self.uc.reg_write(UC_ARM64_REG_LR, RETURN)
        for i, value in enumerate(args):
            self.uc.reg_write((UC_ARM64_REG_X0, UC_ARM64_REG_X1, UC_ARM64_REG_X2)[i], value)
        self.stop_at = BASE + stop_at if stop_at is not None else None
        try:
            self.uc.emu_start(BASE + rva, RETURN, count=100000)
        except UcError as error:
            raise AssertionError(f'Unicorn stopped at RVA 0x{self.last_pc - BASE:X}: {error}') from error
        finally:
            self.stop_at = None
        if self.uc.reg_read(UC_ARM64_REG_PC) not in (RETURN, BASE + stop_at if stop_at else RETURN):
            raise AssertionError('Native call did not return within instruction limit')
        return self.reg(0)

    def poll(self, role):
        self.role = role
        self.invoke(0x4788)
        return self.snapshot()

    def start_device(self):
        # Explicit native caller, as UsbInit/MassStorage owns gadget startup.
        return self.invoke(0x44B8, 0, DEVICE)

    def snapshot(self):
        return {
            'mode': self.r32(BASE + 0x1536C),
            'dual': self.uc.mem_read(BASE + 0x15794, 1)[0],
            'requested': self.r32(self.core),
            'role': self.r32(self.ctx + 0x10),
            'connection': self.r32(self.ctx + 4),
            'platform': self.r32(BASE + 0x156A0),
            'connected': sorted(self.connected.values()),
            'getter_calls': self.getter_calls,
            'start_calls': len(self.start_calls),
        }


class NativeRolePolicyTests(unittest.TestCase):
    def test_original_does_not_auto_start_host(self):
        native = NativeUsbConfig('original')
        result = native.poll(2)
        self.assertEqual(result['role'], 2)
        self.assertEqual(result['mode'], INVALID)
        self.assertEqual(result['connected'], [])
        self.assertEqual(result['start_calls'], 0)
        self.assertEqual(result['dual'], 0)

    def test_gate_only_loses_host_after_device_first_attach(self):
        native = NativeUsbConfig('gate_only')
        self.assertEqual(native.start_device(), 0)
        native.poll(1)
        native.poll(0)
        result = native.poll(2)
        self.assertEqual(result['role'], 2)
        self.assertEqual(result['mode'], DEVICE)
        self.assertNotIn(HOST, result['connected'])

    def test_sampled_detach_preserves_native_device_ownership(self):
        native = NativeUsbConfig()
        self.assertEqual(native.start_device(), 0)
        self.assertEqual(native.poll(1)['mode'], DEVICE)
        native.poll(0)
        host = native.poll(2)
        self.assertEqual(host['mode'], HOST)
        self.assertEqual(host['connected'], [HOST])
        self.assertEqual(host['dual'], 1)
        self.assertEqual(native.poll(0)['mode'], INVALID)
        device = native.poll(1)
        self.assertEqual(device['mode'], INVALID)
        self.assertEqual(device['connected'], [])
        self.assertEqual(native.start_device(), 0)
        self.assertEqual(native.snapshot()['mode'], DEVICE)
        self.assertEqual(native.snapshot()['connected'], [DEVICE])

    def test_previous_board_misses_host_when_no_detach_is_sampled(self):
        native = NativeUsbConfig('previous_board')
        self.assertEqual(native.start_device(), 0)
        native.poll(1)
        for _ in range(3):
            result = native.poll(2)
            self.assertEqual(result['role'], 2)
            self.assertEqual(result['connected'], [DEVICE])
            self.assertEqual(result['mode'], DEVICE)
        self.assertEqual(native.start_calls, [(0, DEVICE)])

    def test_role_change_while_connected_in_both_directions(self):
        native = NativeUsbConfig()
        native.power_role = 1  # Data-role changes can occur without a power-role swap.
        self.assertEqual(native.start_device(), 0)
        native.poll(1)
        for role, expected in ((2, [HOST]), (1, [])):
            for _ in range(4):
                state = native.poll(role)
                self.assertEqual(state['role'], role)
            self.assertEqual(state['connected'], expected)
            self.assertEqual(state['mode'], HOST if role == 2 else INVALID)
        self.assertEqual(native.start_device(), 0)
        self.assertEqual(native.snapshot()['connected'], [DEVICE])
        self.assertEqual([event for event in native.events if event[0] in ('connect', 'disconnect')], [
            ('connect', DEVICE, 0), ('disconnect', DEVICE, 0), ('connect', HOST, 0),
            ('disconnect', HOST, 0), ('connect', DEVICE, 0)])

    def test_device_to_host_only_publishes_one_host_controller(self):
        for variant, expected_counts in (('edge_without_retire', [2, 3]), ('board', [1, 1])):
            with self.subTest(variant=variant):
                native = NativeUsbConfig(variant)
                native.start_device()
                for role in (1, 2, 2):
                    native.poll(role)
                # ConnectAll enumerates every published handle. Both old
                # and new handles see the mode in the *same* interface.
                def host_count():
                    return sum(native.r32(interface + 0x8C) == HOST
                               for _, interface in native.protocols.values())
                counts = [host_count()]
                native.poll(0)
                if variant == 'board':
                    self.assertEqual(native.protocols, {})
                native.poll(2)
                counts.append(host_count())
                self.assertEqual(counts, expected_counts)

    def test_retirement_failure_retains_owner_and_allows_retry(self):
        for mode, slot in [(HOST, 0x152C0), (DEVICE, 0x152C8)]:
            for failure in ('disconnect_status', 'uninstall_status'):
                with self.subTest(mode=mode, failure=failure):
                    native = NativeUsbConfig()
                    if mode == HOST:
                        native.poll(2)
                    else:
                        native.start_device()
                    handle = native.r64(BASE + slot)
                    setattr(native, failure, EFI_DEVICE_ERROR)
                    self.assertEqual(native.invoke(0x4D34, 0, mode), EFI_DEVICE_ERROR)
                    self.assertEqual(native.r64(BASE + slot), handle)
                    self.assertIn(handle, native.protocols)
                    self.assertEqual(native.snapshot()['mode'], mode)
                    setattr(native, failure, 0)
                    self.assertEqual(native.invoke(0x4D34, 0, mode), 0)
                    self.assertEqual(native.r64(BASE + slot), 0)
                    self.assertEqual(native.protocols, {})
                    self.assertEqual(native.snapshot()['mode'], INVALID)

    def test_role_change_leaves_fixed_device_policy_active(self):
        native = NativeUsbConfig()
        self.assertEqual(native.start_device(), 0)
        native.poll(1)
        native.uc.mem_write(BASE + 0x15794, bytes([0]))  # Fixed-role policy
        before = native.events.copy()
        for _ in range(3):
            result = native.poll(2)
            self.assertEqual(result['connected'], [DEVICE])
            self.assertEqual(result['connection'], 2)
        self.assertEqual(native.events, before)

    def test_simulated_dfp_does_not_disconnect_the_active_device(self):
        native = NativeUsbConfig()
        self.assertEqual(native.start_device(), 0)
        native.poll(1)
        native.uc.mem_write(BASE + 0x15824, bytes([1]))  # Simulate DFP
        for _ in range(3):
            self.assertEqual(native.poll(1)['connected'], [DEVICE])
        self.assertNotIn(('disconnect', DEVICE, 0), native.events)

    def test_startup_host_and_repeated_poll_do_not_duplicate_connect(self):
        native = NativeUsbConfig()
        first = native.poll(2)
        for _ in range(5):
            native.poll(2)
        self.assertEqual(first['mode'], HOST)
        self.assertEqual(native.snapshot()['start_calls'], first['start_calls'])
        self.assertEqual(sum(e[0] == 'connect' for e in native.events), 1)
        native.poll(0)
        self.assertEqual(native.poll(2)['connected'], [HOST])
        self.assertEqual(sum(e[0] == 'connect' for e in native.events), 2)

    def test_role_zero_and_invalid_never_start_host(self):
        for role in (0, 3):
            with self.subTest(role=role):
                native = NativeUsbConfig()
                result = native.poll(role)
                self.assertEqual(result['mode'], INVALID)
                self.assertEqual(result['start_calls'], 0)
                self.assertEqual(result['connected'], [])

    def test_getter_error_preserves_preexisting_native_fallback(self):
        for variant in ('original', 'board'):
            with self.subTest(variant=variant):
                native = NativeUsbConfig(variant)
                native.getter_status = EFI_DEVICE_ERROR
                failed = native.poll(2)
                self.assertEqual(failed['connected'], [])
                self.assertEqual(failed['platform'], 0)
                self.assertEqual(failed['getter_calls'], 1)
                native.getter_status = 0
                subsequent = native.poll(2)
                self.assertEqual(subsequent['getter_calls'], 1)
                self.assertEqual(subsequent['role'], 1)
                self.assertNotIn(HOST, subsequent['connected'])

    def test_connect_error_does_not_claim_connected_controller(self):
        native = NativeUsbConfig()
        native.connect_status = EFI_DEVICE_ERROR
        result = native.poll(2)
        # Native StartController updates mode before ConnectController. Its
        # cached mode alone cannot prove hardware initialization succeeded.
        self.assertEqual(result['mode'], HOST)
        self.assertEqual(result['connected'], [])
        self.assertIn(('connect', HOST, EFI_DEVICE_ERROR), native.events)


if __name__ == '__main__':
    unittest.main(verbosity=2)
