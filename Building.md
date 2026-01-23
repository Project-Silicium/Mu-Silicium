# Building

> [!TIP]
> It's recommended to use `Ubuntu 24.04.3 LTS` to Compile this.

## Preparing

Before you can even Begin to Compile a UEFI, You must clone the GitHub Repo:
```bash
# Install the "git" Package if the Command fails with "Not Found".
git clone https://github.com/Project-Silicium/Mu-Silicium.git --recursive
cd Mu-Silicium
```

Once you Cloned the GitHub Repo, You need to install the needed Packages for the Compile Process:
```bash
# Example: Debian based OS: apt
./setup_env.sh -p [Package Manager]
```

## Compiling

After you installed all the Packages you are Ready to Compile. <br>
Run the Build Script with these Params to Compile the UEFI for your Device:
```bash
# Example: For Xiaomi 11T Pro, Use "vili", Check Status.md for other Devices.
# Hint: If you want Debug Logs on your Phone, Add "-r DEBUG" to the Params.
./build_uefi.sh -d [Codename]
```

It will tell you at the End if the Build was Successful or not. <br>
If it was Successful, a Output Image will appear with this Name: `Mu-[Device-Codename]`.

# Troubleshooting

## Python Requirements

If you encounter a Problem while Installing the Python Packages saying something with "Couldn't satisfy", Update your Python Package. <br>
Python 3.10+ is Required to Compile a UEFI Image.
