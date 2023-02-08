# [Project Mu](https://microsoft.github.io/mu/) UEFI Implementation for Qualcomm Snapdragon Devices

## Note

I recomend to not use this on your main Phone, Many Devices here are unstable

## Resources

[Discord Server](https://discord.gg/Dx2QgMx7Sv)

## Status

|✅|Working|
|--|--------|
|⚠️|**Problematic**|
|❌|**Not Working**|
|❔|**Unknown**|

|Device|Maintainer|Stable|
|:-----|:--------:|:----:|
|[Xiaomi 11T Pro](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-11T-Pro.md)|[Robotix22](https://github.com/Robotix22)|⚠️|
|[Poco F1](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Poco-F1.md)|[Robotix22](https://github.com/Robotix22)|⚠️|
|[Xperia XZ1 Compact](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Sony-Xperia-XZ1-Compact.md)|[Robotix22](https://github.com/Robotix22)|❔|
|[Mi Note 10](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Mi-Note-10.md)|[Robotix22](https://github.com/Robotix22)|❔|
|[Mi A3](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Mi-A3.md)|[Kernel357](https://github.com/Kernel357)|⚠️|
|[Redmi Note 8/8T](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Redmi-Note-8.md)|[Robotix22](https://github.com/Robotix22)|⚠️|
|[Redmi 9T](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Redmi-9T.md)|[Robotix22](https://github.com/Robotix22)|⚠️|

## Building

## Debian / Ubuntu
### Step 1:

First of you need the sourcecode to compile a UEFI Image. <br />
Clone the Repo by using:
```
git clone https://github.com/Robotix22/MU-Qcom.git --recursive
cd MU-Qcom
```

### Step 2:

After Cloning the repo we can now continue on Setting up the Environment. <br />
First we need to install the needed Packages:
```
./setup_env.sh
sudo pip install --upgrade -r pip-requirements.txt
```

### Step 3:

So now we are able to begin the real UEFI build:
```
./build_uefi.sh -d <Codename> [-r <Build Mode>] [-m <Mem Size>] [-p <Panel Type>]
```

When then Build is done you will find a `.img` File in the root of the repo.

### Fix build errors:

#### Fix error with RegularExpressionDxe:
   1. During the UEFI build, problems may occur due to the Regular Expression library. There is a workaround for this problem.
   2. You need to comment out this line:
      ```
      MdeModulePkg/Universal/RegularExpressionDxe/RegularExpressionDxe.inf
      ```
      in the file:
      ```
      ../MU-Qcom/Platforms/QcomPkg/Qcom.dsc.inc 
      ```
      by adding # in front of the line.
   3. You also need to comment out this line:
      ```
      INF MdeModulePkg/Universal/RegularExpressionDxe/RegularExpressionDxe.inf 
      ```
      in the file:
      ```
      ../MU-Qcom/Platforms/<Your platform>Pkg/<Your platform>.fdf
      ```

## Credits

This repo is based on [SurfaceDuoPkg](https://github.com/WOA-Project/SurfaceDuoPkg)

## License

All code except drivers in `GPLDriver` directory are licensed under BSD 2-Clause.
GPL Drivers are licensed under GPLv2 license.
