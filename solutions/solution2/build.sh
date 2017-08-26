#!/bin/bash
g++ src/Source.cpp -o bin/run -std=c++11 -lboost_program_options \
    -lopencv_highgui -lopencv_imgcodecs -lopencv_core -lopencv_imgproc \
    -Irun/include \
    -Lrun/lib
