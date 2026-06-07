#!/usr/bin/env python3

from pathlib import Path
from dataclasses import dataclass

import os
import sys
import glob
import gzip
import shutil
import subprocess
import coloredlogs
import argparse
import logging
import tomllib

#
# Logger
#
logger: logging.Logger

#
# Default Paths
#
BOOT_SHIM_PATH        = Path ("BootShim")
CONFIG_PATH           = Path ("Conf")
BUILD_PATH            = Path ("Build")
PLATFORM_PATH         = Path ("Platforms")
RESOURCE_CONFIGS_PATH = Path ("Resources/Configs")
RESOURCE_DTBS_PATH    = Path ("Resources/DTBs")
RESOURCE_PATCHES_PATH = Path ("Resources/MuPatches")
RESOURCE_SCRIPTS_PATH = Path ("Resources/Scripts")

#
# MU Submodule Paths
#
MU_PATH            = Path ("Common/Mu")
MU_OEM_SAMPLE_PATH = Path ("Common/Mu_OEM_Sample")
MU_BASECORE_PATH   = Path ("Mu_Basecore")

@dataclass
class BuildContext:
    device:            str
    device_model:      int
    build_mode:        str
    enable_secureboot: bool
    cleanup:           bool

def setup_logger ():
    global logger

    # Create Logger
    logger = logging.getLogger (sys.argv[0])

    # Set Logger Attributes
    coloredlogs.install (
        level="INFO",
        fmt="%(message)s",
        level_styles={
            "warning": {
                "color": "yellow"
            },

            "error": {
                "color": "red",
                "bold": True
            }
        }
    )

def parse_arguments () -> BuildContext:
    # Create Argument Parser
    parser = argparse.ArgumentParser (description="Build Project Silicium UEFI for the defined ARM64-Based Device.")

    # Add Script Arguments
    parser.add_argument ("-d", "--device",            type=str, required=True,                                   help="Defines the Device to Build.")
    parser.add_argument ("-m", "--model",             type=int, default=0,                                       help="Defines the Release Type of the Build.")
    parser.add_argument ("-r", "--release",           type=str, default="RELEASE", choices=["RELEASE", "DEBUG"], help="Defines the Model Type of the Selected Target Device.")
    parser.add_argument ("-s", "--enable-secureboot", action="store_true",                                       help="Enables Secure Boot.")
    parser.add_argument ("-c", "--clean",             action="store_true",                                       help="Removes Old Build Files and Starts a Clean Build.")

    # Parse Script Arguments
    args = parser.parse_args ()

    # Return Class
    return BuildContext (
        device            = args.device,
        device_model      = args.model,
        build_mode        = args.release,
        enable_secureboot = args.enable_secureboot,
        cleanup           = args.clean
    )

def get_device_package_path (device: str) -> Path | None:
    # Set Device Package Name
    package_name = f"{device}Pkg"

    # Set Search Pattern
    search_pattern = [f"*/{package_name}", f"*/*/{package_name}"]

    # Find Device Package Folder
    matches = (
        match
        for search_pattern in search_pattern
        for match in PLATFORM_PATH.glob (search_pattern)
        if match.is_dir ()
    )

    # Return First Match
    return next (matches, None)

def parse_device_config (device: str) -> dict | None:
    # Set Device Config Path
    config_path = RESOURCE_CONFIGS_PATH / f"{device}.toml"

    # Verify Path Type
    if not config_path.is_file ():
        return None

    # Open Device Config File
    with open (config_path, "rb") as file:
        return tomllib.load (file)

def cleanup_old_build (device: str, device_model: int, cleanup: bool):
    # Delete Device Build File
    if cleanup:
        shutil.rmtree (BUILD_PATH / f"{device}Pkg", ignore_errors=True)

    # Set Old Build Files
    old_build_files = [
        BOOT_SHIM_PATH / "BootShim.elf",
        BOOT_SHIM_PATH / "BootShim.bin",
        BUILD_PATH / f"kernel-{device}",
        Path (f"Mu-{device}-{device_model}.img"),
        Path (f"Mu-{device}-{device_model}.bin")
    ]

    # Delete Old Build Files
    for old_build_file in old_build_files:
        old_build_file.unlink (missing_ok=True)

def handle_git_patch (submodule_path: Path, patch_name: str, remove: bool) -> bool:
    # Set Patch Path
    patch_path = RESOURCE_PATCHES_PATH / patch_name

    # Verify Patch Existence
    if not patch_path.is_file ():
        return False

    # Get Absolute Patch Path
    absolute_patch_path = str (patch_path.resolve ())

    if remove:
        # Undo Patch
        subprocess.run (
            ["git", "apply", "-R", absolute_patch_path],
            cwd=submodule_path,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )

        return True

    # Apply Path
    return subprocess.run (["git", "apply", absolute_patch_path], cwd=submodule_path).returncode == 0

