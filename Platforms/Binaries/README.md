# UEFI Binaries

## Patches

## Galaxy Tab S8 5G

#### ButtonsDxe:
- Reason: Helps navigating Menus (e.g. UEFI Menu)
- Patch: Key code was patched for the power button to be mapped as ENTER instead of Vol UP.
- Patch Creator: [Robotix22](https://github.com/Robotix22)

#### ClockDxe:

- Reason: MDSS reinitializes and we lose framebuffer.
- Patch: DCD Dependency enablement path was patched to not cause MDSS to reinitialize.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### ParserDxe:

- Reason: ParsrLck can't be found by the Driver causing the Driver not to load.
- Patch: ParsrLck check has ben patched to not fail.
- Patch Creator: [Robotix22](https://github.com/Robotix22)

#### PmicDxe:

- Reason: Tries to find SPMI handle but failes causing an crash.
- Patch: SPMI handle checks has ben patched to always return success.
- Patch Creator: [Robotix22](https://github.com/Robotix22)

#### HALIOMMU:

- Reason: A DTB Protocol can't be found, causing the driver to crash.
- Patch: DTB Protocol check has be patched to always return Success.
- Patch Creator: [Robotix22](https://github.com/Robotix22)

#### SdccDxe **(NOT FINISCHED!)**:

- Reason: An MMU Domain is already setup by the previous firmware and gets re-set again, causing the driver to crash.
- Patch: MMU related setup routine was patched to not recreate already existing MMU domains.
- Patch Creator: [Robotix22](https://github.com/Robotix22)

#### UFSDxe:

- Reason: An MMU Domain is already setup by the previous firmware and gets re-set again, causing the driver to crash.
- Patch: MMU related setup routine was patched to not recreate already existing MMU domains, another Patch removes invaild return value.
- Patch Creator: [Robotix22](https://github.com/Robotix22)

#### UsbConfigDxe:

- Reason: Is Important to get USB working in Windows / Linux.
- Patch: Exit BootServices routine was patched to not deinit USB after exit boot services. Another patch disables recreating IOMMU domains.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

## Galaxy Z Fold 3 5G

#### ButtonsDxe:
- Reason: Helps navigating Menus (e.g. UEFI Menu)
- Patch: Key code was patched for the power button to be mapped as ENTER instead of Vol Down.
- Patch Creator: [Robotix22](https://github.com/Robotix22)

#### ClockDxe:

- Reason: MDSS reinitializes and we lose framebuffer.
- Patch: DCD Dependency enablement path was patched to not cause MDSS to reinitialize.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### FeatureEnablerDxe & MinidumpTADxe:

- Reason: The TZ applet it already brought up.
- Patch: Both DXEs were patched to not start again the TZ applet.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### QcomWDogDxe:

- Reason: ReturnStatusCodeHandler implementation is different.
- Patch: Dependency check routine was patched to not fail.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### UFSDxe:

- Reason: An MMU Domain is already setup by the previous firmware and gets re-set again, causing a crash.
- Patch: MMU related setup routine was patched to not recreate already existing MMU domains.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### UsbConfigDxe:

- Reason: Is Important to get USB working in Windows / Linux.
- Patch: Exit BootServices routine was patched to not deinit USB after exit boot services. Another patch disables recreating IOMMU domains.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

## Xiaomi 11T Pro

#### ButtonsDxe:

- Reason: Helps navigating Menus (e.g. UEFI Menu)
- Patch: Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### ClockDxe:

- Reason: MDSS reinitializes and we lose framebuffer.
- Patch: DCD Dependency enablement path was patched to not cause MDSS to reinitialize.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### FeatureEnablerDxe & MinidumpTADxe:

- Reason: The TZ applet it already brought up.
- Patch: Both DXEs were patched to not start again the TZ applet.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### ParserDxe:

- Reason: ParsrLck can't be found by the Driver causing the Driver not to load.
- Patch: ParsrLck check has ben patched to not fail.
- Patch Creator: [Robotix22](https://github.com/Robotix22)

#### PmicDxe:

- Reason: The PMIC AUX chip is already powered on by the previous firmware.
- Patch: PMIC AUX (LEICA) init sequence was patched to not fail.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### QcomWDogDxe:

- Reason: ReturnStatusCodeHandler implementation is different.
- Patch: Dependency check routine was patched to not fail.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### UFSDxe:

- Reason: An MMU Domain is already setup by the previous firmware and gets re-set again, causing a crash.
- Patch: MMU related setup routine was patched to not recreate already existing MMU domains.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### UsbConfigDxe:

- Reason: Is Important to get USB working in Windows / Linux.
- Patch: Exit BootServices routine was patched to not deinit USB after exit boot services. Another patch disables recreating IOMMU domains.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

## Mi A3

#### ButtonsDxe:

- Reason: Helps navigating Menus (e.g. UEFI Menu)
- Patch: Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### ClockDxe:

- Reason: A feature that uses the NPA protocol has been removed to avoid causing a crash
- Patch: Npa Protocol Check has ben removed to not cause a crash
- Patch Creator: [Kernel357](https://github.com/Kernel357) & [Robotix22](https://github.com/Robotix22)

#### UFSDxe:

- Reason: An MMU Domain is already setup by the previous firmware and gets re-set again, causing a crash.
- Patch: MMU related setup routine was patched to not recreate already existing MMU domains.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

## Redmi Note 8/8T

#### ButtonsDxe:

- Reason: Helps navigating Menus (e.g. UEFI Menu)
- Patch: Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### ClockDxe:

- Reason: A feature that uses the NPA protocol has been removed to avoid causing a crash
- Patch: Npa Protocol Check has ben removed to not cause a crash
- Patch Creator: [Kernel357](https://github.com/Kernel357) & [Robotix22](https://github.com/Robotix22)

## Redmi 9T

#### ButtonsDxe:

- Reason: Helps navigating Menus (e.g. UEFI Menu)
- Patch: Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.
- Patch Creator: [Gustave Monce](https://github.com/gus33000)

#### ClockDxe:

- Reason: A feature that uses the NPA protocol has been removed to avoid causing a crash
- Patch: Npa Protocol Check has ben removed to not cause a crash
- Patch Creator: [Kernel357](https://github.com/Kernel357) & [Robotix22](https://github.com/Robotix22)
