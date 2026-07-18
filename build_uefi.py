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
    update:            bool

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
    parser.add_argument ("-u", "--update",            action="store_true",                                       help="Updates your Local Repo before Building.")

    # Parse Script Arguments
    args = parser.parse_args ()

    # Return Class
    return BuildContext (
        device            = args.device,
        device_model      = args.model,
        build_mode        = args.release,
        enable_secureboot = args.enable_secureboot,
        cleanup           = args.clean,
        update            = args.update
    )

def get_device_package_path (device: str) -> Path | None:
    # Set Device Package Name
    package_name = f"{device}Pkg"

    # Set Search Pattern
    search_patterns = [f"*/{package_name}", f"*/*/{package_name}"]

    # Find Device Package Folder
    matches = (
        match
        for pattern in search_patterns
        for match in PLATFORM_PATH.glob (pattern)
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
        Path (f"Mu-{device}-{device_model}.bin"),
        Path (f"Mu-{device}.img"),
        Path (f"Mu-{device}.bin")
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

    # Set Git Command
    cmd = [
        "git",
        "apply",
        absolute_patch_path
    ]

    # Append Undo Flag
    if remove:
        cmd.append ("-R")

    # Apply/Undo Path
    return subprocess.run (cmd, cwd=submodule_path).returncode == 0

def update_local_repo () -> bool:
    # Pull Latest Changes
    if not subprocess.run (["git", "pull"]).returncode == 0:
        return False

    # Update Submodules
    if not subprocess.run (["git", "submodule", "update"]).returncode == 0:
        return False

    return True

def compile_boot_shim (boot_shim_config: dict, fd_config: dict) -> bool:
    # Get FD Details
    fd_base = hex (fd_config.get ("base"))
    fd_size = hex (fd_config.get ("size"))

    # Get Kernel Header Flag
    requires_kernel_header = boot_shim_config.get ("requires_kernel_header", False)

    # Set Compile Command
    cmd = [
        "make",
        f"REQUIRES_KERNEL_HEADER={int (requires_kernel_header)}",
        f"FD_BASE={fd_base}",
        f"FD_SIZE={fd_size}"
    ]

    # Compile Boot Shim
    return subprocess.run (cmd, cwd=BOOT_SHIM_PATH).returncode == 0

def prepare_uefi_environment (script_path: Path, build_mode: str, update: bool) -> bool:
    # Skip UEFI Env Setup
    if not update and os.path.isdir (BUILD_PATH):
        return True

    # Go thru each Environment Action
    for action in ["--setup", "--update"]:
        # Exeucute Environment Action
        result = subprocess.run ([sys.executable, str (script_path), action, "-t", build_mode])
        if result.returncode != 0:
            return False

    return True

def compile_uefi (ctx: BuildContext, fd_config: dict, script_path: Path) -> bool:
    # Get FD Details
    fd_base   = hex (fd_config.get ("base"))
    fd_size   = hex (fd_config.get ("size"))
    fd_blocks = hex (fd_config.get ("blocks"))

    # Set Compile Command
    cmd = [
        sys.executable,
        str (script_path),
        f"TARGET={ctx.build_mode}",
        f"ENABLE_SECUREBOOT={int (ctx.enable_secureboot)}",
        f"FD_BASE={fd_base}",
        f"FD_SIZE={fd_size}",
        f"FD_BLOCKS={fd_blocks}",
        f"DEVICE_MODEL={ctx.device_model}"
    ]

    # Compile UEFI
    return subprocess.run (cmd).returncode == 0

def create_android_boot_img (ctx: BuildContext, image_kernel_config: dict, image_config: dict, append_boot_shim: bool, add_output_suffix: bool) -> bool:
    # Set Base Paths
    device_pkg_path = BUILD_PATH         / f"{ctx.device}Pkg"
    fv_path         = device_pkg_path    / f"{ctx.build_mode}_CLANGPDB" / "FV"

    # Set File Paths
    device_dtb      = RESOURCE_DTBS_PATH / f"{ctx.device}.dtb"
    uefi_fd         = fv_path            / "SILICIUM_UEFI.fd"
    uefi_fd_gz      = fv_path            / "SILICIUM_UEFI.fd.gz"

    # Check Boot Shim Flag
    if append_boot_shim:
        # Set Boot Shim Paths
        boot_shim_payload = BOOT_SHIM_PATH / "BootShim.bin"
        boot_shim_fd      = fv_path        / "SILICIUM_UEFI.fd-bootshim"
        boot_shim_fd_gz   = fv_path        / "SILICIUM_UEFI.fd-bootshim.gz"

        try:
            # Read File Content
            combined_data = boot_shim_payload.read_bytes () + uefi_fd.read_bytes ()

            # Write Combined File Content
            boot_shim_fd.write_bytes (combined_data)
        except Exception as e:
            logger.error ("Failed to Append Boot Shim Payload to UEFI FD File!")
            logger.error (e)
            return False

        # Update UEFI FD Paths
        uefi_fd    = boot_shim_fd
        uefi_fd_gz = boot_shim_fd_gz

    # Get Kernel Compression Type
    kernel_compression = image_kernel_config.get ("kernel_compression")
    if kernel_compression is None:
        logger.error ("No Kernel Compression is Specified!")
        return False

    # Check Kernel Compression Type
    match kernel_compression:
        # Use GZIP Compression
        case "gzip":
            try:
                # Compress UEFI FD File
                with gzip.open (uefi_fd_gz, 'wb') as compressed_fd_file:
                    compressed_fd_file.write (uefi_fd.read_bytes ())
            except Exception as e:
                logger.error ("Failed to Compress UEFI FD File!")
                logger.error (e)
                return False

        # Use No Compression
        case "none":
            pass

        case _:
            logger.error (f"Unknown Kernel Compression Type = \"{kernel_compression}\"!")
            return False

    # Set Android Kernel Path
    android_kernel = uefi_fd if kernel_compression == "none" else uefi_fd_gz

    # Check DTB Append Flag
    if image_kernel_config.get ("append_dtb"):
        # Verify DTB Path
        if not device_dtb.is_file ():
            logger.error (f"The Device DTB is Missing in \"{RESOURCE_DTBS_PATH}\"!")
            return False

        # Append DTB
        try:
            android_kernel.write_bytes (android_kernel.read_bytes () + device_dtb.read_bytes ())
        except Exception as e:
            logger.error ("Failed to Append Android DTB!")
            logger.error (e)
            return False

    # Set "mkbootimg.py" Script Path
    mkbootimg = RESOURCE_SCRIPTS_PATH / "mkbootimg.py"
    if not mkbootimg.is_file ():
        logger.error (f"The \"mkbootimg.py\" Script is Missing in \"{RESOURCE_SCRIPTS_PATH}\"!")
        return False

    # Set Output File Name
    output_file_name = f"Mu-{ctx.device}-{ctx.device_model}.img" if add_output_suffix else f"Mu-{ctx.device}.img"

    # Set Base mkbootimg Command
    cmd = [
        sys.executable, str (mkbootimg),
        "--kernel",     str (android_kernel),
        "--ramdisk",    "Resources/ramdisk",
        "-o",           output_file_name
    ]

    # mkbootimg Argument Map
    argument_map = {
        "base":           "--base",
        "cmdline":        "--cmdline",
        "dtb":            "--dtb",
        "dtb_offset":     "--dtb_offset",
        "header_version": "--header_version",
        "kernel_offset":  "--kernel_offset",
        "os_patch_level": "--os_patch_level",
        "os_version":     "--os_version",
        "ramdisk_offset": "--ramdisk_offset",
        "second_offset":  "--second_offset",
        "tags_offset":    "--tags_offset"
    }

    # Go thru each Argument
    for key, option in argument_map.items ():
        # Get Argument Value
        value = image_config.get (key)

        # Append Argument Value
        if value:
            cmd.append (option)
            cmd.append (str (value))

    # Execute mkbootimg Command
    return subprocess.run (cmd).returncode == 0

def create_payload_file (ctx: BuildContext, payload_config: dict, append_boot_shim: bool, add_output_suffix: bool) -> bool:
    # Set Output File Name
    output_file_name = f"Mu-{ctx.device}-{ctx.device_model}.bin" if add_output_suffix else f"Mu-{ctx.device}.bin"

    # Set Base Paths
    device_pkg_path = BUILD_PATH      / f"{ctx.device}Pkg"
    fv_path         = device_pkg_path / f"{ctx.build_mode}_CLANGPDB" / "FV"

    # Set File Paths
    uefi_fd         = fv_path         / "SILICIUM_UEFI.fd"
    output_file     = Path.cwd ()     / output_file_name

    # Get Payload Type
    payload_type = payload_config.get ("type")
    if payload_type is None:
        logger.error ("No Payload Type was Specified! Please Check your Config File.")
        return False

    # Check Payload Type
    match payload_type:
        case "raw":
            if append_boot_shim:
                # Set Boot Shim Payload Path
                boot_shim_payload = BOOT_SHIM_PATH / "BootShim.bin"

                # Append Boot Shim
                output_file.write_bytes (boot_shim_payload.read_bytes () + uefi_fd.read_bytes ())
            else:
                # Write Output File
                output_file.write_bytes (uefi_fd.read_bytes ())

        case _:
            logger.error (f"Unknown Payload Type = \"{payload_type}\"!")
            return False

    return True

def create_output_image (ctx: BuildContext, config_data: dict, append_boot_shim: bool) -> bool:
    # Set Suffix Flag
    add_output_suffix = add_output_suffix = "status" in config_data and "model_count" in config_data["status"]

    # Check Output Type
    match config_data:
        case {"boot_image": image_config}:
            # Get Image Kernel Configs
            image_kernel_config = config_data.get ("boot_image_kernel")
            if image_kernel_config is None:
                logger.error ("The Boot Image Kernel Config is Missing! Please Check your Config File.")
                return False

            # Create Android Boot Image
            return create_android_boot_img (ctx, image_kernel_config, image_config, append_boot_shim, add_output_suffix)

        case {"payload": payload_config}:
            # Create Payload File
            return create_payload_file (ctx, payload_config, append_boot_shim, add_output_suffix)

        case _:
            logger.warning ("No Output Format was Specified, Skipping Output Creation.")

    return True

def main ():
    # Setup Logger
    setup_logger ()

    # Parse Script Arguments
    ctx = parse_arguments ()

    # Get Device Package Path
    device_package_path = get_device_package_path (ctx.device)
    if device_package_path is None:
        logger.error (f"There are no Build Files for \"{ctx.device}\"!")
        sys.exit (1)

    # Parse Device Config File
    config_data = parse_device_config (ctx.device)
    if config_data is None:
        logger.error (f"There is no Config File in \"{RESOURCE_CONFIGS_PATH}\" for \"{ctx.device}\"!")
        sys.exit (1)

    # Get Device Status Config
    device_status = config_data.get ("status")

    # Check Device Model Argument
    if device_status is not None:
        # Get Number of Models
        device_model_count = device_status.get ("model_count")
        if device_model_count is None:
            logger.error ("The Model Count is Missing! Please Check your Config File.")
            sys.exit (1)

        # Verify Device Model Argument
        if ctx.device_model >= device_model_count:
            logger.error (f"\"{ctx.device}\" doesn't have a Model for the Specified Model Number \"{ctx.device_model}\"!")
            sys.exit (1)

    # Get FD Config
    fd_config = config_data.get ("uefi_fd")
    if fd_config is None:
        logger.error ("The FD Config is Missing! Please Check your Config File.")
        sys.exit (1)

    # Get FD Details
    fd_base = fd_config.get ("base")
    fd_size = fd_config.get ("size")

    # Verify FD Details
    if fd_base is None or fd_size is None:
        logger.error ("The FD Config is Faulty! Please Check your Config File.")
        sys.exit (1)

    # Append FD Block Size
    fd_config["blocks"] = fd_config["size"] // 4096

    # Set Device Script Path
    device_script_path = device_package_path / "DeviceBuild.py"
    if not device_script_path.is_file ():
        logger.error (f"The Device Build Script isn't Present in \"{device_package_path}\"!")
        sys.exit (1)

    # Delete Old Build Files
    cleanup_old_build (ctx.device, ctx.device_model, ctx.cleanup)

    # Remove Mu_Basecore Patches
    for patch_name in ["Auth-Service.patch", "Boot-Manager.patch", "Timer.patch", "Usb-Bus.patch"]:
        handle_git_patch (MU_BASECORE_PATH, patch_name, True)

    # Update Local Repo
    if ctx.update:
        if not update_local_repo ():
            sys.exit (1)

    # Set Boot Shim Compile Flag
    boot_shim_present = "boot_shim" in config_data

    # Compile Boot Shim
    if boot_shim_present:
        if not compile_boot_shim (config_data.get ("boot_shim"), config_data.get ("uefi_fd")):
            sys.exit (1)

    # Prepare UEFI Environment
    if not prepare_uefi_environment (device_script_path, ctx.build_mode, ctx.update):
        sys.exit (1)

    # Apply Mu_Basecore Patches
    for patch_name in ["Auth-Service.patch", "Boot-Manager.patch", "Timer.patch", "Usb-Bus.patch"]:
        if not handle_git_patch (MU_BASECORE_PATH, patch_name, False):
            sys.exit (1)

    # Compile Device UEFI
    if not compile_uefi (ctx, fd_config, device_script_path):
        sys.exit (1)

    # Create Output Image
    if not create_output_image (ctx, config_data, boot_shim_present):
        sys.exit (1)

if __name__ == "__main__":
    main ()
