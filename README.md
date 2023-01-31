# [Project Mu](https://microsoft.github.io/mu/) UEFI Implementation for Qualcomm Snapdragon Devices

## Note

I recomend to not use this on your main Phone, Many Devices here are unstable

## Resources

[Discord Server](https://discord.gg/Dx2QgMx7Sv)

## Status
<table>
<tr><th>Devices</th><th>Description</th></tr>
<tr><td>

|[Xiaomi 11T Pro](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-11T-Pro.md)|
|:-----:|
|**[Mi Note 10](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Mi-Note-10.md)**|
|**[Redmi Note 8/8T](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Redmi-Note-8.md)**|
|**[Redmi 9T](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Redmi-9T.md)**|
|**[Xperia XZ1 Compact](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Sony-Xperia-XZ1-Compact.md)**|
|**[Mi A3](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Mi-A3.md)**|
|**[Poco F1](https://github.com/Robotix22/MU-Qcom/blob/main/Status/Xiaomi-Poco-F1.md)**|

</td><td>

|✅|Working|
|--|--------|
|⚠️|**Problematic**|
|❌|**Not Working**|
|❔|**Unknown**|

</td></tr> </table>

## Building

<table>
<tr><th>Debian / Ubuntu (Recomended)</th><th>Windows</th></tr>
<tr><td>

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

</td><td>

### Requirements:

[Python 3](https://www.python.org/downloads/) <br />
[VS Code](https://aka.ms/vs/17/release/vs_buildtools.exe) <br />
[Git](https://git-scm.com/download/win) <br />
[Chocolatey](https://chocolatey.org/install) <br />
[Clang](https://github.com/llvm/llvm-project/releases/tag/llvmorg-15.0.6) (LLVM-15.0.6-win64.exe)

### Step 1:

First of you need the sourcecode to compile a UEFI Image. <br />
Clone the Repo by doing:
```
git clone https://github.com/Robotix22/MU-Qcom.git --recursive
cd MU-Qcom
```

### Step 2:

Now we install needed Packages:
```
python -m pip install --upgrade -r pip-requirements.txt
choco install make
```

### Step 3:

Lets begin the real UEFI build by doing:
```
build_uefi.bat <Codename> [<Build Mode>] [<RAM Size>] [<Panel Type>] [TRUE]
```
(if it fails with "Acess Denied" disable your Antivirus)

After that you will find an `.img` File on the root of the repo.

</td></tr> </table>

## Credits

This repo is based on [SurfaceDuoPkg](https://github.com/WOA-Project/SurfaceDuoPkg)

## License

All code except drivers in `GPLDriver` directory are licensed under BSD 2-Clause.
GPL Drivers are licensed under GPLv2 license.
