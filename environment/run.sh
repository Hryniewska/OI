#!/bin/bash

#PID=$$
#STDOUT=/proc/$PID/fd/1

PWD=$(pwd)
ENVIRONMENT=${PWD}
SOLUTION=$1

./build.sh
#echo "build environment done"

cd ../solutions/$SOLUTION

./build.sh
#echo "build solution done"
#./deploy.sh
./run.sh
#echo "run solution done"

cd ${ENVIRONMENT}/bin

./run
#echo "run environment done"
