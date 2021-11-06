#!/bin/bash
mkdir build
cd build

g++ ../Lab2_Problem1.cpp -o p1 -O3 -pthread
g++ ../Lab2_Problem2.cpp -o p2 -fopenmp