# Gatech Buzzy Bowl

___
**Author: Wang, Yinuo**

**Date: 10/24/2021**

**Email: ywang3781@gatech.edu**

## Dependency

* OpenGL - install gl, glu, glm, glut

```
sudo apt install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglm-dev
```

* GLFW, GLEW Dependency

```
sudo apt-get install dos2unix git wget xorg-dev
```

* Source of GLFW, GLEW (*Have included in third-party, no need to install*)
    * GLEW-2.1.0 (http://glew.sourceforge.net/index.html)
    * GLFW-3.3.4 (https://www.glfw.org/download.html)

## Build

Under *Gatech-Buzzy-Bowl* folder:

```
mkdir build && cd build
cmake ..
make -j4
```

## Run

Under *build* directory:
`./gtbb`

## Reference

* 