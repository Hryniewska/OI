#!/bin/bash
g++ src/Source.cpp -o bin/run -lboost_program_options \
    -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_imgcodecs \
    -Irun/include \
    -Lrun/lib
