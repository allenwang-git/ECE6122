#ifndef CONTROLS_HPP
#define CONTROLS_HPP
//#include "../include/main.h"

// Camera position
#define CAM_Z 60
#define CAM_Y -70
#define CAM_X 0
// pi
#define PI 3.1415926
// Screen
#define SCREEN_LEN 900
#define SCREEN_WID 500

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif