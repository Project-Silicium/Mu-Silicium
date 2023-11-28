#!/bin/bash

REPO=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd $REPO

VERSION=$(git --no-pager log -n 1 --pretty=format:"%H" -- .docker/Dockerfile)
IMG_NAME="mu_builder:$VERSION"

docker build -t "$IMG_NAME" .

docker run --rm -it -v $REPO:/app "$IMG_NAME" /runasuser.sh --uid $(id -u) --command "./build_uefi.sh $@"
