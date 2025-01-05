#!/bin/bash

# Function to display Help Message
function _help(){
    echo "Usage: setup_env.sh -p <Package Manager>"
    echo
    echo "Install all needed Packages."
    echo
    echo "Options:"
    echo "  --package-manager PAK, -p PAK:   Chose what Package Manager you use."
    echo "  --help, -h:                      Shows this Help."
    echo 
    echo "MainPage: https://github.com/Project-Silicium/Mu-Silicium"
    exit 1
}

# Functions to display the Message Type (Error or Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2;exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2;exit 1; }

# Check if any args were given
OPTS="$(getopt -o p:hfacACDO: -l package-manager:,help -n 'setup_env.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do  case "${1}" in
        -p|--package-manager) PAK="${2}";shift 2;;
        -h|--help) _help 0;shift;;
        --) shift;break;;
        *) _help 1;;
    esac
done

# If no Package Manager arg is present, Display Help Message
if [ -z ${PAK} ]
then _help
fi

# Install all needed Packages
if [ ${PAK} = apt ]; then
    if [ $CI_BUILD == "true" ]; then
        # Update CI Ubuntu
        sudo apt update
        sudo apt full-upgrade -y
    fi
    sudo apt install -y pip git mono-devel build-essential lld uuid-dev nasm gcc-aarch64-linux-gnu gcc-arm-linux-gnueabihf python3 python3-git python3-pip gettext locales gnupg ca-certificates python3-venv git git-core clang llvm curl lld||_error "\nFailed to install Packages!\n"
elif [ ${PAK} = dnf ]; then
    sudo dnf install -y git mono-devel nuget nasm make lld gcc automake gcc-aarch64-linux-gnu arm-linux-gnueabihf-gcc python3 python3-pip gettext gnupg ca-certificates git git-core clang llvm curl lld||_error "\nFailed to install Packages!\n"
elif [ ${PAK} = pacman ] || [ ${PAK} = yay ]; then
    if [ ${PAK} = pacman ]; then
        sudo pacman -Syu --needed git base-devel && git clone https://aur.archlinux.org/yay.git && cd yay && makepkg -si
    fi
    yay -Sy git mono base-devel nuget uuid lld nasm aarch64-linux-gnu-gcc arm-linux-gnueabihf-gcc python3 python python-distutils-extra python-git python-pip gettext gnupg ca-certificates python-virtualenv python-pipenv core-git clang llvm curl lld||_error "\nFailed to install Packages!\n"
else
    _error "\nInvaild Package Manager!\nAvailbe Package Managers: apt, dnf, pacman and yay\n"
fi

# Install Needed Python Packages
python3 -m pip install -r pip-requirements.txt ||python3 -m pip install -r pip-requirements.txt --break-system-packages||_error "\nFailed to install Pip Packages!\n"

export CLANGPDB_AARCH64_PREFIX=aarch64-linux-gnu-
export CLANGDWARF_ARM_PREFIX=arm-linux-gnueabihf-
