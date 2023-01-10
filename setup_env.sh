#!/bin/bash

# Install Dependence
sudo apt update && sudo apt -y upgrade
sudo apt -y install python3-venv pip git mono-devel build-essential nuget build-essential uuid-dev iasl nasm gcc-aarch64-linux-gnu python3 python3-distutils python3-git python3-pip gettext locales gnupg ca-certificates python3-venv git git-core clang llvm curl

# Clang LLVM
sudo apt install -y clang llvm gcc-aarch64-linux-gnu

export CLANG38_BIN=/usr/lib/llvm-38/bin/
export CLANG38_AARCH64_PREFIX=aarch64-linux-gnu-
