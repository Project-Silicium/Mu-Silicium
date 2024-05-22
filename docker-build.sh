#!/bin/bash

# get the script directory
REPO=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $REPO

# set version and image name/tag from git changes for the Dockerfile
VERSION=$(git --no-pager log -n 1 --pretty=format:"%H" -- Dockerfile)
IMG_NAME="mu_builder:$VERSION"

# build the image (if any changes)
docker build -t "$IMG_NAME" .

# run the actual build in the container
docker run --rm -it -v $REPO:/app "$IMG_NAME" /runasuser.sh --uid $(id -u) --command "./build_uefi.sh $@"
