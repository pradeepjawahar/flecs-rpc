#!/bin/bash

set -e
set -u

if [ $# -ne 1 ]
then
	echo "Usage: `basename $0` wait_until"
	exit 1
fi

# wait until after 10 seconds from now.
#   calc does not work with leading 0s. bc works.

CUR_TIME=`date +"%H%M%S%N"`
WAIT_UNTIL=`echo "scale=4; $CUR_TIME + 10000000000" | bc`

echo $WAIT_UNTIL

set -o verbose

# # kill existing server
# killall -w -u hyoon45 -q server || true; 
# killall -KILL -w -u hyoon45 -q server || true;

sleep 10

# restart master
if [ `hostname` == "polynesia1" ]
then
killall -w -u hyoon45 -q master || true;
killall -KILL -w -u hyoon45 -q master || true;
cd /dev/shm/work/flecs-rpc/.build/master;
(/dev/shm/work/flecs-rpc/.build/master/master > /dev/null 2>&1 &);
fi

# While I was implementing this, I realized that sometimes estabilishing ssh
# connection alone takes more than 1 min.  So implemented, less secure, but
# faster agent, hoping that it will be more flexible tool for like gathering
# result and plotting charts.

# # start server
# cd /dev/shm/work/flecs-rpc/.build/server; 
# (/dev/shm/work/flecs-rpc/.build/server/server > /dev/null 2>&1 &);
# 
# # execute client 1 sec after launching server
# sleep 1
# cd /dev/shm/work/flecs-rpc/.build/client; 
# /dev/shm/work/flecs-rpc/.build/client/client > /dev/null 2>&1 &

set +o verbose
