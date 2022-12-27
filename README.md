# [Project Mu](https://microsoft.github.io/mu/) UEFI Implementation for Qualcomm Snapdragon Devices

## Resources

[Discord Server](https://discord.gg/Gb4KAqAQdm)

## [Status](https://github.com/Robotix22/MU-msm/blob/main/Status.md)

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
./build_uefi.sh -s <SOC> -d <Device>-r <Build Mode>
```

After that You will find `MU-<Device>.img` on the root of this repo.

## Credits

This repo is based on [SurfaceDuoPkg](https://github.com/WOA-Project/SurfaceDuoPkg)

## License

All code except drivers in `GPLDriver` directory are licensed under BSD 2-Clause.
GPL Drivers are licensed under GPLv2 license.
