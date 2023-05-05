#!/bin/bash

function _help(){
        echo "Usage: setup_env.sh --package-manager PAK"
        echo
        echo "Install all needed Packages."
        echo
        echo "Options:"
        echo "  --package-manager PAK, -p PAK:   Chose what Package Manager you use."
        echo "  --help, -h:                      Shows this Help."
        echo
        echo "MainPage: https://github.com/Robotix22/MU-Qcom"
        exit 1
}

function _error(){ echo "${@}" >&2;exit 1; }

DISTRO=""
OPTS="$(getopt -o p:hfabcACDO: -l package-manager:,help -n 'setup_env.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do      case "${1}" in
                -p|--package-manager) PAK="${2}";shift 2;;
                -h|--help) _help 0;shift;;
                --) shift;break;;
                *) _help 1;;
        esac
done

if [ -z ${PAK} ]; then
    _help
fi

if [ ${PAK} = apt ]; then
    sudo apt update && sudo apt upgrade -y
    sudo apt install -y pip git mono-devel build-essential nuget uuid-dev iasl nasm gcc-aarch64-linux-gnu python3 python3-distutils python3-git python3-pip gettext locales gnupg ca-certificates python3-venv git git-core clang llvm curl
elif [ ${PAK} = dnf ]; then
    sudo dnf upgrade -y
    sudo dnf install -y git mono-devel nuget iasl nasm make gcc automake gcc-aarch64-linux-gnu kernel-devel python3 python3-pip gettext gnupg ca-certificates git git-core clang llvm curl
else
    _error "Invaild Package Manager! Availbe Package Managers: apt and dnf"
fi

pip install -r pip-requirements.txt

export CLANG38_BIN=/usr/lib/llvm-38/bin/
export CLANG38_AARCH64_PREFIX=aarch64-linux-gnu-
