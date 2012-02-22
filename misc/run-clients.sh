#!/bin/bash

#DATE=`date +%G%m%d`

# ssh-agent bash
# ssh-add
#
# or ssh authentication forwarding with -A

set -e
set -u

if [ $# -ne 1 ]
then
	echo "Usage: `basename $0` num_of_clients"
	exit 1
fi

hostname
date

# wait until after 10 seconds from now.

CUR_TIME=`date +"%H%M%S%N"`
WAIT_UNTIL=`calc $CUR_TIME + 10000000000`

echo $WAIT_UNTIL

machines=( 1 2 4 5 )

for (( i=0; i<$1; i++ ))
do

HOSTNAME="polynesia"${machines[$i]}".cc.gatech.edu"

echo $HOSTNAME

ssh -A hyoon45@$HOSTNAME <<!

TERM=linux

hostname

daemon -o /dev/shm/flecs-client-output /dev/shm/work/flecs-rpc/misc/wait-and-exec $WAIT_UNTIL

echo

exit
!

done