def compile_boot_shim (boot_shim_config: list, uefi_fd_config: list) -> bool:
    # Set Compile Command
    cmd = [
        "make",
        f"REQUIRES_KERNEL_HEADER={int (boot_shim_config['requires_kernel_header'])}",
        f"FD_BASE={hex (uefi_fd_config['fd_base'])}",
        f"FD_SIZE={hex (uefi_fd_config['fd_size'])}"
    ]

    # Compile Boot Shim
    return subprocess.run (cmd, cwd=BOOT_SHIM_PATH).returncode == 0

def prepare_uefi_environment (script_path: Path, build_mode: str) -> bool:
    # Go thru each Environment Action
    for action in ["--setup", "--update"]:
        # Exeucute Environment Action
        result = subprocess.run ([sys.executable, str (script_path), action, "-t", build_mode])
        if result.returncode != 0:
            return False

    return True

def compile_uefi (ctx: BuildContext, fd_config: list, script_path: Path) -> bool:
    # Set Compile Command
    cmd = [
        sys.executable,
        script_path,
        f"TARGET={ctx.build_mode}",
        f"ENABLE_SECUREBOOT={int (ctx.enable_secureboot)}",
        f"FD_BASE={hex (fd_config['fd_base'])}",
        f"FD_SIZE={hex (fd_config['fd_size'])}",
        f"FD_BLOCKS={hex (fd_config['fd_blocks'])}",
        f"DEVICE_MODEL={ctx.device_model}"
    ]

    # Compile UEFI
    return subprocess.run (cmd).returncode == 0

def create_android_boot_img (image_config: list, ctx: BuildContext) -> bool:
    # Set Required Paths
    boot_shim_payload_path = BOOT_SHIM_PATH / "BootShim.bin"
    device_fv_path         = BUILD_PATH / f"{ctx.device}Pkg" / f"{ctx.build_mode}_CLANGPDB" / "FV"
    uefi_fd_path           = device_fv_path / "SILICIUM_UEFI.fd"
    uefi_boot_shim_path    = device_fv_path / "SILICIUM_UEFI.fd-bootshim"
    uefi_boot_shim_gz_path = device_fv_path / "SILICIUM_UEFI.fd-bootshim.gz"
    device_dtb_path        = RESOURCE_DTBS_PATH / f"{ctx.device}.dtb"
    android_kernel_path    = BUILD_PATH / f"kernel-{ctx.device}"

    # Append Boot Shim
    try:
        # Create Combined UEFI FD File
        with open (uefi_boot_shim_path, 'wb') as combined_fd_file:
            # Append Boot Shim Payload
            with open (boot_shim_payload_path, 'rb') as boot_shim_payload_file:
                shutil.copyfileobj (boot_shim_payload_file, combined_fd_file)

            # Append Device UEFI FD
            with open (uefi_fd_path, 'rb') as device_uefi_fd_file:
                shutil.copyfileobj (device_uefi_fd_file, combined_fd_file)
    except Exception as e:
        logger.error (f"Failed to Create Combined UEFI FD File! Error = {e}")
        return False

    # Get Kernel Compression Type
    kernel_compression = image_config["kernel_compression"]

    # Check Kernel Compression Type
    if kernel_compression == "gzip":
        # Compress Kernel
        try:
            # Open Combined UEFI FD File
            with open (uefi_boot_shim_path, 'rb') as combined_fd_file:
                # Compress Combined UEFI FD File
                with gzip.open (uefi_boot_shim_gz_path, 'wb') as compressed_fd_file:
                    shutil.copyfileobj (combined_fd_file, compressed_fd_file)
        except Exception as e:
            logger.error (f"Failed to Compress Combined UEFI FD File! Error = {e}")
            return False
    elif kernel_compression == "none":
        pass
    else:
        logger.error (f"Unknown Kernel Compression Type = \"{kernel_compression}\"!")
        return False

    # Get DTB Location
    dtb_location = image_config["dtb_location"]

    # Check DTB Location
    if dtb_location == "kernel":
        # Append Android DTB
        try:
            # Create Final Android Kernel
            with open (android_kernel_path, 'wb') as android_kernel_file:
                # Open Combined UEFI FD File
                if kernel_compression == "none":
                    with open (uefi_boot_shim_path, 'rb') as combined_fd_file:
                        shutil.copyfileobj (combined_fd_file, android_kernel_file)
                else:
                    with open (uefi_boot_shim_gz_path, 'rb') as combined_fd_file:
                        shutil.copyfileobj (combined_fd_file, android_kernel_file)

                # Open Device DTB
                with open (device_dtb_path, 'rb') as device_dtb_file:
                    shutil.copyfileobj (device_dtb_file, android_kernel_file)
        except Exception as e:
            logger.error (f"Failed to Append Android DTB! Error = {e}")
            return False
    elif dtb_location == "outside" or dtb_location == "none":
        try:
            # Create Final Android Kernel
            with open (android_kernel_path, 'wb') as android_kernel_file:
                # Open Combined UEFI FD File
                if kernel_compression == "none":
                    with open (uefi_boot_shim_path, 'rb') as combined_fd_file:
                        shutil.copyfileobj (combined_fd_file, android_kernel_file)
                else:
                    with open (uefi_boot_shim_gz_path, 'rb') as combined_fd_file:
                        shutil.copyfileobj (combined_fd_file, android_kernel_file)
        except Exception as e:
            logger.error (f"Failed to Create Final Android Kernel! Error = {e}")
            return False
    else:
        logger.error (f"Unknown DTB Location = \"{dtb_location}\"!")
        return False

    # Set mkbootimg Command
    cmd = [
        sys.executable, "Resources/Scripts/mkbootimg.py",
        "--kernel", android_kernel_path,
        "--ramdisk", "Resources/ramdisk",
        "--header_version", str (image_config["header_version"]),
        "-o", f"Mu-{ctx.device}-{ctx.device_model}.img"
    ]

    # Append DTB Location
    if dtb_location == "outside":
        cmd.append ("--dtb")
        cmd.append (device_dtb_path)

    # Execute mkbootimg Command
    return subprocess.run (cmd).returncode == 0

