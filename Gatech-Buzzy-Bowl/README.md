# Gatech Buzzy Bowl

___
**Author:** Wang, Yinuo

**Date:** 10/24/2021

**Email:** ywang3781@gatech.edu

## Build & Run

Under *Gatech-Buzzy-Bowl* folder:

```
sh make_run.sh
```

## Dependency

* OpenGL - install gl, glu, glm, glut

```
sudo apt install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglm-dev
```

* GLFW, GLEW Dependency

```
sudo apt-get install dos2unix git wget xorg-dev
```
* Eigen3
```
sudo apt-get install libeigen3-dev
```

* Source of GLFW, GLEW (*Have included in third-party, no need to install*)
    * GLEW-2.1.0 (http://glew.sourceforge.net/index.html)
    * GLFW-3.3.4 (https://www.glfw.org/download.html)
    
## Vedio

I have upload two vedios in the /vedio folder. The vedio shows 15 UAVs launch from ground and head to the sphere. 
When all UAVs reach the sphere surface, they will begin to move around the sphere and a timer will start. 
If the timer has run more than 60 seconds and all UAVs are inside the sphere then the program will end automatically. 
You can find some info from terminal output such as UAV collison and final timer records.

## Reference

* https://www.opengl-tutorial.org/beginners-tutorials/
* https://github.com/opengl-tutorials/ogl
* https://www.glfw.org/docs/latest/quick_guide.html