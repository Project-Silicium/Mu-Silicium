# DFCI Stand Alone Recovery shell application

## Overview

DFCI Stand Alone Recovery is the same code as the menu item Refresh From Network, and is used to
execute the Refresh From Network operation with DEBUG prints routed to the console.

## Building the DfciSARecovery module

To get the debug lib output to the console, build DfciSARecovery with the UefiDebugLibConOut
library, add the following to your platform build .dsc file:

```ini
 DfciPkg/Application/DfciMenu/DfciSARecovery.inf {
  <LibraryClasses>
    DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
}
```

## Running the DfciSARecovery application

In order to run the DfciSARecovery application, the system must be able to boot to the Shell.
Insure that any device settings that disable booting from USB are set to enable booting from USB.
In addition, the InTune setting Dfci.BootExternalMedia.Enable must be enabled.

Prepare a USB drive that will boot to the UEFI Shell.
It is beyond the scope of this document to describe how to build the shell itself, or how to
create a USB drive that boots to the shell.
For information on the UEFI shell, visit:

* <https://github.com/tianocore/tianocore.github.io/wiki/ShellPkg>

Copy the DfciSARecovery.efi application to the USB Drive.

Using your platform mechanism, boot to the USB drive just created.

At the **Shell>** prompt, log into the USB drive and run DfciSARecovery.
For example:

```ini
  fs0:
  DfciSARecovery >a RefreshLog.txt
```
