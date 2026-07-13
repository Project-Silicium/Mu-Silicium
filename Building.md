# Building

> [!TIP]
> For the Compilation, a Linux OS is recommended.

## Cloning the Source Code

To Compile this UEFI Project in the first Place, You have to Download the Source Code. <br>
You can do this by using the `git` Command:
```
git clone https://github.com/Project-Silicium/Mu-Silicium.git --recursive
```
The entire Source is about ~5GB in Size, so the Download Time depends on your Internet Speed and Storage Speed.
At the End you should see a new Folder called `Mu-Silicium`.

## Platform Specific Steps

<details>
<summary>Windows</summary>

## Requirements
  - [Git](https://git-scm.com/install/windows)
  - [Python](https://www.python.org/downloads/) Version 3.11 or higher
  - [LLVM](https://github.com/llvm/llvm-project/releases/)
  - [VS Build Tools](https://aka.ms/vs/stable/vs_BuildTools.exe)
  - [GnuMake32](https://gnuwin32.sourceforge.net/packages/make.htm)

## Setting Up PATH

Since GnuMake32 doesn't add itself to PATH, you have to add it manually. <br>
You can Update the PATH Variable using UI in Windows, Simply Enter "Path" in your Search Bar. <br>
Once you Open the Window, Go to System PATH and add two new Entries.

The Entry should Point to the Install `bin` Folder of GnuMake32. <br>
The Install Location Depends on what Path you Extracted it to.

## Installing pip Dependencies

Before you can Start to Compile this UEFI Project, You have to Install the pip Dependencies. <br>
Open a Command Prompt Window in the same Folder as the Source Code. <br>
Once you did that, Run this Command to Install all pip Dependencies:
```cmd
python3 -m pip install -r pip-requirements.txt
```

---

</details>

<details>
<summary>Linux</summary>

## Installing Dependencies

Before you can Start to Compile this UEFI Project, You have to Install the Dependencies. <br>
Open a Terminal in the same Folder as the Source Code. <br>
Once you did that, Run this Command to Install all Dependencies:
```bash
./setup_env.sh -p [PACKAGE_MANAGER_NAME]
```
The `[PACKAGE_MANAGER_NAME]` Argument here is the Name of your Distro Package Manager. <br>
For Example, for Ubuntu or Debian users the Name would be `apt`.

</details>

## Compiling UEFI

> [!IMPORTANT]
> Depending on your Python Install, You might have to use `python` instead of `python3`.

After you have Done all the Steps from before, You're now ready to Compile UEFI with this Command:
```
python3 build_uefi.py -d [DEVICE_CODENAME]
```
The `[DEVICE_CODENAME]` Argument here is the Codename of your Device. <br>
You can Find the Codename of the Device in the [Status Page](Status.md).

# Updating

To get these new Updates and Changes you can use the `-u` Parameter to Update your Local Repo when Building:
```
python3 build_uefi.py -d [DEVICE_CODENAME] -u
```

# Tips & Tricks

## Faster Build Times on Windows

If you're Compiling on Windows, you probably Experience slow Builds Times. <br>
Here are some Suggestions to Improve this:

### Setting Up a Dev Drive

One Way to Improve the Build Times is using a Dev Drive in Windows. <br>
You can Find more Details, the Requirements and How to Set it Up on the [Microsoft Website](https://learn.microsoft.com/en-us/windows/dev-drive/).

### Adding a Antivirus Exception

> [!CAUTION]
> This will Disable Security in the `Mu-Silicium` Folder! <br>
> Only follow this if you're Comfortable with that.

Another Way to Improve the Build Times is adding the `Mu-Silicium` as an Exception in your Antivirus. <br>
How to do that Highly depends on the Antivirus you're using, On Microsoft Defender you can do it like this:
```
Virus & thread protection -> Virus & thread protection settings -> Exclusions -> Add or remove exclusions
```
In there you can Add the `Mu-Silicium` Folder as an Exclusion.
