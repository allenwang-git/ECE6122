/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/25/2021

Description:
 This program use OpenGL to create a 3D game called "Gatech Buzzy Bowl",
 In this game, the user launches buzzy across the field attempting
 to hit the evil mascots from other universities, while avoiding
 the cute woodland creatures.

 Press Enter to start/restart the game, press Space to modify the buzzy's
 initial velocity and press up/down to modify buzzy's initial direction.
 When releasing the Space, the buzzy will be launched.

 If user win or lose the game, the page will reset to the initial page.
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"
#include "../common/texture.hpp"
//#include "../common/objloader.hpp"
//#include "../common/"
#include "../include/ECE_UAV.h"

using namespace std;
using namespace glm;
#define Y0 0.0
#define Y1 25.0
#define Y2 50.0
#define X0 0.0
#define X1 25.0
thread uavs[15];
//vector<thread> uavs;
bool createUAV()
{
//    for (int i = 0; i < 15; ++i)
//    {
//        if(i<3)
//        {
//            ECE_UAV uav1(25*(i-1),0);
//            uavs[i] = move(uav1.thuav);
//            uavs.push_back(uav.thuav);
//        }
//        else if(i>=3 && i <6)
//        {
//            ECE_UAV uav2(25*(i-4),25);uavs[i] = move(uav2.thuav);
////            uavs.push_back(&uav);
//        }
//        else if(i>=6 && i <9)
//        {
//            ECE_UAV uav3(25*(i-7),-25);uavs[i] = move(uav3.thuav);
////            uavs.push_back(&uav);
//        }
//        else if(i>=9 && i <12)
//        {
//            ECE_UAV uav4(25*(i-10),50);uavs[i] = move(uav4.thuav);
////            uavs.push_back(&uav);
//        }
//        else if(i>=12)
//        {
//            ECE_UAV uav5(25*(i-13),-50);uavs[i] = move(uav5.thuav);
////            uavs.push_back(&uav);
//        }
//        uavs[i] = move(uav.thuav);
//    }

}

int main(void)
{

//    createUAV();
    ECE_UAV uav_1(0,0);
    ECE_UAV uav_2(0,Y1);
    ECE_UAV uav_3(0,-Y1);
    ECE_UAV uav_4(0,Y2);
    ECE_UAV uav_5(0,-Y2);

    ECE_UAV uav_6(X1,0);
    ECE_UAV uav_7(X1,Y1);
    ECE_UAV uav_8(X1,-Y1);
    ECE_UAV uav_9(X1,Y2);
    ECE_UAV uav_10(X1,-Y2);

    ECE_UAV uav_11(-X1,0);
    ECE_UAV uav_12(-X1,Y1);
    ECE_UAV uav_13(-X1,-Y1);
    ECE_UAV uav_14(-X1,Y2);
    ECE_UAV uav_15(-X1,-Y2);
    uav_1.join();
    uav_2.join();
    uav_3.join();
    uav_4.join();
    uav_5.join();
    uav_6.join();
    uav_7.join();
    uav_8.join();
    uav_9.join();
    uav_10.join();
    uav_11.join();
    uav_12.join();
    uav_13.join();
    uav_14.join();
    uav_15.join();
//    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
/*    for (auto uav:uavs)
    {
        uav->start();
    }
    for (int i =0;i<3;i++)
    {
        uavs[i].join();

    }*/
    /* ******************************************************************************
    * Initialization
    * *******************************************************************************/

    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    // Init Configuration
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1024, 512, "Playground", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);// Set window context as current thread context

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    /* **************************************************************************
     * Shaders and VAO, VBP, EBO
     * **************************************************************************/

    static const GLfloat g_vertex_buffer_data[] =
            {
                //Normalized Device Coordinates
                //x      y   depth   texture
                -1.f,-0.5f, 0.0f,  0.06f, 0.f,  // left bottom
                1.f, -0.5f, 0.0f,  1.0f,0.f,    // right bottom
                1.f,  0.5f, 0.0f,  1.f, 1.f,    // right top
                -1.f, 0.5f, 0.0f,  0.06f, 1.f   // left top
            };
    GLuint indices[] =
            {
                0,1,2, // first triangle
                3,0,2  // second triangle
            };
    // Create and compile our GLSL program from the shaders
    GLuint shaderProgram = LoadShaders( "VertexShader.vertexshader",
                                        "FragmentShader.fragmentshader" );
    // Load the texture using any two methods
    GLuint texture = loadBMP_custom("../ff.bmp");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(shaderProgram, "ffTexture");
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create and bind VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // generate a VBO with ID=1
    GLuint VBO;
    glGenBuffers(1, &VBO);
    //  set buffer type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //  copy vertex data into buffer(graphics card RAM)
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Create EBO and bind to buffer
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(0,-1,3), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
    // 1st attribute buffer : vertices
    glVertexAttribPointer(
            0,                  // attribute no.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            5*sizeof(float),   // stride
            (void*)0           // array buffer offset
    );
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    // 2nd attribute buffer
    glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            5*sizeof(float),
            (void*)(3*sizeof(float))
    );
    glEnableVertexAttribArray(1);

    /* **********************************************************************************
     * Draw something
     * ************************************************************************************/
    do//render loop
    {
        // Dark blue background
        glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
        // clear screen before a new render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Use our shader
        glUseProgram(shaderProgram);

        // Send our transformation to the currently bound shader,in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        // Bind our texture in texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);


        // Draw the triangle !
//        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // Swap buffers-- draw new screen
        glfwSwapBuffers(window);
        // check events(key,mouse,window)
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture);
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

