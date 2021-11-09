//
// Created by allen on 11/9/21.
//

#ifndef GATECH_BUZZY_BOWL_MAIN_H
#define GATECH_BUZZY_BOWL_MAIN_H
#include "../include/ECE_UAV.h"

#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"
#include "../common/vboindexer.hpp"
#include "../common/controls.hpp"


// Screen
#define SCREEN_LEN 400
#define SCREEN_WID 400

// UAV Position
#define Y0 0.0
#define Y1 25.0
#define X0 0.0
#define X1 25.0
#define X2 50.0

// constant number
#define PI 3.1415926

// Camera position
#define CAM_Z 90
#define CAM_Y -100
#define CAM_X 0

// draw or not
#define DRAW_UAV
#define DRAW_FF
#define DRAW_SPHERE
#endif //GATECH_BUZZY_BOWL_MAIN_H
