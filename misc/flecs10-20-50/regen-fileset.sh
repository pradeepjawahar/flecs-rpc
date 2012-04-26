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

touch /dev/shm/flecs/src/.build/launcher/flecs.trigger.regen-fileset

echo

exit
!
done
