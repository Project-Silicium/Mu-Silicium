import React from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Badge } from "@/components/ui/badge";
import { Separator } from "@/components/ui/separator";
import { AlertTriangle, Smartphone, Wrench, GitBranch, FileText, Download, ExternalLink } from 'lucide-react';
import { Alert, AlertDescription } from "@/components/ui/alert";

const SnapdragonPortingGuide = () => {
  return (
    <div className="container mx-auto px-4 py-8 max-w-5xl">
      {/* Header */}
      <div className="text-center mb-12">
        <div className="flex items-center justify-center gap-3 mb-4">
          <Smartphone className="h-10 w-10 text-primary" />
          <h1 className="text-4xl font-bold bg-gradient-to-r from-primary to-primary/60 bg-clip-text text-transparent">
            Snapdragon Device Porting Guide
          </h1>
        </div>
        <p className="text-xl text-muted-foreground max-w-3xl mx-auto">
          Complete guide for adding and porting Snapdragon devices to Project Silicium
        </p>
        <div className="flex items-center justify-center gap-2 mt-4">
          <Badge variant="outline" className="text-sm">
            <GitBranch className="h-3 w-3 mr-1" />
            Project Silicium
          </Badge>
          <Badge variant="secondary" className="text-sm">Snapdragon SoC</Badge>
        </div>
      </div>

      {/* Warning Alert */}
      <Alert className="mb-8 border-amber-200 bg-amber-50 dark:border-amber-900 dark:bg-amber-950">
        <AlertTriangle className="h-4 w-4 text-amber-600" />
        <AlertDescription className="text-amber-800 dark:text-amber-200">
          <strong>Important:</strong> This guide assumes you have basic knowledge of device tree structure, 
          kernel compilation, and UEFI development. Proceed with caution as improper modifications can brick your device.
        </AlertDescription>
      </Alert>

<Alert className="mb-8 border-amber-200 bg-amber-50 dark:border-amber-900 dark:bg-amber-950">
        <AlertTriangle className="h-4 w-4 text-amber-600" />
        <AlertDescription className="text-amber-800 dark:text-amber-200">
          <strong>ALERT:</strong>This Guide is Outdated! Remasters of these Guides will come soon.
        </AlertDescription>
      </Alert>

      <div className="grid gap-8">
        {/* Prerequisites */}
        
        <Card>
        <Alert className="mb-8 border-amber-200 bg-amber-50 dark:border-amber-900 dark:bg-amber-950">
        <AlertTriangle className="h-4 w-4 text-amber-600" />
        <AlertDescription className="text-amber-800 dark:text-amber-200">
          <strong><b>Warning:</b></strong> Booting Windows/Linux on Sony/Google Device will wipe your UFS Clean! (Unable to recover)
        </AlertDescription>
      </Alert>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Wrench className="h-5 w-5" />
              Description
            </CardTitle>
            <CardDescription>
              This Guide will show you how to create a UEFI Port for your Device.
            </CardDescription>
          </CardHeader>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Wrench className="h-5 w-5" />
              Requirements:
            </CardTitle>
            <CardDescription>
              To Port UEFI to your Phone, It needs the following things:
            </CardDescription>
          </CardHeader>
          <CardContent className="space-y-4">
            <div className="grid md:grid-cols-2 gap-4">
              <div className="space-y-3">
                {/*<h4 className="font-semibold">Hardware Requirements</h4>*/}
                <ul className="space-y-2 text-sm text-muted-foreground">
                  <li className="flex items-start gap-2">
                    <span className="w-1.5 h-1.5 bg-primary rounded-full mt-2 flex-shrink-0"></span>
                    Snapdragon-powered device with unlocked bootloader
                  </li>
                  <li className="flex items-start gap-2">
                    <span className="w-1.5 h-1.5 bg-primary rounded-full mt-2 flex-shrink-0"></span>
                    xbl or uefi partition in /dev/block/by-name/
                  </li>
                  <li className="flex items-start gap-2">
                    <span className="w-1.5 h-1.5 bg-primary rounded-full mt-2 flex-shrink-0"></span>
                    fdt in /sys/firmware/
                  </li>
                  <li className="flex items-start gap-2">
                    <span className="w-1.5 h-1.5 bg-primary rounded-full mt-2 flex-shrink-0"></span>
                    A brain with knowledge in using Git and terminal
                  </li>
                </ul>
              </div>
              <div className="space-y-3">
                <h4 className="font-semibold">Requirements recomended but optional</h4>
                <ul className="space-y-2 text-sm text-muted-foreground">
                  <li className="flex items-start gap-2">
                    <span className="w-1.5 h-1.5 bg-primary rounded-full mt-2 flex-shrink-0"></span>
                    Ubuntu 24.04  or Arch linux or any debian Linux destribution
                  </li>
                  <li className="flex items-start gap-2">
                    <span className="w-1.5 h-1.5 bg-primary rounded-full mt-2 flex-shrink-0"></span>
                    WSL (WIndows SubSystem for Linux) or Virtual machine with linux installed
                  </li>
                  <li className="flex items-start gap-2">
                    <span className="w-1.5 h-1.5 bg-primary rounded-full mt-2 flex-shrink-0"></span>
                    Nano or vim in linux (there are text editors)
                  </li>
                </ul>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Step 1: Device Tree Setup */}
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <FileText className="h-5 w-5" />
              Step 1: Copying Files
            </CardTitle>
            <CardDescription>
              Copying some necessary files to your computer
            </CardDescription>
          </CardHeader>
          <CardContent className="space-y-6">
            <div className="space-y-4">
              <div className="bg-muted/50 p-4 rounded-lg">
                <h4 className="font-semibold mb-2">1.1 Copying fdt </h4>
                <p className="text-muted-foreground">
Lets begin with Copying Files.
  Copy the <code className="bg-muted px-1 rounded text-sm font-mono">fdt</code> file from 
  <code className="bg-muted px-1 rounded text-sm font-mono ml-1">/sys/firmware/</code></p>
  <p className="text-muted-foreground">
  you can get it using adb with root:
</p>

			<div className="bg-background p-3 rounded border font-mono text-sm">
			<p><code>{'adb shell "dd if=/sys/firmware/fdt of=/sdcard/<Device Codename>.img"'}</code></p>
			<p><code>{'adb pull /sdcard/<Device Codename>.img .'}</code></p>
                </div>
                <p className="text-muted-foreground mb-4">
		Rename <code className="bg-muted px-1 rounded text-sm font-mono">&lt;Device Codename&gt;.img</code> to <code className="bg-muted px-1 rounded text-sm font-mono">&lt;Device Codename&gt;.dtb</code>
		</p>
		<Alert className="mb-8 border-amber-200 bg-amber-50 dark:border-amber-900 dark:bg-amber-950">
        <AlertTriangle className="h-4 w-4 text-amber-600" />
        <AlertDescription className="text-amber-800 dark:text-amber-200">
          <strong>Note:</strong>        If it doesn't works for some devices(i.e. stuck on download mode with Samsung logo) try extracting dtb from boot.img
        </AlertDescription>
      </Alert>
      <p className="text-sm text-muted-foreground mb-4">
  Unpack the stock <code className="bg-muted px-1 rounded text-sm font-mono">boot.img</code> using Android Image Kitchen (AIK), and navigate to the <code className="bg-muted px-1 rounded text-sm font-mono">split_img</code> directory.
  There, you will find a file named <code className="bg-muted px-1 rounded text-sm font-mono">boot.img-dtb</code>.
  Rename this file to <code className="bg-muted px-1 rounded text-sm font-mono">&lt;Device Codename&gt;.dtb</code>.
</p>
<Alert className="mb-8 border-amber-200 bg-amber-50 dark:border-amber-900 dark:bg-amber-950">
        <AlertTriangle className="h-4 w-4 text-amber-600" />
        <AlertDescription className="text-amber-800 dark:text-amber-200">
          <strong>Note:</strong>        This can be maked only on Wsl or an Linux Distro
        </AlertDescription>
      </Alert>

 <div className="bg-background p-3 rounded border font-mono text-sm mb-4">
			<p><code>{'adb shell "dd if=/sys/firmware/fdt of=/sdcard/<Device Codename>.img"'}</code></p>
			<p><code>{'adb pull /sdcard/<Device Codename>.img .'}</code></p>
                </div>

<p className="text-sm text-muted-foreground mb-4">
  Now copy <code className="bg-muted px-1 rounded text-sm font-mono">.dts</code> and <code className="bg-muted px-1 rounded text-sm font-mono">.dtb</code> to <code className="bg-muted px-1 rounded text-sm font-mono">Mu-Silicium/Resources/DTBs/</code>.
</p>

<p className="text-sm text-muted-foreground mb-4">
  Now extract your <code className="bg-muted px-1 rounded text-sm font-mono">xbl</code> or <code className="bg-muted px-1 rounded text-sm font-mono">uefi</code> from <code className="bg-muted px-1 rounded text-sm font-mono">/dev/block/by-name/</code> and place it somewhere you can reach it:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'adb shell'}</code></p>
  <p><code>{'dd if=/dev/block/by-name/<UEFI Partition> of=/<UEFI Partition>.img'}</code></p>
  <p><code>{'exit'}</code></p>
  <p><code>{'adb pull /<UEFI Partition>.img'}</code></p>
