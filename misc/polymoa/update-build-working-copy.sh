#!/bin/bash

# update source code and build. update uncommitted code with scp, not with git.

set -e
set -u

USER=hyoon45
DATETIME=`date +"%y%m%d-%H%M%S"`


job_main()
{
	HOSTNAME=$1
	echo "-------------------- "$HOSTNAME" -------------------" 

	scp tmp.tar $USER@$HOSTNAME:/dev/shm/flecs/src

	ssh -A -t -t $USER@$HOSTNAME <<!
set -e
mkdir -p /dev/shm/flecs/src/
cd /dev/shm/flecs/src/

tar xf tmp.tar
\rm tmp.tar

mkdir -p /dev/shm/flecs/src/.build
cd /dev/shm/flecs/src/.build

echo
cmake ..
make -j8

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


echo "tar source code..."
echo
tar cf tmp.tar ../../* --exclude=.build --exclude=tags --exclude=.git 2>/dev/null

run_parallel ${MACHINES[@]}

\rm tmp.tar
