#!/bin/bash

# update source code and build. update uncommitted code with scp, not with git.

set -e
set -u

USER=hyoon45
#USER=hobin

tar cf tmp.tar ../../* --exclude=.build --exclude=tags --exclude=.git


for i in 1 2 5
do
HOSTNAME="polynesia"$i".cc.gatech.edu"
#HOSTNAME="flecs"$i"0"

scp -r tmp.tar $USER@$HOSTNAME:/dev/shm/flecs/src

ssh -A $USER@$HOSTNAME <<!

TERM=linux

hostname

cd /dev/shm/flecs/src/

tar xf tmp.tar
\rm tmp.tar

mkdir -p /dev/shm/flecs/src/.build
cd /dev/shm/flecs/src/.build

echo

cmake ..
make -j8

echo

exit
!
done


\rm tmp.tar