def create_payload_file (payload_config: list, ctx: BuildContext) -> bool:
    # Set Required Paths
    boot_shim_payload_path = BOOT_SHIM_PATH / "BootShim.bin"
    device_fv_path         = BUILD_PATH / f"{ctx.device}Pkg" / f"{ctx.build_mode}_CLANGPDB" / "FV"
    uefi_fd_path           = device_fv_path / f"{ctx.device.upper ()}_UEFI.fd"
    payload_file_output    = f"Mu-{ctx.device}-{ctx.device_model}.bin"

    # Get Payload Type
    payload_type = payload_config["type"]

    # Check Payload Type
    if payload_type == "raw":
        # Append Boot Shim
        try:
            # Create Combined UEFI FD File
            with open (payload_file_output, 'wb') as combined_fd_file:
                # Append Boot Shim Payload
                with open (boot_shim_payload_path, 'rb') as boot_shim_payload_file:
                    shutil.copyfileobj (boot_shim_payload_file, combined_fd_file)

                # Append Device UEFI FD
                with open (uefi_fd_path, 'rb') as device_uefi_fd_file:
                    shutil.copyfileobj (device_uefi_fd_file, combined_fd_file)
        except Exception as e:
            logger.error (f"Failed to Create Combined UEFI FD File! Error = {e}")
            return False
    else:
        logger.error (f"Unknown Payload Type = \"{payload_type}\"!")
        return False

    return True

def create_output_image (device_data: dict, ctx: BuildContext) -> bool:
    # Check Output Type
    if "boot_image" in device_data:
        # Create Android Boot Image
        if not create_android_boot_img (device_data["boot_image"], ctx):
            return False
    elif "payload" in device_data:
        # Create Payload File
        if not create_payload_file (device_data["payload"], ctx):
            return False

    return True

def main ():
    # Setup Logger
    setup_logger ()

    # Parse Script Arguments
    ctx = parse_arguments ()

    # Get Device Package Path
    device_package_path = get_device_package_path (ctx.device)
    if not device_package_path:
        logger.error (f"There are no Build Files for \"{ctx.device}\"!")
        sys.exit (1)

    # Parse Device Config File
    device_data = parse_device_config (ctx.device)
    if not device_data:
        logger.error (f"There is no Config File in \"{RESOURCE_CONFIGS_PATH}\" for \"{ctx.device}\"!")
        sys.exit (1)

    # Append FD Block Size
    device_data["uefi_fd"]["fd_blocks"] = device_data["uefi_fd"]["fd_size"] // 4096

    # Set Device Script Path
    device_script_path = device_package_path / "DeviceBuild.py"

    # Delete Old Build Files
    cleanup_old_build (ctx.device, ctx.device_model, ctx.cleanup)

    # Remove Mu_Basecore Patches
    for patch_name in ["Auth-Service.patch", "Timer.patch", "Usb-Bus.patch"]:
        handle_git_patch (MU_BASECORE_PATH, patch_name, True)

    # Compile Boot Shim
    if not compile_boot_shim (device_data["boot_shim"], device_data["uefi_fd"]):
        sys.exit (1)

    # Prepare UEFI Environment
    if not prepare_uefi_environment (device_script_path, ctx.build_mode):
        sys.exit (1)

    # Apply Mu_Basecore Patches
    for patch_name in ["Auth-Service.patch", "Timer.patch", "Usb-Bus.patch"]:
        if not handle_git_patch (MU_BASECORE_PATH, patch_name, False):
            sys.exit (1)

    # Compile Device UEFI
    if not compile_uefi (ctx, device_data["uefi_fd"], device_script_path):
        sys.exit (1)

    # Create Output Image
    if not create_output_image (device_data, ctx):
        sys.exit (1)

if __name__ == "__main__":
    main ()
