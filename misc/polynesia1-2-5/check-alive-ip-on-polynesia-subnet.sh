#!/bin/bash

set -e
set -u

for (( i=1; i<256; i++ ))
do

IP=130.207.110.$i

ping -c 3 $IP >> alive-ip-poly.txt 2>&1 || true

done
