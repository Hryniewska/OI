#!/bin/bash
ZMIENNA=10
while [ $ZMIENNA -le 90 ]; do
./bin/run --param $ZMIENNA --solution ../../environment/data/test/images/
cp solution_result.txt wynik_$ZMIENNA.txt
ZMIENNA=$[$ZMIENNA + 10]
done
