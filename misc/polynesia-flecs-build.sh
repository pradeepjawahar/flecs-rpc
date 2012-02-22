#!/bin/bash

set -e
set -u

USER=hyoon45

for i in 1 2 4 5
do
HOSTNAME="polynesia"$i".cc.gatech.edu"

ssh -A $USER@$HOSTNAME <<!

TERM=linux

hostname

cd /dev/shm/work/flecs-rpc/.build

git pull
echo

\rm -rf *
cmake ..
make -j

echo

exit
!
done
