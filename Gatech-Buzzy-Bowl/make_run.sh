#!/bin/bash
# build and run the project
mkdir build
cd build
cmake ..
make -j8
cp gtbb ../source/
cd ../source
./gtbb