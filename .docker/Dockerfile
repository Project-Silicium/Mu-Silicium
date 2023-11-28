FROM debian:trixie AS edk2-builder

# Update package list and install necessary packages
RUN apt update -y && \
    apt install -y pip git mono-devel build-essential nuget uuid-dev iasl nasm gcc-aarch64-linux-gnu gcc-arm-linux-gnueabihf python3 python3-distutils python3-git python3-pip gettext locales gnupg ca-certificates python3-venv git-core clang llvm curl python-is-python3 device-tree-compiler adb fastboot

# Create an executable bash script /runasuser.sh
COPY ./runasuser.sh /runasuser.sh

RUN chmod +x /runasuser.sh

WORKDIR /app

# Label the image
LABEL maintainer="Dani Ash <d4n1.551@gmail.com>" \
      description="Image for EDK2 development with necessary tools" \
      version="latest" \
      edk2-builder="latest"