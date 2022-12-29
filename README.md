# [Project Mu](https://microsoft.github.io/mu/) UEFI Implementation for Qualcomm Snapdragon Devices

## Resources

[Discord Server](https://discord.gg/Dx2QgMx7Sv)

## Status
|✅|Working|
|--|--------|
|⚠️|Problematic|
|❌|Not Working|
|❔|Unknown|

## UEFI Status

|Device|SOC|Display|Clocks|UFS/eMMC|Buttons|USB|SD Card|ACPI|
|------|---|-------|------|--------|-------|---|-------|----|
|Xiaomi 11T Pro|SM8350|✅|✅|✅|✅|⚠️|❌|❌|
|Mi Note 10|SM7150|❔|❔|❔|❔|❔|❔|❔|
|Redmi Note 8/8T|SM6125|✅|❌|❌|✅|❌|❌|❌|
|Redmi 9T|SM6115|✅|❌|❌|✅|❌|❌|❌|
|Sony Xperia XZ1 Compact|MSM8998|❔|❔|❔|❔|❔|❔|❔|

## Building
(NOTE: you NEED to use Ubuntu 20.04 to Build!)

### 1. Clone the Repo

```
git clone https://github.com/Robotix22/MU-msm.git --recursive
cd MU-msm
```

### 2. Setup Base environment
```
./setup_env.sh
pip install --upgrade -r pip-requirements.txt
```

### 3. Stamp the build
```
./build_releaseinfo.ps1
```

### 4. Build the UEFI
```
./build_uefi.sh -s <SOC> -d <Device> -b <Build Mode>
```

After that You will find `MU-<Device>.img` on the root of this repo.

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
