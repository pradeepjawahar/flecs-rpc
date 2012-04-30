#!/bin/bash

set -e
set -u

USER=
CMD=
DATETIME=`date +"%y%m%d-%H%M%S"`


job_main()
{
	HOSTNAME=$1
	echo "-------------------- "$HOSTNAME" -------------------" 

	ssh -t -t $USER@$HOSTNAME <<!
time $CMD
echo

exit
!
}


if [ $# -ne 2 ]
then
	echo "Usage: `basename $0` user cmd"
	exit 1
fi
USER=$1
CMD=$2


# Enable Job Control
set -m

MACHINES=(
"polynesia1.cc.gatech.edu"
"polynesia2.cc.gatech.edu"
"polynesia5.cc.gatech.edu"
"samoa5.cc.gatech.edu"
"samoa6.cc.gatech.edu"
"samoa7.cc.gatech.edu"
)


echo "Parallel processing on "${#MACHINES[@]}" machines:"
for h in ${MACHINES[@]}
do
	echo " "$h
done


for h in ${MACHINES[@]}
do
job_main $h >> "batch.output-"$DATETIME"-"$h &
done

# Wait for all parallel jobs to finish
set +e
while [ 1 ]; do fg 2> /dev/null; [ $? == 1 ] && break; done
set -e

echo "all done."

for h in ${MACHINES[@]}
do
RESULT="batch.output-"$DATETIME"-"$h
echo
less $RESULT
done
