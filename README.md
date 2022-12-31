# [Project Mu](https://microsoft.github.io/mu/) UEFI Implementation for Qualcomm Snapdragon Devices

## Resources

[Discord Server](https://discord.gg/Dx2QgMx7Sv)

## Status
|✅|Working|
|--|--------|
|⚠️|Problematic|
|❌|Not Working|
|❔|Unknown|

### UEFI Status

|Device|SOC|Display|Clocks|UFS/eMMC|Buttons|USB|SD Card|ACPI|
|------|---|-------|------|--------|-------|---|-------|----|
|Xiaomi 11T Pro|SM8350|✅|✅|✅|✅|⚠️|❌|❌|
|Mi Note 10|SM7150|❔|❔|❔|❔|❔|❔|❔|
|Redmi Note 8/8T|SM6125|✅|❌|❌|✅|❌|❌|❌|
|Redmi 9T|SM6115|✅|❌|❌|✅|❌|❌|❌|
|Xperia XZ1 Compact|MSM8998|❔|❔|❔|❔|❔|❔|❔|

## Building
(NOTE: you NEED to use Ubuntu 20.04 to Build!)

### Step 1:

First of you need the sourcecode to compile a UEFI Image. <br />
Clone the Repo by using:
```
git clone https://github.com/Robotix22/MU-msm.git --recursive
cd MU-msm
```

### Step 2:

After Cloning the repo we can now continue on Setting up the Environment. <br />
First we need to install the needed Packages:
```
./setup_env.sh
```

Now We Stamp the Build:
```
./build_releaseinfo.ps1
```

### Step 3:

So now we are able to begin the real UEFI build:
```
./build_uefi.sh -s [SOC] -d [Device] -r [Build Mode]
```

SOC: The SOC of your Device you want to build. <br />
Device: The Codename of your Device. <br />
Build Mode: The Mode you want to use to build the UEFI.

When then Build is done you will find a `.img` File in the root of the repo.

## Booting

### PC/Laptop:

To boot a UEFI Image with PC/Laptop you need ADB and Fastboot. <br />
if you haven´t set up ADB/Fastboot yet use [this](https://wiki.lineageos.org/adb_fastboot_guide). <br />
After Setting up ADB/Fastboot you can now boot your Phone to fastboot with keycombo or ADB:
```
(Make sure to enable USB Debbugin in Developer Settings)
adb reboot bootloader
```

Now download or compile a UEFI Image and use Fastboot to boot it:
```
fastboot boot [Path to UEFI Image]
```
or flash it:
```
(NOTE: If you still want to boot Android then make a backup of boot before flashing the UEFI Image)
fastboot flash boot [Path to UEFI Image]
```

Now your Phone should boot UEFI.

### Phone:

To Flash a UEFI Image you need a custom recovery like [TWRP](https://twrp.me/). <br />
Download a UEFI Image on your Phone and save it somewhere you can find it again. <br />

(NOTE: If you still want to boot Android then make a backup of boot before flashing the UEFI Image)

Now boot into TWRP, When in TWRP Chosse Install -> Install Image -> [Path to UEFI Image] -> Boot <br />

After that Reboot the Device and it should boot the UEFI Image

## Credits

This repo is based on [SurfaceDuoPkg](https://github.com/WOA-Project/SurfaceDuoPkg)

Thanks to these People for Testing on Devices I don´t own.

[SwedMlite](https://github.com/SwedMlite): Redmi Note 8 <br />
[VAVANESKA](https://github.com/VAVANESKA): Redmi Note 8 <br />
[Vicente Cortes](https://github.com/vicenteicc2008): Redmi Note 8 <br />
[kubawis128](https://github.com/kubawis128): Redmi Note 8T <br />
[Xhelowrk](https://github.com/Xhelowrk): Redmi 9T

## License

All code except drivers in `GPLDriver` directory are licensed under BSD 2-Clause.
GPL Drivers are licensed under GPLv2 license.
