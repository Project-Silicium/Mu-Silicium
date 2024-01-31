# Building

*Recommended Linux OS: Ubuntu 22.04.3 LTS*
*Or use the docker method*

## If using docker method
### Step 1:

First of you need the sourcecode to compile a UEFI Image. <br />
Clone the Repo by using:
```
git clone https://github.com/Robotix22/Mu-Qcom.git --recursive
cd Mu-Qcom
```

You need to have `git` installed.

### Step 2
After the clone is done, proceed to open a shell/powershell/terminal inside the clone location
For Linux/MacOS/BSD users, run:
```
./docker-build.sh "-d <Codename> [-r <Build Mode>] [-m <RAM Size>]"
```
You need the "double quotes"

For Windows users, run (not tested):
```
.\docker-build.cmd "-d <Codename> [-r <Build Mode>] [-m <RAM Size>]"
```
You need the "double quotes".

When then Build is done you will find a `.img` File in the root of the repo.

## If building natively

### Step 1:

First of you need the sourcecode to compile a UEFI Image. <br />
Clone the Repo by using:
```
git clone https://github.com/Robotix22/Mu-Qcom.git --recursive
cd Mu-Qcom
```

### Step 2:

After Cloning the repo we can now continue on Setting up the Environment. <br />
First we need to install the needed Packages:
```
./setup_env.sh -p <Package Manager> [-v]
```

### Step 3:

So now we are able to begin the real UEFI build:
```
./build_uefi.sh -d <Codename> [-r <Build Mode>] [-m <RAM Size>]
```

When then Build is done you will find a `.img` File in the root of the repo.

## Troubleshooting:

### Python Requirements:
   1. You may encounter an issue That the required package is not satisfied or something, If your Python Version is lower than 3.10 install Python 3.10 or newer
   2. After installing Python 3.10 Linux won't automatically chose Python 3.10 as default, To set it as default use these commands:
      ```
      sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.10 1
      sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10 1
      ```
   3. Now check the Python Version:
      ```
      python3 --version
      ```
      You should get this output:
      ```
      Python 3.10.*
      ```

### Git Unknown Switch
   1. If you have an old Version of git you may come across this Issue, So you need to install the latest version of git.
   2. I prefer using `apt` to update git but if it tells you it is already newest version you should follow [this](https://www.fosslinux.com/64522/install-git-debian-linux.htm) Guide

### Device dosen't boot UEFI

   1. If your Device dosen't boot the UEFI and is just stuck on the boot screen then maybe the DTB is the Problem.
   2. Dump your DTB from Android `dd if=/sys/firmware/fdt of=/sdcard/<Device Codename>.dtb`.
   3. After that replace `ImageResources/DTBs/<Device Codename>.dtb` with your dumped DTB.
