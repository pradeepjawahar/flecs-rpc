#!/bin/bash

set -e
set -u

USER=hyoon45
DATETIME=`date +"%y%m%d-%H%M%S"`


job_main()
{
	HOSTNAME=$1
	echo "-------------------- "$HOSTNAME" -------------------" 

	ssh -A -t -t $USER@$HOSTNAME <<!
set -e

killall flecs-agent-server flecs-server flecs-master flecs-client flecs-launcher || true
sleep 1
killall -w -s KILL flecs-agent-server flecs-server flecs-master flecs-client flecs-launcher || true

export LD_LIBRARY_PATH=.:/usr/local/lib:/usr/local/lib64
cd /dev/shm/flecs/src/.build/agent
./flecs-agent-server
cd /dev/shm/flecs/src/.build/launcher
./flecs-launcher

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


MACHINES=(
"polynesia1.cc.gatech.edu"
"polynesia2.cc.gatech.edu"
"polynesia5.cc.gatech.edu"
"samoa5.cc.gatech.edu"
"samoa6.cc.gatech.edu"
"samoa7.cc.gatech.edu"
)


run_parallel ${MACHINES[@]}
