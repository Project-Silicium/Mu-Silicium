#!/bin/bash

# Function to display Help Message
function _help(){
    echo "Usage: setup_env.sh -p <Package Manager>"
    echo
    echo "Install all needed Packages."
    echo
    echo "Options:"
    echo "  --package-manager PAK, -p PAK:   Chose what Package Manager you use."
    echo "  --venv, -v:                      Installs pip requirements in venv not local."
    echo "  --help, -h:                      Shows this Help."
    echo
    echo "MainPage: https://github.com/Robotix22/Mu-Qcom"
    exit 1
}

# Functions to display the Message Type (Error or Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2;exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2;exit 1; }

# Set Default Defines
VENV="FALSE"

# Check if any args were given
OPTS="$(getopt -o v,p:hfabcACDO: -l package-manager:,help,venv -n 'setup_env.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do  case "${1}" in
        -p|--package-manager) PAK="${2}";shift 2;;
        -v|-venv) VENV="TRUE";shift;;
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
    sudo apt install -y pip git mono-devel build-essential nuget uuid-dev iasl nasm gcc-aarch64-linux-gnu python3 python3-distutils python3-git python3-pip gettext locales gnupg ca-certificates python3-venv git git-core clang llvm curl||_error "\nFailed to install Packages!\n"
elif [ ${PAK} = dnf ]; then
    sudo dnf install -y git mono-devel nuget iasl nasm make gcc automake gcc-aarch64-linux-gnukernel-devel python3 python3-pip gettext gnupg ca-certificates git git-core clang llvm curl||_error "\nFailed to install Packages!\n"
elif [ ${PAK} = pacman ] || [ ${PAK} = yay ]; then
    if [ ${PAK} = pacman ]; then
        sudo pacman -Syu --needed git base-devel && git clone https://aur.archlinux.org/yay.git && cd yay && makepkg -si
    fi
    yay -Sy git mono base-devel nuget uuid iasl nasm aarch64-linux-gnu-gcc python3 python python-distutils-extra python-git python-pip gettext gnupg ca-certificates python-virtualenv python-pipenv core-git clang llvm curl||_error "\nFailed to install Packages!\n"
else
    _error "\nInvaild Package Manager!\nAvailbe Package Managers: apt, dnf, pacman and yay\n"
fi

if [ ${VENV} = TRUE ]; then
    python3 -m venv .venv
    source .venv/bin/activate
fi

python3 -m pip install -r pip-requirements.txt||_error "\nFailed to install Pip Packages!\n"

export CLANG38_BIN=/usr/lib/llvm-38/bin/
export CLANG38_AARCH64_PREFIX=aarch64-linux-gnu-
