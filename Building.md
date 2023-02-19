# Building

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
./setup_env.sh -d <Distro>
sudo pip3 install --upgrade -r pip-requirements.txt
```

### Step 3:

So now we are able to begin the real UEFI build:
```
./build_uefi.sh -d <Codename> [-r <Build Mode>] [-m <RAM Size>] [-p <Panel Type>]
```

When then Build is done you will find a `.img` File in the root of the repo.

## Troubleshooting:

### RegularExpressionDxe:
   1. During the UEFI build, problems may occur due to the Regular Expression library. There is a workaround for this problem.
   2. You need to comment out this line:
      ```
      MdeModulePkg/Universal/RegularExpressionDxe/RegularExpressionDxe.inf
      ```
      in the file:
      ```
      ./MU-Qcom/Platforms/QcomPkg/Frontpage.dsc.inc
      ```
      by adding # in front of the line.
   3. You also need to comment out this line:
      ```
      INF MdeModulePkg/Universal/RegularExpressionDxe/RegularExpressionDxe.inf 
      ```
      in the file:
      ```
      ./MU-Qcom/Platforms/<Your platform>Pkg/<Your platform>.fdf
      ```

### Python Requirements:
   1. You may encounter an issue That the recuired package is not satisfied or something, If your Python is lower that 3.10 install Python 3.10 or newer
   2. After installing Python 3.10 Linux won't automaticly chose Python 3.10 as default, To set it as default use these commands:
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
      
