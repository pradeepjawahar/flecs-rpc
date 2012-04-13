#!/bin/bash

set -e
set -u

if [ $# -ne 2 ]
then
	echo "Usage: `basename $0` user cmd"
	exit 1
fi


USER=$1
CMD=$2

for i in 1 2 4 5
do
HOSTNAME="polynesia"$i".cc.gatech.edu"

ssh $USER@$HOSTNAME <<!

hostname
time $CMD
echo

exit
!
done
