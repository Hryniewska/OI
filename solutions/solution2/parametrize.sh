#!/bin/bash
ZMIENNA=10
while [$ZMIENNA -le 90]; do
./bin/run --solution -param $ZMIENNA ../../environment/data/test/images/ > wynik_$ZMIENNA.txt
$ZMIENNA=$[ZMIENNA + 10]
done
