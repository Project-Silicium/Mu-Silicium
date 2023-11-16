# [Project Mu](https://microsoft.github.io/mu/) UEFI Implementation for Xiaomi Mi 11

<!-- ![Banner](https://github.com/Robotix22/Mu-Qcom/blob/main/Pictures/Banner.png) -->

## Description

This repo brings UEFI Firmware on your Xiaomi Mi 11 to boot OSs like Windows or Linux

## Resources

[Discord Server](https://discord.gg/Dx2QgMx7Sv)

## [Status](https://github.com/Robotix22/Mu-Qcom/blob/main/Status.md)

## [Building](https://github.com/Robotix22/Mu-Qcom/blob/main/Building.md)
## [Building 2]
First do setup_env.sh, tut is linked in building guide above
then ./build_uefi.sh -d venus -r DEBUG -m 8
Now you can choose between -m 8 or -m 12 (12 is untested) (those are memory configs)
You can choose between building a Debug build or a Release build, if you want release then swap -r DEBUG with -r RELEASE

After it build, you can find the image in the Mu-Qcom-Venus-Mi11 folder.
You can temp boot it with fastboot : 
```
fastboot boot Mu-venus.img
```
Or flash it to always boot with Mu :
```
fastboot flash boot Mu-venus.img
```

Done. You can press Vol up while boot to get into FrontPage, or Vol down to enter UEFI Shell.
## [Guides](https://github.com/Robotix22/UEFI-Guides/blob/main/Mu-Qcom/README.md)

## Credits

Thanks to [Gustave Monce](https://github.com/gus33000) for his [SurfaceDuoPkg](https://github.com/WOA-Project/SurfaceDuoPkg), without it this repo would probally not exist.

## License

All code except drivers in `GPLDrivers` directory are licensed under BSD 2-Clause. <br />
GPL Drivers are licensed under GPLv2 license.
