//
// Created by allen on 11/9/21.
//

#ifndef GATECH_BUZZY_BOWL_MAIN_H
#define GATECH_BUZZY_BOWL_MAIN_H
#pragma once
#include "../include/ECE_UAV.h"

#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"
#include "../common/vboindexer.hpp"
#include "../common/controls.hpp"
#include <set>

// Screen
#define SCREEN_LEN 900
#define SCREEN_WID 500

// UAV Position
#define Y0 0.0
#define Y1 25.0
#define Y2 50.0
#define X0 0.0
#define X1 25.0
//#define X2 50.0


// Camera position
#define CAM_Z 60
#define CAM_Y -70
#define CAM_X 0

// draw or not
#define DRAW_UAV
#define DRAW_FF
#define DRAW_SPHERE

void collisionThreadFunc();
int oglThreadFunc();

#endif //GATECH_BUZZY_BOWL_MAIN_H
