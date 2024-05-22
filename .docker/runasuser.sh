#!/bin/bash
set -e
if [ "$#" -ne 4 ] || [ "$1" != "--uid" ] || [ "$3" != "--command" ]; then
  echo "Usage: /runasuser.sh --uid <uid> --command <command>"
  exit 1
fi
uid=$2
command=$4
user=dockeruser
if id "$uid" &>/dev/null; then
  user=$(id -nu $uid)
else
  adduser --disabled-password --gecos "" --uid $uid $user
fi
exec su -c "$command" -s /bin/bash $user