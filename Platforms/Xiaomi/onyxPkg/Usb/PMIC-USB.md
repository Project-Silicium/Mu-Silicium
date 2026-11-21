# Native PMIC USB attach crash

The November 21, 2026 crash report reaches `SPMI+0x3758` through
`SPMI+0x1544`, `UsbPwrCtrlDxe+0x5290/+0x4638`, and
`UsbConfigDxe+0x36a8/+0x49c8`. SPMI's protocol table confirms that this
is **WriteLong**, not ReadLongDebug. The fault instruction reads the
write channel's STATUS before submitting the write. FAR/ESR are still
needed to distinguish a channel access fault from other MMIO failures.

## Identified native call path

- `UsbPwrCtrlLib_GetHSUsbChgPortType` calls PMIC protocol
  `AE6AE96E-483F-42AE-9CC1-9FAC1B584728`, revision `0x10026`, slot `0x28`.
- That slot is `EFI_PmicSchgGetChargerPortType` at PMIC RVA `0x23e2c`.
- Its native type >= 7 fallback still calls `pm_comm_write_byte` at
  `0x24374`: bus 0, SID 7, register `0x2a52`, value `0x0a`, priority 1,
  length 1. This uses the SPMI WriteLong protocol slot `+0x10`.
- The next six direct PMIC writes are already NOPs in the upstream
  `PmicDxeLa.patched.efi`. The first call was left active.

`prepare_pmic.py` extends that existing protected-write patch by replacing
only this missed BL instruction with a NOP. Its result is unused; the
native function sets `x0=0` at `0x243e8`. The charger type output is not
changed or forced to SDP. The script checks the complete input SHA256
and the expected instruction, translates its RVA through the PE section
table, and leaves the Binary repository untouched.

USB Host attachment can enter this path while the controller is still
in Device mode. The native port callback queries the charger type before
switching modes. A pending Host role bypasses the gadget SDP/CDP decision
and proceeds to `UsbConfigDxe+0x44b8(0, 1)`; the native Unknown charger
type therefore does not need to be falsified to enable Host.

## Preparation and verification

The onyx `DeviceBuild.py` pre-build hook runs `prepare_pmic.py`. For a
direct BaseTools build, first run
`python Platforms/Xiaomi/onyxPkg/Usb/prepare_pmic.py`. Use its `--check`
option to detect missing or stale local outputs.
`Tests/run_tests.py` verifies the input hash, the exact four changed
bytes, unchanged neighbouring patches and DEPEX, firmware-version
rejection, and onyx-only FV/APRIORI selection.

The local INF keeps the original PMIC file GUID and dependencies. Only
the onyx DSC/FV references select it; other devices retain their existing
PMIC drivers. SC8581, UCSI, Host, keyboard, Mass Storage, and boot menu
code are not modified by this fix.

The original stack omitted the PMIC frame. Subsequent tests on the cleaned
onyx image confirmed successful OTG disk boot and keyboard input through
the USB attach path.
