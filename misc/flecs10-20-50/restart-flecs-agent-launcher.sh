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

killall flecs-agent-server flecs-server flecs-master flecs-client flecs-launcher || true
sleep 1
killall -w -s KILL flecs-agent-server flecs-server flecs-master flecs-client flecs-launcher || true

export LD_LIBRARY_PATH=.:/usr/local/lib:/usr/local/lib64
cd /dev/shm/flecs/src/.build/agent
./flecs-agent-server
cd /dev/shm/flecs/src/.build/launcher
./flecs-launcher

echo

exit
!
done
