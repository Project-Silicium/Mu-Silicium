# USB on REDMI Turbo 4 Pro (onyx)

The Qualcomm USB configuration driver follows the Type-C data role reported by
`UsbPwrCtrlDxe`. A DFP attachment starts xHCI on core 0. The existing EDK II
USB bus, keyboard and mass-storage drivers provide menu input and boot through
the standard **USB Storage** entry. In UFP mode the project's **Mass Storage**
application uses the native device controller.

## OTG power

`OnyxIpccDxe` acknowledges the mailbox interrupt and delivers GLink callbacks
at `TPL_CALLBACK`. The early supply driver attaches to the PMIC router before
`PmicGlinkDxe` opens its channel. After that driver is dispatched, the supply
bootstrap starts the ADSP/PMIC handshake and initializes the SC8581 at
`TPL_APPLICATION`, where PIL can perform file I/O. The worker then processes
Android MCA notifications and switches the charge pump over the native I²C
protocol. The charge-pump configuration comes from the onyx device tree and
kernel driver. [Charger device tree](https://github.com/xiaomi-onyx-playground/android_kernel_xiaomi_sm8735-devicetrees/blob/f4fbbed3359e85ffee608a0ff4359fa462735614/qcom/onyx-charger-common.dtsi) · [SC8581 driver](https://github.com/xiaomi-onyx-playground/android_kernel_xiaomi_sm8735/blob/7750565ada3d57e195798519b005d2b038e888c1/drivers/power/supply/mca/mca_hardware_ic/charge_pump_ic/sc8581/sc8581.c)

## Board firmware preparation

The onyx pre-build hook runs `prepare_firmware.py` against the pinned images
under `Binaries/onyx`. It checks their SHA-256 and the original bytes at each
patch site. The generated drivers keep the native file GUIDs and DEPEX.

| Driver | Board-specific change |
| --- | --- |
| PIL | At RVA `0x1978`, skip the retail list check that rejects the ADSP device-tree image. The native loader still performs PAS authentication. |
| PMIC | At `0x24374`, omit the remaining protected SPMI write to SID 7, register `0x2a52`, in the charger-port query. The Binary repo already omits the following six writes. |
| UsbConfig | Enable automatic DFP attach at `0xe8ba` and retain dual-role mode at `0x4990`. On a connected role reversal, `0x782c` enters native detach. After disconnect, `0x4e54` and `0x4e9c` retire the old core 0 interface so the new role can reuse the controller device path. |

For direct BaseTools builds, run `python Platforms/Xiaomi/onyxPkg/Usb/prepare_firmware.py`
first. The script assembles `UsbConfigRoleEdge.S` with `clang` and
`llvm-objcopy` from `CLANG_BIN` or `PATH`. Add `--check` to verify the
generated files without replacing them.
