#!/bin/bash
ENVIRONMENT=solution2
#echo "$(<./data/param.txt)"
./bin/run --param cat ./data/param.txt --solution ../../environment/data/test/images/ --outfile solution_result.txt

#./bin/solution -f ${ENVIRONMENT}/data/test/annotations/annotations.txt \
#    -o solution_result.txt --error 2
#-param cat ./data/param.txt
