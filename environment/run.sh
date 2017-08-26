#!/bin/bash

PID=$$
STDOUT=/proc/$PID/fd/1

PWD=$(pwd)
ENVIRONMENT=${PWD}
#SOLUTION=$1
SOLUTION=solution2

./build.sh

cd ../solutions/$SOLUTION

#./build.sh
#./deploy.sh
# ./run.sh ${ENVIRONMENT}

cd ${ENVIRONMENT}

./bin/check -t ${ENVIRONMENT}/data/test/annotations/annotations.txt \
    -s ../solutions/${SOLUTION}/solution_result.txt > wynik_WH.txt
