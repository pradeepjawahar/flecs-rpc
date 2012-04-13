#!/bin/bash

set -e
set -u

#USER=hyoon45
USER=hobin

for i in 1 2 5
do
#HOSTNAME="polynesia"$i".cc.gatech.edu"
HOSTNAME="flecs"$i"0"

ssh -A $USER@$HOSTNAME <<!

TERM=linux

hostname

cd /dev/shm

if [ ! -d /dev/shm/flecs-rpc ]
then
git clone ssh://hyoon45@killerbee1.cc.gatech.edu/~/www/code/flecs-rpc
fi

mkdir -p /dev/shm/flecs-rpc/.build
cd /dev/shm/flecs-rpc/.build

git pull
echo

cmake ..
make -j8

echo

exit
!
done
