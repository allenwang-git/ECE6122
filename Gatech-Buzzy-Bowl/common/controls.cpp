// Include GLFW
#pragma once
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "../include/main.h"
#include "controls.hpp"
#include <iostream>
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( CAM_X, CAM_Y, CAM_Z );
// Initial horizontal angle : toward -Z
float horizontalAngle = PI;
// Initial vertical angle : none
float verticalAngle = PI/3;
// Initial Field of View
float initialFoV = 50.0f;
glm::vec3 direction(0.f,0.9f,-0.4f);
float speed = 5.0f; // 3 units / second
float mouseSpeed = 0.0002f;

static void ScrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{

    initialFoV -= 5 * yoffset; // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
    if(!(initialFoV>0 && initialFoV<180))
        initialFoV+=5*yoffset;
//    std::cout << "Scroll: (" << initialFoV << "," << yoffset << ")" << endl;
}

void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, SCREEN_LEN/2, SCREEN_WID/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(SCREEN_LEN/2 - xpos );
	verticalAngle   += mouseSpeed * float( SCREEN_WID/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
    direction = glm::vec3 (
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - PI/2.0f),
		0,
		cos(horizontalAngle - PI/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
    glfwSetScrollCallback(window,ScrollCallback);
    float FoV = initialFoV;
//    std::cout<<FoV<<" "<<position.x<<" "<<position.y<<" "<<position.z<<" "<<direction.x<<" "<<direction.y<<" "<<direction.z<<std::endl;
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 500.0f);
	// Camera matrix  glm::vec3(0.f,0.f,0.f),//
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}