</div>

<p className="text-sm text-muted-foreground mb-4">
  After copying the <code className="bg-muted px-1 rounded text-sm font-mono">xbl</code> file or the <code className="bg-muted px-1 rounded text-sm font-mono">uefi</code> file, extract all UEFI binaries from it using <code className="bg-muted px-1 rounded text-sm font-mono">UEFIReader</code>.<br />
  A compiled version is pinned in <code className="bg-muted px-1 rounded text-sm font-mono">#general</code> in our Discord.<br />
  You can also compile it yourself:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'# Linux'}</code></p>
  <p><code>{'# Install dotnet-sdk-8.0 for your distribution'}</code></p>
  <p><code>{'git clone https://github.com/WOA-Project/UEFIReader.git'}</code></p>
  <p><code>{'cd UEFIReader/'}</code></p>
  <p><code>{'dotnet build UEFIReader.sln'}</code></p>
  <p><code>{'# Now here you have a compiled version. Go to UEFIReader/bin/Debug/net8.0/'}</code></p>
</div>

<p className="text-sm text-muted-foreground mb-4">
  Here is how you use it:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'# Windows'}</code></p>
  <p><code>{'UEFIReader.exe <UEFI Partition>.img out'}</code></p>
  <p><code>{'# Linux'}</code></p>
  <p><code>{'./UEFIReader <UEFI Partition>.img out'}</code></p>
</div>

<p className="text-sm text-muted-foreground mb-4">
  Now move all the output files from UEFIReader into <code className="bg-muted px-1 rounded text-sm font-mono">Mu-Silicium/Binaries/&lt;Device Codename&gt;/</code>.<br />
  Then execute <code className="bg-muted px-1 rounded text-sm font-mono">CleanUp.sh</code> in the binaries folder once.
</p>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Step 2: Create config file for your device */}
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Download className="h-5 w-5" />
              Step 2: Creating the Config File
            </CardTitle>
            <CardDescription>
              Creating a device specific .conf device
            </CardDescription>
          </CardHeader>
          <CardContent className="space-y-6">
            <div className="space-y-4">
              <div className="bg-muted/50 p-4 rounded-lg">
               <p className="text-sm text-muted-foreground mb-4">
  Every device has its own config file to define some device-specific settings, like the SoC.<br />
  Create a file called <code className="bg-muted px-1 rounded text-sm font-mono">&lt;Device Codename&gt;.conf</code> in <code className="bg-muted px-1 rounded text-sm font-mono">Mu-Silicium/Resources/Configs/</code>.<br />
  It should contain at least the following:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'# General Configs'}</code></p>
  <p><code>{'TARGET_DEVICE_VENDOR="<Device Vendor>"'}</code></p>
  <p><code>{'TARGET_MULTIPLE_MODELS=0'}</code></p>
  <p><code>{'TARGET_NUMBER_OF_MODELS=0'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'# Arch Config'}</code></p>
  <p><code>{'TARGET_ARCH="AARCH64"'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'# UEFI FD Configs'}</code></p>
  <p><code>{'TARGET_REQUIRES_BOOTSHIM=1'}</code></p>
  <p><code>{'TARGET_FD_BASE="<FD Base>"'}</code></p>
  <p><code>{'TARGET_FD_SIZE="<FD Size>"'}</code></p>
  <p><code>{'TARGET_FD_BLOCKS="<FD Blocks>"'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'# FDT Configs'}</code></p>
  <p><code>{'TARGET_CREATE_POINTER=0'}</code></p>
  <p><code>{'TARGET_POINTER_ADDRESS=0x0'}</code></p>
</div>

<p className="text-sm text-muted-foreground mb-4">
  <code className="bg-muted px-1 rounded text-sm font-mono">&lt;FD Base/Size Value&gt;</code> is the UEFI FD value from the memory map (<code className="bg-muted px-1 rounded text-sm font-mono">uefiplat.cfg</code>).<br />
  <code className="bg-muted px-1 rounded text-sm font-mono">&lt;FD Blocks&gt;</code> is the number of blocks the UEFI FD has, calculated as <code className="bg-muted px-1 rounded text-sm font-mono">&lt;UEFI FD Size&gt; / 0x1000</code>.<br />
  Modify <code className="bg-muted px-1 rounded text-sm font-mono">TARGET_ARCH</code> according to your architecture.
</p>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Step 3: Creating Files */}
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <GitBranch className="h-5 w-5" />
              Step 3: Creating Files
            </CardTitle>
            <CardDescription>
              Creating .fdf .dec and .dsc files for your device
            </CardDescription>
          </CardHeader>
          <CardContent className="space-y-6">
            <div className="space-y-4">
              <div className="bg-muted/50 p-4 rounded-lg">
                <p className="text-sm text-muted-foreground mb-4">
  Structure of the device files:
</p>
<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'./Platforms/<Device Vendor>/<Device Codename>Pkg/'}</code></p>
  <p><code>{'├── Include'}</code></p>
  <p><code>{'│   ├── ACPI.inc'}</code></p>
  <p><code>{'│   ├── APRIORI.inc'}</code></p>
  <p><code>{'│   ├── DXE.inc'}</code></p>
  <p><code>{'│   └── RAW.inc'}</code></p>
  <p><code>{'├── Library'}</code></p>
  <p><code>{'│   ├── DeviceMemoryMapLib'}</code></p>
  <p><code>{'│   │   ├── DeviceMemoryMapLib.c'}</code></p>
  <p><code>{'│   │   └── DeviceMemoryMapLib.inf'}</code></p>
  <p><code>{'│   └── DeviceConfigurationMapLib'}</code></p>
  <p><code>{'│       ├── DeviceConfigurationMapLib.c'}</code></p>
  <p><code>{'│       └── DeviceConfigurationMapLib.inf'}</code></p>
  <p><code>{'├──FdtBlob'}</code></p>
  <p><code>{'|  └── <SoC Codename>-<Device Vendor>-<Device Codename>.dtb'}</code></p>
  <p><code>{'├── PlatformBuild.py'}</code></p>
  <p><code>{'├── <Device Codename>.dec'}</code></p>
  <p><code>{'├── <Device Codename>.dsc'}</code></p>
  <p><code>{'└── <Device Codename>.fdf'}</code></p>
</div>

              </div>
            </div>
          </CardContent>
          <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.1: Creating .dsc & .dec & .fdf File
    </CardTitle>
      <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.1.1: Creating .dsc File
    </CardTitle>
    <CardDescription>
      This file describes what and how modules, libraries, and components are to be built, as well as defining library instances that will be used when linking EDKII modules.
    </CardDescription>
  </CardHeader>
  </CardHeader>
  <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
<p className="text-sm text-muted-foreground mb-4">
  Create a file named <code>{'<Device Codename>.dsc'}</code> inside the directory:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'./Platforms/<Device Vendor>/<Device Codename>Pkg/<Device Codename>.dsc'}</code></p>
</div>

<p className="text-sm text-muted-foreground mb-4">
  Use the following template:
