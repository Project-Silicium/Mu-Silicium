# USB support on onyx

## Role selection and controller lifetime

The native UsbPwrCtrl PMIC backend supplies the Type-C data role. In dual-role
mode UsbConfig selects core 0 xHCI for DFP (data role 2, mode 1). After UFP
(data role 1), the project's Mass Storage application can start DeviceSS
(mode 4). Host storage and keyboards use the existing xHCI, UsbBusDxe,
UsbMassStorageDxe and UsbKbDxe drivers. The boot menu uses its standard
USB Storage entry.

`prepare_usbconfig.py` uses the Binary repository's onyx UsbConfig image.
It checks the input hash, instruction bytes and dependency expression before
generating the board copy. The relevant RVAs in that input are:

- `0xe8ba` enables the native automatic controller start on DFP attach.
- `0x4990` keeps the native dual-role selection active after first attach.
- `0x782c` dispatches a connected UFP/DFP reversal through the native detach
  handler. The next physical poll can then attach the newly selected role.
- `0x4e54` and `0x4e9c` retire the previous core 0 role interface after the
  native disconnect succeeds. Leaving it published gives both handles the
  same xHCI device path. A failed uninstall retains ownership for a retry.

The added instructions and assembly commands are in `UsbConfigRoleEdge.S`.
The build uses its hash-checked machine code in `UsbConfigRoleEdge.bin`.

## Power and platform dependencies

The supply driver receives Android MCA notifications over the PMIC GLink
channel and configures the SC8581 over I²C. It registers the callbacks
before PmicGlinkDxe starts. Its second entry point starts the ADSP/PMIC
handshake at application TPL and arms the supply worker. The onyx package
includes the IPCC mailbox driver and the ADSP memory reservation used by
this sequence. See `PMIC-USB.md` for the native charger query fix.

## Build and verification

The onyx `DeviceBuild.py` pre-build step generates the PIL, PMIC and
UsbConfig EFI files and copies their dependency expressions from
`Binaries/onyx`. For a direct EDK build or a standalone test run on a fresh
checkout, run `prepare_pil.py`, `prepare_pmic.py` and `prepare_usbconfig.py`
from this directory first.

- `Tests/run_tests.py --cc <gcc>` checks the packaged drivers and exercises
  the SC8581 supply and IPCC lifecycle with mocked firmware services.
- `Tests/test_native_usbconfig.py` uses Unicorn and pefile to execute the
  packaged AArch64 role policy with simulated hardware and boot services.

The device owner confirmed USB Storage boot from an OTG disk, keyboard
input in the boot menu and gadget Mass Storage on the cleaned onyx image
with SHA-256 `63a619f9a388f893f5b6d7b514853df7b4581fc31eb15659ffdbf86cc0fde101`.
