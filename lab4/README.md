# Lab 4

___
10/30/2021

## Test Environment

Ubuntu 16.04, Ubuntu 18.04

## Build
### Build in Ubuntu (Cmake-3.10,GCC-7.5)
```
cd lab4
mkdir build && cd build
cmake ..
make
```

### BUild on pace-ice (GCC-10.1)
I have provide the executable file in this folder, if you want to build again, please follow the guide:
(Following steps can not be run in vnc command line, only vpn command line works)
```
cd lab4
./gccBuild.sh
```

## Run
### Run in Ubuntu
```
./server 6XXXX
```
```
./client localhost 6XXXX
./client 127.0.0.1 6XXXX
```
### Run in pace-ice
```
./server.sh 6XXXX
./client.sh localhost 6XXXX
```
or
```
LD_LIBRARY_PATH=./SFML-2.5.1/lib ./server 6XXXX
LD_LIBRARY_PATH=./SFML-2.5.1/lib ./client localhost 6XXXX
```
If LD_LIBRARY_PATH was not indicated explicitly, some dynamic library errors will occurs when run the program.