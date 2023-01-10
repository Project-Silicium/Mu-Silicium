# [Project Mu](https://microsoft.github.io/mu/) UEFI Implementation for Qualcomm Snapdragon Devices

## Note

I recomend to not use this on your main Phone, Many Devices here are unstable

## Resources

[Discord Server](https://discord.gg/Dx2QgMx7Sv)

## Status
|✅|Working|
|--|--------|
|⚠️|Problematic|
|❌|Not Working|
|❔|Unknown|

|Devices|
|-------|
|[Xiaomi 11T Pro](https://github.com/Robotix22/MU-msm/blob/main/Status/Xiaomi-11T-Pro.md)|
|[Mi Note 10](https://github.com/Robotix22/MU-msm/blob/main/Status/Xiaomi-Mi-Note-10.md)|
|[Redmi Note 8/8T](https://github.com/Robotix22/MU-msm/blob/main/Status/Xiaomi-Redmi-Note-8.md)|
|[Redmi 9T](https://github.com/Robotix22/MU-msm/blob/main/Status/Xiaomi-Redmi-9T.md)|
|[Xperia XZ1 Compact](https://github.com/Robotix22/MU-msm/blob/main/Status/Sony-Xperia-XZ1-Compact.md)|

## Building

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
sudo pip install --upgrade -r pip-requirements.txt
```

### Step 3:

So now we are able to begin the real UEFI build:
```
./build_uefi.sh -d [Codename]
```

When then Build is done you will find a `.img` File in the root of the repo.

## Credits

This repo is based on [SurfaceDuoPkg](https://github.com/WOA-Project/SurfaceDuoPkg)

## License

All code except drivers in `GPLDriver` directory are licensed under BSD 2-Clause.
GPL Drivers are licensed under GPLv2 license.