</p>
<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'##'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.'}</code></p>
  <p><code>{'#  Copyright (c) 2014, Linaro Limited. All rights reserved.'}</code></p>
  <p><code>{'#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.'}</code></p>
  <p><code>{'#  Copyright (c) 2018, Bingxing Wang. All rights reserved.'}</code></p>
  <p><code>{'#  Copyright (c) Microsoft Corporation.'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'#  SPDX-License-Identifier: BSD-2-Clause-Patent'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'##'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'################################################################################'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'# Defines Section - statements that will be processed to create a Makefile.'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'################################################################################'}</code></p>
  <p><code>{'[Defines]'}</code></p>
  <p><code>{'  PLATFORM_NAME                  = <Device Codename>'}</code></p>
  <p><code>{'  PLATFORM_GUID                  = <GUID>'}</code></p>
  <p><code>{'  PLATFORM_VERSION               = 0.1'}</code></p>
  <p><code>{'  DSC_SPECIFICATION              = 0x00010005'}</code></p>
  <p><code>{'  OUTPUT_DIRECTORY               = Build/<Device Codename>Pkg'}</code></p>
  <p><code>{'  SUPPORTED_ARCHITECTURES        = AARCH64'}</code></p>
  <p><code>{'  BUILD_TARGETS                  = RELEASE|DEBUG'}</code></p>
  <p><code>{'  SKUID_IDENTIFIER               = DEFAULT'}</code></p>
  <p><code>{'  FLASH_DEFINITION               = <Device Codename>Pkg/<Device Codename>.fdf'}</code></p>
  <p><code>{'  USE_CUSTOM_DISPLAY_DRIVER      = 0'}</code></p>
  <p><code>{'  HAS_BUILD_IN_KEYBOARD          = 0'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # If your SoC has multimple variants define the Number here'}</code></p>
  <p><code>{'  # If not don\'t add this Define'}</code></p>
  <p><code>{'  SOC_TYPE                       = 2'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'# If your SoC has multiple variants keep these Build Options'}</code></p>
  <p><code>{'# If not don\'t add "-DSOC_TYPE=$(SOC_TYPE)" to the Build Options.'}</code></p>
  <p><code>{'[BuildOptions]'}</code></p>
  <p><code>{'  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'[LibraryClasses]'}</code></p>
  <p><code>{'  DeviceMemoryMapLib|<Device Codename>Pkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf'}</code></p>
  <p><code>{'  DeviceConfigurationMapLib|<Device Codename>Pkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'[PcdsFixedAtBuild]'}</code></p>
  <p><code>{'  # DDR Start Address'}</code></p>
  <p><code>{'  gArmTokenSpaceGuid.PcdSystemMemoryBase|<Start Address>'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # Device Maintainer'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"<Your Github Name>"'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # CPU Vector Address'}</code></p>
  <p><code>{'  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|<CPU Vector Base Address>'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # UEFI Stack Addresses'}</code></p>
  <p><code>{'  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|<UEFI Stack Base Address>'}</code></p>
  <p><code>{'  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|<UEFI Stack Size>'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # SmBios'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"<Device Vendor>"'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"<Device Model>"'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"<Device Codename>"'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"<Device_Model>_<Device_Codename>"'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"<Device Model>"'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # Simple FrameBuffer'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|<Display Width>'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|<Display Height>'}</code></p>
  <p><code>{'  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|<Display Color Depth>'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # Dynamic RAM Start Address'}</code></p>
  <p><code>{'  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|<Free DDR Region Start Address>'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # SD Card Slot'}</code></p>
  <p><code>{'  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE'}</code></p>
  <p><code>{'  # If your Phone has no SD Card Slot, Set it to FALSE.'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'  # USB Controller'}</code></p>
  <p><code>{'  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE'}</code></p>
  <p><code>{'  # This should be TRUE unless your UsbConfigDxe is Patched to be Dual Role.'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'[PcdsDynamicDefault]'}</code></p>
  <p><code>{'  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|<Display Width>'}</code></p>
  <p><code>{'  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|<Display Height>'}</code></p>
  <p><code>{'  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|<Display Width>'}</code></p>
  <p><code>{'  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|<Display Height>'}</code></p>
  <p><code>{'  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|<Setup Con Column>'}</code></p>
  <p><code>{'  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|<Setup Con Row>'}</code></p>
  <p><code>{'  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|<Con Column>'}</code></p>
  <p><code>{'  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|<Con Row>'}</code></p>
  <p><code>{''}</code></p>
  <p><code>{'!include <SoC Codename>Pkg/<SoC Codenmae>.dsc.inc'}</code></p>
</div>
<p className="text-sm text-muted-foreground mb-4 space-y-2">
  <span className="block">
    <strong>&lt;GUID&gt;</strong> is a value to identify your device. Generate one <a href="https://www.guidgenerator.com/" className="underline text-blue-500" target="_blank">here</a>. Make sure it's uppercase.
  </span>
  <span className="block">
    <strong>&lt;Start Address&gt;</strong> is the start address of the MemoryMap (<code>uefiplat.cfg</code>).
  </span>
  <span className="block">
    <strong>&lt;CPU Vector Base Address&gt;</strong> is the base address of CPU vectors in the MemoryMap (<code>uefiplat.cfg</code>).
  </span>
  <span className="block">
    <strong>&lt;UEFI Stack base/Size&gt;</strong> is the base/size address of the UEFI stack in the MemoryMap (<code>uefiplat.cfg</code>).
  </span>
  <span className="block">
    <strong>&lt;Display Color Depth&gt;</strong> is the value of your display's color depth. It can be found in the specifications of your phone, for example at <a href="https://www.devicespecifications.com/" className="underline text-blue-500" target="_blank">www.devicespecifications.com</a>.
  </span>
  <span className="block">
    <strong>&lt;Free DDR Region Start Address&gt;</strong> is the end address of the last DDR memory region. It is calculated as <code>&lt;Base Addr&gt; + &lt;Size Addr&gt; = &lt;End Addr&gt;</code>.
  </span>
  <span className="block">
    <strong>&lt;Setup Con Column&gt;</strong> / <strong>&lt;Con Column&gt;</strong> is the value of <code>&lt;Display Width&gt; / 8</code>.
  </span>
  <span className="block">
    <strong>&lt;Setup Con Row&gt;</strong> / <strong>&lt;Con Row&gt;</strong> is the value of <code>&lt;Display Height&gt; / 19</code>.
  </span>
</p>
      </div>
    </div>
  </CardContent>
  <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.1.2: Creating .dsc File
    </CardTitle>
    <CardDescription>
      This file declares information about what is provided in the package. An EDKII package is a collection of like content.
    </CardDescription>
  </CardHeader>
  <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
        <p className="text-sm">
  After we created the <code>.dsc</code> file, we will now continue with the <code>.dec</code> file.<br />
  Create a file called <code>&lt;Device Codename&gt;.dec</code> in <code>Mu-Silicium/Platforms/&lt;Device Vendor&gt;/&lt;Device Codename&gt;/</code>.<br />
  Follow this template:
</p>
<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
<p><code>{'[Defines]'}</code></p>
  <p><code>{'DEC_SPECIFICATION                   =  0x00010005'}</code></p>
  <p><code>{'PACKAGE_NAME                        = <device_codename>'}</code></p>
  <p><code>{'PACKAGE_GUID                        = <GUID>'}</code></p>
   <p><code>{'PACKAGE_VERSION                     = 0.1'}</code></p>
   <p><code>{'\u00A0'}</code></p>
   <p><code>{'[Guids]'}</code></p>
   <p><code>{'g<device_codename>TokenSpaceGuid              = { guid, pair, one, {guid, pair, two  } }'}</code></p>
   </div>
      <p className="text-sm text-muted-foreground mb-4">
  Suppose this GUID: <strong>9B67CDA8-3412-7856-9ABC-DEF011223344</strong><br />
  The first 3 fields are reversed (little-endian), while the last 8 bytes are not.
</p>

<p className="text-sm text-muted-foreground mb-4">
  For example, in a <strong>
    gDeviceCodenameTokenSpaceGuid = {"{"} guid, pair, one, {"{"} guid, pair, two {"}"} {"}"}
  </strong> line, you define a GUID like this:
</p>

<p className="text-sm text-muted-foreground mb-4">
  <strong>
    {`gMyPkgTokenSpaceGuid = { 0xA8CD679B, 0x1234, 0x5678, { 0x9A, 0xBC, 0xDE, 0xF0, 0x11, 0x22, 0x33, 0x44 } }`}
  </strong>
</p>

<p className="text-sm text-muted-foreground mb-4">
  But internally, the first three fields are stored in <strong>little-endian</strong> format (i.e., byte-reversed).
</p>

<p className="text-sm text-muted-foreground mb-4">
  For instance, the value <strong>0xA8CD679B</strong> is stored as: <strong>9B 67 CD A8</strong>.
</p>

<p className="text-sm text-muted-foreground mb-4">
  Similarly, <strong>0x1234</strong> becomes <strong>34 12</strong>, and <strong>0x5678</strong> becomes <strong>78 56</strong>.
</p>

<p className="text-sm text-muted-foreground mb-4">
  However, the last 8 bytes (from <strong>0x9A</strong> to <strong>0x44</strong>) are stored in the order they are written — no reversal needed.
</p>
	
      </div>
    </div>
  </CardContent>
<CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.1.3: Creating .fdf File
    </CardTitle>
    <CardDescription>
      FDF file describes information about flash parts as well as rules for combining binaries (Firmware Image) built from a DSC file. Additionally, if a DSC file specifies a FLASH_DEFINITION file, then the EDK II tools will locate the FDF file (looking in the same directory as the DSC file, then the parsing utilities will scan the FDF file to gather PCD information that can be used by AutoGen utilities for building components or modules. The output of the first phase of an EDK II build (as defined in the EDK II Build Specification) generates valid PE32/PE32+/Coff image files. The second phase of the build process consumes the images generated during the first phase, using statements and rules defined in the FDF file to process the PE32/PE32+/Coff images files into one or more EFI sections. The EFI sections may get combined with other optional sections (version, depex, user interface) sections, into EFI Firmware File system (FFS) Sections. FFS images are put into Firmware Volumes (FVs,) and finally, the FV sections are combined into one or more Flash Device binary image (FD).
    </CardDescription>
  </CardHeader>
  <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
        <p className="text-sm text mb-4">
  Once the <code>.dec</code> file is complete, we can move on to the <code>.fdf</code> file.<br />
  Create a file called <code>&lt;Device Codename&gt;.fdf</code> in <code>Mu-Silicium/Platforms/&lt;Device Vendor&gt;/&lt;Device Codename&gt;/</code>.<br />
  The <code>.fdf</code> file contains specific stuff about your device. Here is a template of how it should look:
</p>
<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'## @file'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'#  Copyright (c) 2018, Linaro Limited. All rights reserved.'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'#  SPDX-License-Identifier: BSD-2-Clause-Patent'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'##'}</code></p>
  
    <p><code>{'################################################################################'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'# FD Section'}</code></p>
  <p><code>{'# The [FD] Section is made up of the definition statements and a'}</code></p>
  <p><code>{'# description of what goes into  the Flash Device Image. Each FD section'}</code></p>
  <p><code>{'# defines one flash "device" image. A flash device image may be one of'}</code></p>
  <p><code>{'# the following: Removable media bootable image (like a boot floppy'}</code></p>
  <p><code>{'# image,) an Option ROM image (that would be "flashed" into an add-in'}</code></p>
  <p><code>{'# card,) a System "Flash" image (that would be burned into a system\'s'}</code></p>
  <p><code>{'# flash) or an Update ("Capsule") image that will be used to update an'}</code></p>
  <p><code>{'# existing system flash.'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'################################################################################'}</code></p>
  
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'[FD.Device Codename_UEFI]'}</code></p>
  <p><code>{'BaseAddress   = $(FD_BASE)|gArmTokenSpaceGuid.PcdFdBaseAddress'}</code></p>
  <p><code>{'Size          = $(FD_SIZE)|gArmTokenSpaceGuid.PcdFdSize'}</code></p>
  <p><code>{'ErasePolarity = 1'}</code></p>
  <p><code>{'BlockSize     = 0x1000'}</code></p>
  <p><code>{'NumBlocks     = $(FD_BLOCKS)'}</code></p>
    <p><code>{'\u00A0'}</code></p>
    
    <p><code>{'################################################################################'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'# Following are lists of FD Region layout which correspond to the locations of different'}</code></p>
  <p><code>{'# images within the flash device.'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'# Regions must be defined in ascending order and may not overlap.'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'# A Layout Region start with a eight digit hex offset (leading "0x" required) followed by'}</code></p>
  <p><code>{'# the pipe "|" character, followed by the size of the region, also in hex with the leading'}</code></p>
  <p><code>{'# "0x" characters. Like:'}</code></p>
  <p><code>{'# Offset|Size'}</code></p>
  <p><code>{'# PcdOffsetCName|PcdSizeCName'}</code></p>
  <p><code>{'# RegionType <FV, DATA, or FILE>'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'################################################################################'}</code></p>

  
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'0x00000000|$(FD_SIZE)'}</code></p>
  <p><code>{'gArmTokenSpaceGuid.PcdFvBaseAddress|gArmTokenSpaceGuid.PcdFvSize'}</code></p>
  <p><code>{'FV = FVMAIN_COMPACT'}</code></p>
  
  <p><code>{'\u00A0'}</code></p>
    <p><code>{'################################################################################'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'# FV Section'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'# [FV] section is used to define what components or modules are placed within a flash'}</code></p>
  <p><code>{'# device file.  This section also defines order the components and modules are positioned'}</code></p>
  <p><code>{'# within the image.  The [FV] section consists of define statements, set statements and'}</code></p>
  <p><code>{'# module statements.'}</code></p>
  <p><code>{'#'}</code></p>
  <p><code>{'################################################################################'}</code></p>

  
 <p><code>{'\u00A0'}</code></p>
  <p><code>{'[FV.FvMain]'}</code></p>
  <p><code>{'FvNameGuid         = 631008B0-B2D1-410A-8B49-2C5C4D8ECC7E'}</code></p>
  <p><code>{'BlockSize          = 0x1000'}</code></p>
  <p><code>{'NumBlocks          = 0'}</code></p>
  <p><code>{'FvAlignment        = 8'}</code></p>
  <p><code>{'ERASE_POLARITY     = 1'}</code></p>
  <p><code>{'MEMORY_MAPPED      = TRUE'}</code></p>
  <p><code>{'STICKY_WRITE       = TRUE'}</code></p>
  <p><code>{'LOCK_CAP           = TRUE'}</code></p>
  <p><code>{'LOCK_STATUS        = TRUE'}</code></p>
  <p><code>{'WRITE_DISABLED_CAP = TRUE'}</code></p>
  <p><code>{'WRITE_ENABLED_CAP  = TRUE'}</code></p>
  <p><code>{'WRITE_STATUS       = TRUE'}</code></p>
  <p><code>{'WRITE_LOCK_CAP     = TRUE'}</code></p>
  <p><code>{'WRITE_LOCK_STATUS  = TRUE'}</code></p>
  <p><code>{'READ_DISABLED_CAP  = TRUE'}</code></p>
  <p><code>{'READ_ENABLED_CAP   = TRUE'}</code></p>
  <p><code>{'READ_STATUS        = TRUE'}</code></p>
  <p><code>{'READ_LOCK_CAP      = TRUE'}</code></p>
  <p><code>{'READ_LOCK_STATUS   = TRUE'}</code></p>
 <p><code>{'\u00A0'}</code></p>
  <p><code>{'  !include Include/APRIORI.inc'}</code></p>
  <p><code>{'  !include Include/DXE.inc'}</code></p>
  <p><code>{'  !include Include/RAW.inc'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'  INF MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf'}</code></p>
  <p><code>{'  INF QcomPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'  INF MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf'}</code></p>
  <p><code>{'  INF MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'  !include Include/ACPI.inc'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'  !include QcomPkg/Extra.fdf.inc'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'[FV.FVMAIN_COMPACT]'}</code></p>
  <p><code>{'FvAlignment        = 8'}</code></p>
  <p><code>{'ERASE_POLARITY     = 1'}</code></p>
  <p><code>{'MEMORY_MAPPED      = TRUE'}</code></p>
  <p><code>{'STICKY_WRITE       = TRUE'}</code></p>
  <p><code>{'LOCK_CAP           = TRUE'}</code></p>
  <p><code>{'LOCK_STATUS        = TRUE'}</code></p>
  <p><code>{'WRITE_DISABLED_CAP = TRUE'}</code></p>
  <p><code>{'WRITE_ENABLED_CAP  = TRUE'}</code></p>
  <p><code>{'WRITE_STATUS       = TRUE'}</code></p>
  <p><code>{'WRITE_LOCK_CAP     = TRUE'}</code></p>
  <p><code>{'WRITE_LOCK_STATUS  = TRUE'}</code></p>
  <p><code>{'READ_DISABLED_CAP  = TRUE'}</code></p>
  <p><code>{'READ_ENABLED_CAP   = TRUE'}</code></p>
  <p><code>{'READ_STATUS        = TRUE'}</code></p>
  <p><code>{'READ_LOCK_CAP      = TRUE'}</code></p>
  <p><code>{'READ_LOCK_STATUS   = TRUE'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'  INF SiliciumPkg/PrePi/PrePi.inf'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'  FILE FREEFORM = dde58710-41cd-4306-dbfb-3fa90bb1d2dd {'}</code></p>
  <p><code>{'    SECTION UI = "uefiplat.cfg"'}</code></p>
  <p><code>{'    SECTION RAW = Binaries/Device Codename/RawFiles/uefiplat.cfg'}</code></p>
  <p><code>{'  }'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'  FILE FV_IMAGE = 9E21FD93-9C72-4c15-8C4B-E77F1DB2D792 {'}</code></p>
  <p><code>{'    SECTION GUIDED EE4E5898-3914-4259-9D6E-DC7BD79403CF PROCESSING_REQUIRED = TRUE {'}</code></p>
  <p><code>{'      SECTION FV_IMAGE = FVMAIN'}</code></p>
  <p><code>{' \u00A0 \u00A0  }'}</code></p>
  <p><code>{'  }'}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{'  !include SiliciumPkg/Common.fdf.inc'}</code></p>
</div>
</div>
</div>
</CardContent>
<CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.2: Creating .fdf.inc Files
    </CardTitle>
    <CardDescription>
      Now we create some files for the .fdf File.
    </CardDescription>
    <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.2.1: Creating ACPI.inc
    </CardTitle>
    <CardDescription>
      This file defines what ACPI tables need to be added to the UEFI image you will build.
    </CardDescription>
  </CardHeader>
  </CardHeader>

  <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
       <p className="text-sm">
  For now, leave it empty.<br />
  Once your UEFI is working stably, you can follow the ACPI guide.
</p>
      </div>
    </div>
    <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.2.2: Creating APRIORI.inc
    </CardTitle>
    <CardDescription>
      This file is simply a list of FV (Firmware Volume) filenames that the DXE dispatcher will always load—ignoring any dependency rules.
    </CardDescription>
  </CardHeader>
  <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
     <p className="text-sm text-muted-foreground mb-4">
  We continue with <code className="bg-muted px-1 rounded text-sm font-mono">APRIORI.inc</code>.<br />
  Create <code className="bg-muted px-1 rounded text-sm font-mono">APRIORI.inc</code> in <code className="bg-muted px-1 rounded text-sm font-mono">Mu-Silicium/Platforms/&lt;Device Vendor&gt;/&lt;Device Codename&gt;Pkg/Include/</code>.<br />
  Now we need the order of the binaries in <code className="bg-muted px-1 rounded text-sm font-mono">APRIORI.inc</code>. Use <code className="bg-muted px-1 rounded text-sm font-mono">UEFITool</code> to get the order.
</p>
<img src="./APRIORI1.png" alt="Apriori Step 1" width={900} />
<img src="./APRIORI2.png" alt="Apriori Step 2" width={900} />
<p className="text-sm text-muted-foreground mb-4">
  Next we place all the binaries in <code className="bg-muted px-1 rounded text-sm font-mono">APRIORI.inc</code> like this:
</p>
<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'APRIORI DXE {'}</code></p>
  <p><code>{'INF <Path to .inf File>'}</code></p>
  <p><code>{'INF <Path to .inf File>'}</code></p>
  <p><code>{'INF <Path to .inf File>'}</code></p>
  <p><code>{'.......'}</code></p>
  <p><code>{'}'}</code></p>
</div>
<p className="text-sm text-muted-foreground mb-4">
  Next we place all the binaries in <code className="bg-muted px-1 rounded text-sm font-mono">APRIORI.inc</code> like this:
</p>

<div className="bg-muted/50 p-4 rounded-lg font-mono text-sm space-y-1">
  <p>INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf</p>
  <p>INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf</p>
  <p>INF QcomPkg/Drivers/DynamicRAMDxe/DynamicRAMDxe.inf</p>
  <p>INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf</p>
  <p>INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf</p>
</div>

<p className="text-sm text-muted-foreground mt-4">
  <strong>Pcd</strong> should be placed under <code className="bg-muted px-1 rounded text-sm font-mono">DxeMain</code>.<br />
  <strong>ArmPsciMpServicesDxe</strong> should be under <code className="bg-muted px-1 rounded text-sm font-mono">TimerDxe</code>.<br />
  <strong>DynamicRAMDxe</strong> should be under <code className="bg-muted px-1 rounded text-sm font-mono">SmemDxe</code>.<br />
  <strong>ClockSpeedUpDxe</strong> should be under <code className="bg-muted px-1 rounded text-sm font-mono">ClockDxe</code>.<br />
  <strong>SimpleFbDxe</strong> doesn't replace <code className="bg-muted px-1 rounded text-sm font-mono">DisplayDxe</code>; make an <code className="bg-muted px-1 rounded text-sm font-mono">if</code> case for it — check other devices for reference.
</p>

<p className="text-sm text-muted-foreground mt-2">
  ⚠️ Do not add <code className="bg-muted px-1 rounded text-sm font-mono">FvSimpleFileSystemDxe</code>.
</p>

<p className="text-sm text-muted-foreground">
  Review other devices’ <code className="bg-muted px-1 rounded text-sm font-mono">APRIORI.inc</code> files to see what gets replaced with Mu drivers — and what doesn't.
</p>
      </div>
    </div>
  </CardContent>
  <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.2.3: Creating DXE.inc File
    </CardTitle>
    <CardDescription>
      DXE.inc is a text file used during the UEFI firmware build. It contains a list of all the DXE drivers and programs that should be included in the DXE phase of the firmware.
    </CardDescription>
    </CardHeader>
  <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
        <p className="text-sm text-muted-foreground mb-4">
  After that we can now move on to <code className="bg-muted px-1 rounded text-sm font-mono">DXE.inc</code>. Create <code className="bg-muted px-1 rounded text-sm font-mono">DXE.inc</code> in <code className="bg-muted px-1 rounded text-sm font-mono">Mu-Silicium/Platforms/&lt;Device Vendor&gt;/&lt;Device Codename&gt;Pkg/Include/</code>.<br />
  Now again we need the Order. To get the order of <code className="bg-muted px-1 rounded text-sm font-mono">DXE.inc</code>, open <code className="bg-muted px-1 rounded text-sm font-mono">xbl</code> or <code className="bg-muted px-1 rounded text-sm font-mono">uefi</code> in <code className="bg-muted px-1 rounded text-sm font-mono">UEFITool</code> and expand the FV(s), then you see the Order.
</p>
<img src="./DXE.png" alt="DXE" width={900} />

<p className="text-sm text-muted-foreground mb-4">
  Again we place all the Binaries like this:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'INF <Path to .inf>'}</code></p>
</div>

<p className="text-sm text-muted-foreground mb-4">
  Also here again you need to add some extra Stuff:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{'INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf'}</code></p>
  <p><code>{'INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf'}</code></p>
  <p><code>{'INF QcomPkg/Drivers/DynamicRAMDxe/DynamicRAMDxe.inf'}</code></p>
  <p><code>{'INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf'}</code></p>
  <p><code>{'INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf'}</code></p>
  <p><code>{'INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf'}</code></p>
</div>

<p className="text-sm text-muted-foreground mb-4">
  ArmPsciMpServicesDxe should be under TimerDxe.<br />
  Pcd should be under DxeMain.<br />
  DynamicRAMDxe should be under SmemDxe.<br />
  ClockSpeedUpDxe should be under ClockDxe.<br />
  SimpleFbDxe doesn't Replace DisplayDxe. Make an If case for it. Check other Devices for the if case.<br />
  UsbMouseAbsolutePointerDxe should be under UsbKbDxe.
</p>

<p className="text-sm text-muted-foreground mb-4">
  Remove any EFI Applications from XBL in DXE.inc.<br />
  Also again, Make sure that you don't add FvSimpleFileSystemDxe.
</p>

<p className="text-sm text-muted-foreground">
  Check other Devices DXE.inc File to get an Idea, What to replace with the Mu Driver and what not.
</p>
      </div>
    </div>
  </CardContent>
  <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.2.4: Creating RAW.inc File
    </CardTitle>
    <CardDescription>
      RAW.inc is a file that tells the UEFI build system to include certain raw binary blobs (like configs or images) directly into the firmware without changing them. Each file is added with a unique ID and kept exactly as it is.
    </CardDescription>
    </CardHeader>
  <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
        <p className="text-sm text-muted-foreground mb-4">
  You can take the RAW files order from <code className="bg-muted px-1 rounded text-sm font-mono">DXE.inc</code> that UEFIReader generated. That’s how they should look:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <pre>
{`FILE FREEFORM = <GUID> {
  SECTION RAW = Binaries/<Device Codename>/RawFiles/<File Name>.<File Extension>
  SECTION UI = "<Name>"
}`}
  </pre>
</div>

<p className="text-sm text-muted-foreground">
  Just note that UEFIReader doesn’t format the lines correctly. you need to fix that manually.
</p>

<p className="text-sm text-muted-foreground">
  Also, remove any <code className="bg-muted px-1 rounded text-sm font-mono">RAW</code> section that contains a picture.
</p>
      </div>
    </div>
  </CardContent>
   </CardContent>
  
   <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.3: Creating DeviceConfigurationMap Library
    </CardTitle>
    <CardDescription>
      So when you open uefiplat.cfg file, the all bottom strings in it are included in this library to tell the build system to implement these configurations too. This is just a Qualcomm shit and not in other implmentations. 
    </CardDescription>
   </CardHeader>
    <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
        
        <p className="text-sm text-muted-foreground mb-2"> Now, We move on to creating a Configuration Map for your Device.<br/> We need uefiplat.cfg from XBL to create This Map.<br/> Here is a Template for the <code className="bg-muted px-1 rounded text-sm font-mono">.c</code> File: </p> <div className="bg-background p-3 rounded border font-mono text-sm mb-4"> <p><code>{`#include <Library/DeviceConfigurationMapLib.h>`}</code></p> <br/> <p><code>{`STATIC`}</code></p> <p><code>{`CONFIGURATION_DESCRIPTOR_EX`}</code></p> <p><code>{`gDeviceConfigurationDescriptorEx[] = {`}</code></p> <p><code>{` // NOTE: All Conf are located before Terminator!`}</code></p> <br/> <p><code>{` // Terminator`}</code></p> <p><code>{` {"Terminator", 0xFFFFFFFF}`}</code></p> <p><code>{`};`}</code></p> <br/> <p><code>{`CONFIGURATION_DESCRIPTOR_EX*`}</code></p> <p><code>{`GetDeviceConfigurationMap ()`}</code></p> <p><code>{`{`}</code></p> <p><code>{` return gDeviceConfigurationDescriptorEx;`}</code></p> <p><code>{`}`}</code></p> </div> <p className="text-sm text-muted-foreground mb-2"> Place all Configs from <code className="bg-muted px-1 rounded text-sm font-mono">[ConfigParameters]</code> (uefiplat.cfg) In the <code className="bg-muted px-1 rounded text-sm font-mono">.c</code> File.<br/> Here is an Example: </p> <div className="bg-background p-3 rounded border font-mono text-sm mb-4"> <p><code>{`EnableShell = 0x1`}</code></p> </div> <p className="text-sm text-muted-foreground mb-2"> Becomes this: </p> <div className="bg-background p-3 rounded border font-mono text-sm mb-4"> <p><code>{`{"EnableShell", 0x1},`}</code></p> </div> <p className="text-sm text-muted-foreground mb-2"> Configs that have Strings instead of Decimal won't be added: </p> <div className="bg-background p-3 rounded border font-mono text-sm mb-4"> <p><code>{`# This for Example`}</code></p> <p><code>{`OsTypeString = "LA"`}</code></p> </div> <p className="text-sm text-muted-foreground mb-2"> And don't add <code className="bg-muted px-1 rounded text-sm font-mono">ConfigParameterCount</code> to the <code className="bg-muted px-1 rounded text-sm font-mono">.c</code> File either. </p> <p className="text-sm text-muted-foreground mb-2"> The INF can be copied from any other Device. </p>
        
        
        
      </div>
    </div>
  </CardContent>
  <CardHeader>
    <CardTitle className="flex items-center gap-2">
      <GitBranch className="h-5 w-5" />
      Step 3.4: Creating DeviceMemoryMap Library
    </CardTitle>
    <CardDescription>
      The memory map defines the physical memory layout, which specifies how much memory is available on the system and which parts of the physical memory are reserved for memory-mapped I/O or other special purposes. The kernel uses the memory map to setup its internal memory management data structures in regions of the memory that are safe to use.
    </CardDescription>
   </CardHeader>
    <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
<p className="text-sm text-muted-foreground mb-2">
  Lets move on making Memory Map.<br />
  We will use <code className="bg-muted px-1 rounded text-sm font-mono">uefiplat.cfg</code> to create the Memory Map.<br />
  Create a Folder Named <code className="bg-muted px-1 rounded text-sm font-mono">DeviceMemoryMapLib</code> in <code className="bg-muted px-1 rounded text-sm font-mono">Mu-Silicium/Platforms/&lt;Device Vendor&gt;/&lt;Device Codename&gt;Pkg/Library/</code>.<br />
  After that create two Files called <code className="bg-muted px-1 rounded text-sm font-mono">DeviceMemoryMapLib.c</code> and <code className="bg-muted px-1 rounded text-sm font-mono">DeviceMemoryMapLib.inf</code>.<br />
  You can either make the Memory Map by yourself or use an automated <a href="https://gist.github.com/N1kroks/0b3942a951a2d4504efe82ab82bc7a50" className="underline">Script</a> if your SoC is older than Snapdragon 8 Gen 3 (SM8650).
</p>
        <Alert className="mb-8 border-amber-200 bg-amber-50 dark:border-amber-900 dark:bg-amber-950">
        <AlertTriangle className="h-4 w-4 text-amber-600" />
        <AlertDescription className="text-amber-800 dark:text-amber-200">
          <strong>Note:</strong>        script also create Configuration Map, remove it from Memory Map
        </AlertDescription>
      </Alert>
      <p className="text-sm text-muted-foreground mb-2">
  Let's move on to making the Memory Map.
</p>
<p className="text-sm text-muted-foreground mb-2">
  We will use <code className="bg-muted px-1 rounded text-sm font-mono">uefiplat.cfg</code> to create the Memory Map.
</p>
<p className="text-sm text-muted-foreground mb-2">
  Create a folder named <code className="bg-muted px-1 rounded text-sm font-mono">DeviceMemoryMapLib</code> in 
  <code className="bg-muted px-1 rounded text-sm font-mono"> Mu-Silicium/Platforms/&lt;Device Vendor&gt;/&lt;Device Codename&gt;Pkg/Library/</code>.
</p>
<p className="text-sm text-muted-foreground mb-2">
  After that, create two files called <code className="bg-muted px-1 rounded text-sm font-mono">DeviceMemoryMapLib.c</code> and 
  <code className="bg-muted px-1 rounded text-sm font-mono">DeviceMemoryMapLib.inf</code>.
</p>
<p className="text-sm text-muted-foreground mb-2">
  You can either make the Memory Map yourself or use an automated script if your SoC is older than 
  <code className="bg-muted px-1 rounded text-sm font-mono">Snapdragon 8 Gen 3 (SM8650)</code>.
</p>

<p className="text-sm text-muted-foreground mb-2">
  If you want to make the Memory Map by yourself, here is a template for the 
  <code className="bg-muted px-1 rounded text-sm font-mono">.c</code> file:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <pre><code>{`#include <Library/DeviceMemoryMapLib.h>

STATIC
ARM_MEMORY_REGION_DESCRIPTOR_EX
gDeviceMemoryDescriptorEx[] = {
  // Name, Address, Length, HobOption, ResourceAttribute, ArmAttributes, ResourceType, MemoryType

  // DDR Regions

  // Other memory regions

  // Register regions

  // Terminator for MMU
  {"Terminator", 0, 0, 0, 0, 0, 0, 0}
};

ARM_MEMORY_REGION_DESCRIPTOR_EX*
GetDeviceMemoryMap ()
{
  return gDeviceMemoryDescriptorEx;
}`}</code></pre>
</div>

<p className="text-sm text-muted-foreground mb-2">
  Place all DDR memory regions under <code className="bg-muted px-1 rounded text-sm font-mono">DDR Regions</code> in 
  <code className="bg-muted px-1 rounded text-sm font-mono">DeviceMemoryMapLib.c</code>. Example:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <pre><code>{`0xEA600000, 0x02400000, "Display Reserved",  AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN`}</code></pre>
</div>

<p className="text-sm text-muted-foreground mb-2">
  would become in the Memory Map:
</p>

<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <pre><code>{`{"Display Reserved",  0xEA600000, 0x02400000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN},`}</code></pre>
</div>

<p className="text-sm text-muted-foreground mb-2">
  Do that with every memory region, but if there's a <code className="bg-muted px-1 rounded text-sm font-mono">#</code> in front of a memory region, 
  do not add it.
</p>

<p className="text-sm text-muted-foreground mb-2">
  After that, it should look something like this.
</p>

<p className="text-sm text-muted-foreground mb-2">
  The <code className="bg-muted px-1 rounded text-sm font-mono">.inf</code> file can be copied from any other device.
</p>

      </div>
    </div>
  </CardContent>
  </Card>
  {/* Step 4: Boot script section */}
   <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Smartphone className="h-5 w-5" />
              Step 4: Creating Android Boot Image Script
            </CardTitle>
            <CardDescription>
              In this section we create a boot script so that you can disguise your UEFI image as your boot.img 
            </CardDescription>
          </CardHeader>
         <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
        <p className="text-sm text-muted-foreground mb-2"> You also need to create a script that creates the boot image. </p> <p className="text-sm text-muted-foreground mb-2"> You can copy a device with a similar/same boot image creation script from <code className="bg-muted px-1 rounded text-sm font-mono">Mu-Silicium/Resources/Scripts/&lt;Device Codename&gt;.sh</code> and just replace the code name with yours. </p> <p className="text-sm text-muted-foreground mb-2"> If there is no device with a similar boot image creation script, extract the original Android boot image using <code className="bg-muted px-1 rounded text-sm font-mono">AIK (Android Image Kitchen)</code>. </p> <p className="text-sm text-muted-foreground mb-2"> Then, you just use the info that the tool gives you and put them into the script. </p>
        
        <p className="text-sm text-muted-foreground mb-2"> you can also use this template: </p>
        
        <div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{`#!/bin/bash`}</code></p>
  <p><code>{``}</code></p>
  <p><code>{`# Build an Android kernel that is actually UEFI disguised as the Kernel`}</code></p>
  <p><code>{`cat ./BootShim/BootShim.bin "./Build/<device_codename>Pkg/\${TARGET_BUILD_MODE}_CLANGPDB/FV/<device_codename_in_CAPiTAL>_UEFI.fd" > "./Build/<device_codename>Pkg/\${TARGET_BUILD_MODE}_CLANGPDB/FV/<device_codename_in_CAPiTAL>_UEFI.fd-bootshim"||exit 1`}</code></p>
  <p><code>{`gzip -c < "./Build/<device_codename>Pkg/\${TARGET_BUILD_MODE}_CLANGPDB/FV/<device_codename_in_CAPiTAL>_UEFI.fd-bootshim" > "./Build/device_codename>Pkg/\${TARGET_BUILD_MODE}_CLANGPDB/FV/<device_codename_in_CAPiTAL>_UEFI.fd-bootshim.gz"||exit 1`}</code></p>
  <p><code>{`cat "./Build/<device_codename>Pkg/\${TARGET_BUILD_MODE}_CLANGPDB/FV/A52Q_UEFI.fd-bootshim.gz" ./Resources/DTBs/<device_codename>.dtb > ./Resources/bootpayload.bin||exit 1`}</code></p>
  <p><code>{``}</code></p>
  <p><code>{`# Create bootable Android boot.img`}</code></p>
  <p><code>{`python3 ./Resources/Scripts/mkbootimg.py \\`}</code></p>
  <p><code>{`  --kernel ./Resources/bootpayload.bin \\`}</code></p>
  <p><code>{`  --ramdisk ./Resources/ramdisk \\`}</code></p>
  <p><code>{`  --kernel_offset 0x00000000 \\`}</code></p>
  <p><code>{`  --ramdisk_offset 0x00000000 \\`}</code></p>
  <p><code>{`  --tags_offset 0x00000000 \\`}</code></p>
  <p><code>{`  --os_version 13.0.0 \\`}</code></p>
  <p><code>{`  --os_patch_level "\$(date '+%Y-%m')" \\`}</code></p>
  <p><code>{`  --header_version 1 \\`}</code></p>
  <p><code>{`  -o boot.img \\`}</code></p>
  <p><code>{`  ||_error "\\nFailed to create Android Boot Image!\\n"`}</code></p>
  <p><code>{``}</code></p>
  <p><code>{`# Compress Boot Image in a tar File for Odin/heimdall Flash if you are building for a Samsung device`}</code></p>
  <p><code>{`remove "#" from bellow lines`}</code></p>
  <p><code>{`# tar -c boot.img -f Mu-<device_codename>.tar||exit 1`}</code></p>
  <p><code>{`# mv boot.img Mu-<device_codename>.img||exit 1`}</code></p>
</div>

<p className="text-sm text-muted-foreground mb-2"> Replace <code className="bg-muted px-1 rounded text-sm font-mono">&lt;device_Codename&gt;</code> with your device codename. Remember to not add &lt; and &gt; when you replace with your device codename </p> <p className="text-sm text-muted-foreground mb-2"> Replace <code className="bg-muted px-1 rounded text-sm font-mono">&lt;device_codename_in_CAPiTAL&gt;</code>with your device codename but capitalised example A52Q instead of a52q </p>
      </div>
    </div>
  </CardContent>
 
        </Card>

  
        {/* Step 5: Building */}
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Smartphone className="h-5 w-5" />
              Step 5: Create DeviceBuild.py
            </CardTitle>
            <CardDescription>
              As it is named, this is a file which defines all parameters which will be used during building.
            </CardDescription>
          </CardHeader>
          <CardContent className="space-y-6">
    <div className="space-y-4">
      <div className="bg-muted/50 p-4 rounded-lg">
	<p className="text-sm text-muted-foreground mb-2">As DeviceBuild.py can be a little device specific, mostly, it isnt. So you can follow this template:</p>
	<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  <p><code>{`##`}</code></p>
  <p><code>{`# Copyright (c) Microsoft Corporation.`}</code></p>
  <p><code>{`# SPDX-License-Identifier: BSD-2-Clause-Patent`}</code></p>
  <p><code>{`##`}</code></p>
 <p><code>{'\u00A0'}</code></p>
  <p><code>{`import datetime`}</code></p>
  <p><code>{`import logging`}</code></p>
  <p><code>{`import os`}</code></p>
  <p><code>{`import uuid`}</code></p>
 <p><code>{'\u00A0'}</code></p>
  <p><code>{`from io import StringIO`}</code></p>
  <p><code>{`from pathlib import Path`}</code></p>
  <p><code>{'\u00A0'}</code></p>
  <p><code>{`from edk2toolext.environment import shell_environment`}</code></p>
  <p><code>{`from edk2toolext.environment.uefi_build import UefiBuilder`}</code></p>
  <p><code>{`from edk2toolext.invocables.edk2_platform_build import BuildSettingsManager`}</code></p>
  <p><code>{`from edk2toolext.invocables.edk2_pr_eval import PrEvalSettingsManager`}</code></p>
  <p><code>{`from edk2toolext.invocables.edk2_setup import (RequiredSubmodule, SetupSettingsManager)`}</code></p>
  <p><code>{`from edk2toolext.invocables.edk2_update import UpdateSettingsManager`}</code></p>
  <p><code>{`from edk2toolext.invocables.edk2_parse import ParseSettingsManager`}</code></p>
  <p><code>{`from edk2toollib.utility_functions import RunCmd`}</code></p>
   <p><code>{'\u00A0'}</code></p>
  <p><code>{`# ####################################################################################### #`}</code></p>
  <p><code>{`#                                Common Configuration                                     #`}</code></p>
  <p><code>{`# ####################################################################################### #`}</code></p>
   <p><code>{'\u00A0'}</code></p>
  <p><code>{`class CommonPlatform ():`}</code></p>
  <p><code>{`    PackagesSupported = ("<device_codename>Pkg")`}</code></p>
  <p><code>{`    ArchSupported = ("AARCH64")`}</code></p>
  <p><code>{`    TargetsSupported = ("DEBUG", "RELEASE")`}</code></p>
  <p><code>{`    Scopes = ('a52q', 'gcc_aarch64_linux', 'edk2-build')`}</code></p>
  <p><code>{`    WorkspaceRoot = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))`}</code></p>
  <p><code>{`    PackagesPath = (`}</code></p>
  <p><code>{`        "Platforms/<device_vendor>",`}</code></p>
  <p><code>{`        "Common/Mu",`}</code></p>
  <p><code>{`        "Common/Mu_OEM_Sample",`}</code></p>
  <p><code>{`        "Common/Mu_Tiano_Plus",`}</code></p>
  <p><code>{`        "Features/DFCI",`}</code></p>
  <p><code>{`        "Mu_Basecore",`}</code></p>
  <p><code>{`        "Silicon/Arm/Mu_Tiano",`}</code></p>
  <p><code>{`        "Silicon/Qualcomm",`}</code></p>
  <p><code>{`        "Silicon/Silicium",`}</code></p>
  <p><code>{`        "Silicium-ACPI/Platforms/<device_vendor>",`}</code></p>
  <p><code>{`        "Silicium-ACPI/SoCs/Qualcomm"`}</code></p>
  <p><code>{`    )`}</code></p>
 <p><code>{'\u00A0'}</code></p>
  <p><code>{`# ####################################################################################### #`}</code></p>
<p><code>{`#                         Configuration for Update & Setup                                #`}</code></p>
<p><code>{`# ####################################################################################### #`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`class SettingsManager (UpdateSettingsManager, SetupSettingsManager, PrEvalSettingsManager, ParseSettingsManager):`}</code></p>
<p><code>{`    def GetPackagesSupported (self):`}</code></p>
<p><code>{`        return CommonPlatform.PackagesSupported`}</code></p>
<p><code>{`    def GetArchitecturesSupported (self):`}</code></p>
<p><code>{`        return CommonPlatform.ArchSupported`}</code></p>
<p><code>{`    def GetTargetsSupported (self):`}</code></p>
<p><code>{`        return CommonPlatform.TargetsSupported`}</code></p>
<p><code>{`    def GetRequiredSubmodules (self):`}</code></p>
<p><code>{`        return [`}</code></p>
<p><code>{`            RequiredSubmodule ("Binaries", True),`}</code></p>
<p><code>{`            RequiredSubmodule ("Common/Mu", True),`}</code></p>
<p><code>{`            RequiredSubmodule ("Common/Mu_OEM_Sample", True),`}</code></p>
<p><code>{`            RequiredSubmodule ("Common/Mu_Tiano_Plus", True),`}</code></p>
<p><code>{`            RequiredSubmodule ("Features/DFCI", True),`}</code></p>
<p><code>{`            RequiredSubmodule ("Mu_Basecore", True),`}</code></p>
<p><code>{`            RequiredSubmodule ("Silicon/Arm/Mu_Tiano", True),`}</code></p>
<p><code>{`            RequiredSubmodule ("Silicium-ACPI", True),`}</code></p>
<p><code>{`        ]`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def SetArchitectures (self, list_of_requested_architectures):`}</code></p>
<p><code>{`        unsupported = set(list_of_requested_architectures) - set(self.GetArchitecturesSupported())`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`        if (len(unsupported) > 0):`}</code></p>
<p><code>{`            errorString = ("Unsupported Architecture Requested: " + " ".join(unsupported))`}</code></p>
<p><code>{`            logging.critical (errorString)`}</code></p>
<p><code>{`            raise Exception (errorString)`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`        self.ActualArchitectures = list_of_requested_architectures`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetWorkspaceRoot (self):`}</code></p>
<p><code>{`        return CommonPlatform.WorkspaceRoot`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetActiveScopes (self):`}</code></p>
<p><code>{`        return CommonPlatform.Scopes`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`    def FilterPackagesToTest (self, changedFilesList: list, potentialPackagesList: list) -> list:`}</code></p>
<p><code>{`        build_these_packages = []`}</code></p>
<p><code>{`        possible_packages = potentialPackagesList.copy ()`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`        for f in changedFilesList:`}</code></p>
<p><code>{`            if "BaseTools" in f:`}</code></p>
<p><code>{`                if os.path.splitext(f) not in [".txt", ".md"]:`}</code></p>
<p><code>{`                    build_these_packages = possible_packages`}</code></p>
<p><code>{`                    break`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`            if "platform-build-run-steps.yml" in f:`}</code></p>
<p><code>{`                build_these_packages = possible_packages`}</code></p>
<p><code>{`                break`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`        return build_these_packages`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetPlatformDscAndConfig (self) -> tuple:`}</code></p>
<p><code>{`        return ("<device_codename>Pkg/<device_codename>.dsc", {})`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetName (self):`}</code></p>
<p><code>{`        return "<device_codename>"`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetPackagesPath (self):`}</code></p>
<p><code>{`        return CommonPlatform.PackagesPath`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`# ####################################################################################### #`}</code></p>
<p><code>{`#                         Actual Configuration for Platform Build                         #`}</code></p>
<p><code>{`# ####################################################################################### #`}</code></p>
 <p><code>{'\u00A0'}</code></p>
<p><code>{`class PlatformBuilder (UefiBuilder, BuildSettingsManager):`}</code></p>
<p><code>{`    def __init__ (self):`}</code></p>
<p><code>{`        UefiBuilder.__init__ (self)`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def AddCommandLineOptions (self, parserObj):`}</code></p>
<p><code>{`        parserObj.add_argument('-a', "--arch", dest="build_arch", type=str, default="AARCH64", help="Optional - CSV of architecture to build. AARCH64 is used for PEI and DXE and is the only valid option for this platform.")`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def RetrieveCommandLineOptions (self, args):`}</code></p>
<p><code>{`        if args.build_arch.upper() != "AARCH64":`}</code></p>
<p><code>{`            raise Exception("Invalid Arch Specified.  Please see comments in DeviceBuild.py::PlatformBuilder::AddCommandLineOptions")`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetWorkspaceRoot (self):`}</code></p>
<p><code>{`        return CommonPlatform.WorkspaceRoot`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetPackagesPath (self):`}</code></p>
<p><code>{`        result = [ shell_environment.GetBuildVars().GetValue("FEATURE_CONFIG_PATH", "") ]`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`        for a in CommonPlatform.PackagesPath:`}</code></p>
<p><code>{`            result.append(a)`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`        return result`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetActiveScopes (self):`}</code></p>
<p><code>{`        return CommonPlatform.Scopes`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetName (self):`}</code></p>
<p><code>{`        return "<device_codename>Pkg"`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def GetLoggingLevel (self, loggerType):`}</code></p>
<p><code>{`        return logging.INFO`}</code></p>
<p><code>{`        return super().GetLoggingLevel(loggerType)`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def SetPlatformEnv (self):`}</code></p>
<p><code>{`        logging.debug ("PlatformBuilder SetPlatformEnv")`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`        self.env.SetValue ("PRODUCT_NAME", "<device_codename>", "Platform Hardcoded")`}</code></p>
<p><code>{`        self.env.SetValue ("ACTIVE_PLATFORM", "<device_codename>Pkg/<device_codename>.dsc", "Platform Hardcoded")`}</code></p>
<p><code>{`        self.env.SetValue ("TARGET_ARCH", "AARCH64", "Platform Hardcoded")`}</code></p>
<p><code>{`        self.env.SetValue ("TOOL_CHAIN_TAG", "CLANGPDB", "set default to clangpdb")`}</code></p>
<p><code>{`        self.env.SetValue ("EMPTY_DRIVE", "FALSE", "Default to false")`}</code></p>
<p><code>{`        self.env.SetValue ("RUN_TESTS", "FALSE", "Default to false")`}</code></p>
<p><code>{`        self.env.SetValue ("SHUTDOWN_AFTER_RUN", "FALSE", "Default to false")`}</code></p>
<p><code>{`        self.env.SetValue ("BLD_*_BUILDID_STRING", "Unknown", "Default")`}</code></p>
<p><code>{`        self.env.SetValue ("BUILDREPORTING", "TRUE", "Enabling build report")`}</code></p>
<p><code>{`        self.env.SetValue ("BUILDREPORT_TYPES", "PCD DEPEX FLASH BUILD_FLAGS LIBRARY FIXED_ADDRESS HASH", "Setting build report types")`}</code></p>
<p><code>{`        self.env.SetValue ("BLD_*_MEMORY_PROTECTION", "TRUE", "Default")`}</code></p>
<p><code>{`        self.env.SetValue ("BLD_*_SHIP_MODE", "FALSE", "Default")`}</code></p>
<p><code>{`        self.env.SetValue ("BLD_*_DEVICE_MODEL", self.env.GetValue("DEVICE_MODEL"), "Default")`}</code></p>
<p><code>{`        self.env.SetValue ("BLD_*_FD_BASE", self.env.GetValue("FD_BASE"), "Default")`}</code></p>
<p><code>{`        self.env.SetValue ("BLD_*_FD_SIZE", self.env.GetValue("FD_SIZE"), "Default")`}</code></p>
<p><code>{`        self.env.SetValue ("BLD_*_FD_BLOCKS", self.env.GetValue("FD_BLOCKS"), "Default")`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`        return 0`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def PlatformPreBuild (self):`}</code></p>
<p><code>{`        return 0`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def PlatformPostBuild (self):`}</code></p>
<p><code>{`        return 0`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    def FlashRomImage (self):`}</code></p>
<p><code>{`        return 0`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`if __name__ == "__main__":`}</code></p>
<p><code>{`    import argparse`}</code></p>
<p><code>{`    import sys`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    from edk2toolext.invocables.edk2_platform_build import Edk2PlatformBuild`}</code></p>
<p><code>{`    from edk2toolext.invocables.edk2_setup import Edk2PlatformSetup`}</code></p>
<p><code>{`    from edk2toolext.invocables.edk2_update import Edk2Update`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    SCRIPT_PATH = os.path.relpath (__file__)`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    parser = argparse.ArgumentParser (add_help=False)`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    parse_group = parser.add_mutually_exclusive_group()`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    parse_group.add_argument ("--update", "--UPDATE", action='store_true', help="Invokes stuart_update")`}</code></p>
<p><code>{`    parse_group.add_argument ("--setup",  "--SETUP",  action='store_true', help="Invokes stuart_setup")`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    args, remaining = parser.parse_known_args()`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    new_args = ["stuart", "-c", SCRIPT_PATH]`}</code></p>
<p><code>{`    new_args = new_args + remaining`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    sys.argv = new_args`}</code></p>
<p><code>{'\u00A0'}</code></p>
<p><code>{`    if args.setup:`}</code></p>
<p><code>{`        Edk2PlatformSetup().Invoke()`}</code></p>
<p><code>{`    elif args.update:`}</code></p>
<p><code>{`        Edk2Update().Invoke()`}</code></p>
<p><code>{`    else:`}</code></p>
<p><code>{`        Edk2PlatformBuild().Invoke()`}</code></p>
</div>
<p className="text-sm text-muted-foreground mb-2"> Replace <code className="bg-muted px-1 rounded text-sm font-mono">&lt;device_Codename&gt;</code> with your device codename. Remember to not add &lt; and &gt; when you replace with your device codename </p> <p className="text-sm text-muted-foreground mb-2"> Replace <code className="bg-muted px-1 rounded text-sm font-mono">&lt;device_vendor&gt;</code>with your device manufacturer. for example Samsung </p>
      </div>
    </div>
  </CardContent>

        </Card>

        {/*Building */}
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <ExternalLink className="h-5 w-5" />
              Step 6: Building
            </CardTitle>
            <CardDescription>
              The last but not the least, building your UEFI image on your PC
            </CardDescription>
          </CardHeader>
          <CardContent>
          <Alert className="mb-8 border-amber-200 bg-amber-50 dark:border-amber-900 dark:bg-amber-950">
        <AlertTriangle className="h-4 w-4 text-amber-600" />
        <AlertDescription className="text-amber-800 dark:text-amber-200">
          <strong>Important:</strong> It's recommended to use Ubuntu 22.04.3 LTS to Compile this. You can install Ubuntu by either VM, WSL or dualboot.
        </AlertDescription>
      </Alert>
      <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <ExternalLink className="h-5 w-5" />
              Preparing
            </CardTitle>
          </CardHeader>
          <p className="text-sm text-muted-foreground mb-2">
          Before you can even Begin to Compile a UEFI, You must clone the GitHub Repo:
	</p>
	<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  		<p><code>{`# Install the "git" Package if the Command fails with "Not Found".`}</code></p>
		<p><code>{`git clone https://github.com/Project-Silicium/Mu-Silicium.git --recursive`}</code></p>
		<p><code>{`cd Mu-Silicium`}</code></p>
		 </div>
		 
		 <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <ExternalLink className="h-5 w-5" />
              Compiling
            </CardTitle>
          </CardHeader>
          <p className="text-sm text-muted-foreground mb-2">
          Once you Cloned the GitHub Repo, You need to install the needed Packages for the Compile Process:
	</p>
		<div className="bg-background p-3 rounded border font-mono text-sm mb-4">
  		<p><code>{`# Example: For Xiaomi 11T Pro, Use "vili". vili is a devcie codename`}</code></p>
		<p><code>{`# Hint: If you want Debug Logs on your Phone, Add "-r DEBUG" to the Params`}</code></p>
		<p><code>{`./build_uefi.sh -d [Codename]`}</code></p>
		 </div>
		  <p className="text-sm text-muted-foreground mb-2">
          It will tell you at the End if the Build was Successfull or not.
	If it was Successfull, a Output Image will apear with this Name: <code className="bg-muted px-1 rounded text-sm font-mono">Mu-[Device-Codename].</code>
	Just flash it in Fastboot mode and have fun!. But wait... if you see any balck screen, weird crashes, stuffs like that, you will need to patch certain drivers based on your phone.
	if you have some concerns or anything, just head over to the discord page and one of all devs will eventually respond you. 
	</p>
	<p className="text-sm text-muted-foreground mb-2">
	And dont be sure that you are done now. as i told you before, you will have crashes for anything. This is common and solution is that you have to find what driver is crashing and then reverse engineering it in Ghidra or IDA PRO and then patch it then replace the default driver line with the one you patched in DXE.inc and APRIORI.inc files. and then build it again.
	</p>
	<p className="text-sm text-muted-foreground mb-2">
	And one last thing to say, if you encounter some Build errors, no problem. This means that you havent ported correctly and the issue can be either some "Typo" mistake or some missing lines etc. Its just a matter of time and some fixing things and you will have with your Built image soon.
	</p>
          </CardContent>
        </Card>
      </div>
    </div>
  );
};

export default SnapdragonPortingGuide;
