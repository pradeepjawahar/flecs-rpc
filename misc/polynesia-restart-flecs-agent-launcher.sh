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

killall flecs-agent-server flecs-server flecs-master flecs-client flecs-launcher || true
sleep 1
killall -w -s KILL flecs-agent-server flecs-server flecs-master flecs-client flecs-launcher || true

export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64
cd /dev/shm/work/flecs-rpc/.build/agent
./flecs-agent-server
cd /dev/shm/work/flecs-rpc/.build/launcher
./flecs-launcher

echo

exit
!
done
