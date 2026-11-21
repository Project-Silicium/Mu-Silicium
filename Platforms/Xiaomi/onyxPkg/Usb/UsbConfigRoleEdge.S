// SPDX-License-Identifier: BSD-2-Clause-Patent
//
// QueryConnectionChange, physical port, after a successful PMIC read.
// x20: port context; x24: page of the native platform and policy variables.
// Only scratch registers w8/w9 are used. All targets are image-relative.
// To reproduce UsbConfigRoleEdge.bin:
// llvm-mc -triple=aarch64-none-elf -filetype=obj UsbConfigRoleEdge.S -o role.o
// llvm-objcopy -O binary --only-section=.text role.o UsbConfigRoleEdge.bin
.text
.balign 4
RoleEdge:
    .set NativeRoleSwitch, RoleEdge + (0x7830 - 0x17000)
    .set NativeDetach,     RoleEdge + (0x785c - 0x17000)
    ldr     w8, [x20, #0x10]        // Original instruction at 0x782c: current DataRole.
    ldrb    w9, [x20, #8]           // Physical getter's Connected flag.
    cbz     w9, original
    ldr     w9, [x20, #4]           // Native connection state from the previous poll.
    cmp     w9, #2
    b.ne    original
    ldrb    w9, [x24, #0x820]       // Ignore the firmware's role overrides.
    cbnz    w9, original
    ldrb    w9, [x24, #0x824]
    cbnz    w9, original            // The third override bypasses this site entirely.
    ldrb    w9, [x24, #0x794]       // Active native dual-role policy.
    cmp     w9, #1
    b.ne    original
    ldrb    w9, [x24, #0x3d8]       // Previous selected mode valid.
    cmp     w9, #1
    b.ne    original
    ldr     w9, [x24, #0x3dc]
    cmp     w8, #1                  // UFP after Host.
    b.ne    check_dfp
    cmp     w9, #1                  // XHCI mode.
    b.eq    detach
    b       original
check_dfp:
    cmp     w8, #2                  // DFP after Device.
    b.ne    original
    cmp     w9, #4                  // DeviceSS mode.
    b.eq    detach
original:
    b       NativeRoleSwitch
detach:
    // Complete the old role with the existing detach handler. On the next
    // poll the same physical role reaches the ordinary native attach path.
    b       NativeDetach

// StartController publishes one role handle at a time, using the same
// UsbConfig interface for core 0. DisconnectController unbinds drivers but
// leaves that interface published. Retire it before clearing its handle.
// Both hooks run inside StopController with the native frame/LR saved.
// x20 is the base of the native data page and is callee-saved.
.global RetireDeviceHandle
RetireDeviceHandle:
    ldr     x0, [x20, #0x2c8]
    bl      UnpublishConfig
    mov     x19, x0
    tbnz    x0, #63, retire_error
    mov     x8, x20                 // Replaces ADRP x8, 0x15000.
    b       RoleEdge + (0x4e58 - 0x17000)

.global RetireHostHandle
RetireHostHandle:
    ldr     x0, [x20, #0x2c0]
    bl      UnpublishConfig
    mov     x19, x0
    tbnz    x0, #63, retire_error
    str     xzr, [x20, #0x2c0]     // Original instruction after successful teardown.
    b       RoleEdge + (0x4ea0 - 0x17000)
retire_error:
    b       RoleEdge + (0x4f34 - 0x17000)  // Return EFI_STATUS with slot and mode intact.

UnpublishConfig:
    sub     x1, x20, #0x1000
    add     x1, x1, #0xe8          // The GUID passed to InstallMultipleProtocolInterfaces.
    add     x2, x20, #0x2e0        // The original core 0 protocol interface.
    ldr     x8, [x20, #0x6f0]
    ldr     x8, [x8, #0x90]       // EFI_BOOT_SERVICES.UninstallProtocolInterface
    br      x8
