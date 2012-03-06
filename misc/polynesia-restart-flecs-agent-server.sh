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


killall -w flecs-agent-server flecs-server flecs-master flecs-client || true
killall -w -s KILL flecs-agent-server flecs-server flecs-master flecs-client || true

cd /dev/shm/work/flecs-rpc/.build/agent
export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64
./flecs-agent-server

echo

exit
!
done
