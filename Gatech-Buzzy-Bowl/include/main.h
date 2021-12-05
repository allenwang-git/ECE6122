/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 11/25/2021

Description:
 This is the header of main.cpp to define some useful constants and include header files
*/
#ifndef GATECH_BUZZY_BOWL_MAIN_H
#define GATECH_BUZZY_BOWL_MAIN_H

#include "../include/ECE_UAV.h"

#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"
#include "../common/vboindexer.hpp"
#include "../common/controls.hpp"
#include <set>
#include <chrono>

// UAV Position
#define Y0 0.0
#define Y1 25.0
#define Y2 50.0
#define X0 0.0
#define X1 25.0

// draw or not
#define DRAW_UAV
#define DRAW_FF
#define DRAW_SPHERE

// run time
#define RUN_TIME 60.0

void collisionThreadFunc();
int oglThreadFunc();
bool checkAllIn();
bool checkUavCollision(Vector3d posA, Vector3d posB);
// flag to end the opengl and collision threads
bool endThreadFlag = false;
const Vector3d sphereOrigin = {0., 0., 50.};
#endif //GATECH_BUZZY_BOWL_MAIN_H
