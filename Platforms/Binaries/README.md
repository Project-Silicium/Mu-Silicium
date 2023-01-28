# Binaries from Qualcomm Snapdragon Devices

## Patches

## Xiaomi 11T Pro

### UFSDxe

- *MMU related setup routine was patched to not recreate already existing MMU domains.*

### ButtonsDxe

- *Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.*

### PmicDxe

- *PMIC AUX (LEICA) init sequence was patched to not fail due to the PMIC AUX chip already being powered on.*

### ClockDxe

- *DCD Dependency enablement path was patched to not cause MDSS to reinitialize and thus lose framebuffer.*

### FeatureEnablerDxe & MinidumpTADxe

- *Both DXEs were patched to not start again the TZ applet given it was already brought up*

### QcomWDogDxe

- *Dependency check routine was patched to not fail due to ReturnStatusCodeHandler implementation being different.*

### UsbConfigDxe

- *Exit BootServices routine was patched to not deinit USB after exit boot services. Another patch disables recreating IOMMU domains.*

## Redmi Note 8/8T

### ButtonsDxe

- *Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.*

## Redmi 9T

### ButtonsDxe

- *Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.*

## Mi A3

### ButtonsDxe

- *Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.*

## Poco F1

### HALIOMMU

- *MMU related setup routine was patched to not recreate already existing MMU domains and thus crash UEFI.*

### ButtonsDxe

- *Key code was patched for the power button to be mapped as ENTER instead of SUSPEND.*
