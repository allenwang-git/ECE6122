# Lab 5
---
## Dependency

* Nvidia GPU & driver
* cuda-9.0 or higher


## Build
```
source mesaSetup.sh
nvcc main.cu -o lab5 -stdc++=11
```

## Run
default value of N is 100, I is 1000
```
./lab5 -N 100 -I 1000
./lab5 -N 10
./lab5 -I 100
./lab5
```

