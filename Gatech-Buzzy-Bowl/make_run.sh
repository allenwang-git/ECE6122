#!/bin/bash
# build and run the project

# compile the source code
mkdir build
cd build
cmake ..
make -j8

# must copy the executable file into source directory before running
cp gtbb ../source/
cd ../source

# run the program
./gtbb