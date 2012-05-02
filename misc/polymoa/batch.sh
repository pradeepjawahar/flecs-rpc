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
time ($CMD)
echo

exit
!
}


run_parallel()
{
	echo "Parallel processing on "$#" machines:"
	for i in "$@"
	do
		echo "  "$i
	done
	echo

	pids=()

	for (( i=0; i<$#; i++ ))
	do
		h=${@:$((i+1)):1}

		if [ $i -eq 0 ]
		then
			(job_main $h | tee -a "batch.output-"$DATETIME"-"$h) &
		else
			job_main $h >> "batch.output-"$DATETIME"-"$h &
		fi

		pids[${#pids[*]}]=$!
	done


	for i in "${pids[@]}"
	do
		wait $i || true
		#echo $i" done"
	done

	echo "all parellel jobs done"


	for h in $@
	do
		RESULT="batch.output-"$DATETIME"-"$h
		echo
		dos2unix $RESULT
		less $RESULT
	done
}


if [ $# -ne 2 ]
then
	echo "Usage: `basename $0` user cmd"
	exit 1
fi
USER=$1
CMD=$2


MACHINES=(
"polynesia1.cc.gatech.edu"
"polynesia2.cc.gatech.edu"
"polynesia5.cc.gatech.edu"
"samoa5.cc.gatech.edu"
"samoa6.cc.gatech.edu"
"samoa7.cc.gatech.edu"
)


run_parallel ${MACHINES[@]}
