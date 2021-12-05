/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 11/25/2021

Description:
 This program use OpenGL to create a 3D multi-thread simulation called "Gatech Buzzy Bowl".
 15 UAVs in the playground will be launched and move around a 10m sphere in the sky for 60 seconds.
 Once they reach the sphere surface, they will begin to move randomly on the surface and
 all UAVs will collide with each other if they are too close to each other.
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/main.h"

using namespace std;
using namespace glm;
using namespace chrono;

GLFWwindow* window;
vector<Vector3d*> UAVs;
chrono::high_resolution_clock::time_point allReachSphereTimestamp;

int main()
{
    // create the opengl thread
    thread oglThread(oglThreadFunc);

    //  create UAV
    ECE_UAV uav_1(X0, Y0);
    ECE_UAV uav_2(X0, Y1);
    ECE_UAV uav_3(X0, -Y1);
    ECE_UAV uav_4(X0, Y2);
    ECE_UAV uav_5(X0, -Y2);
    ECE_UAV uav_6(X1, Y0);
    ECE_UAV uav_7(X1, Y1);
    ECE_UAV uav_8(X1, -Y1);
    ECE_UAV uav_9(X1, Y2);
    ECE_UAV uav_10(X1, -Y2);
    ECE_UAV uav_11(-X1, Y0);
    ECE_UAV uav_12(-X1, Y1);
    ECE_UAV uav_13(-X1, -Y1);
    ECE_UAV uav_14(-X1, Y2);
    ECE_UAV uav_15(-X1, -Y2);

    // add uav position
    UAVs.push_back(uav_1.getPosition());
    UAVs.push_back(uav_2.getPosition());
    UAVs.push_back(uav_3.getPosition());
    UAVs.push_back(uav_4.getPosition());
    UAVs.push_back(uav_5.getPosition());
    UAVs.push_back(uav_6.getPosition());
    UAVs.push_back(uav_7.getPosition());
    UAVs.push_back(uav_8.getPosition());
    UAVs.push_back(uav_9.getPosition());
    UAVs.push_back(uav_10.getPosition());
    UAVs.push_back(uav_11.getPosition());
    UAVs.push_back(uav_12.getPosition());
    UAVs.push_back(uav_13.getPosition());
    UAVs.push_back(uav_14.getPosition());
    UAVs.push_back(uav_15.getPosition());

    // handle the uav collision
    thread colThread(collisionThreadFunc);
    bool timerFlag = false;
    double timeTillNow = 0.;
    while(true)
    {
        // set the start timestamp and turn on the timer
        if(!timerFlag && uav_1.getInitSphereFlag() && uav_2.getInitSphereFlag() && uav_3.getInitSphereFlag() &&
                uav_4.getInitSphereFlag() && uav_5.getInitSphereFlag() && uav_6.getInitSphereFlag() &&
                uav_7.getInitSphereFlag() && uav_8.getInitSphereFlag() && uav_9.getInitSphereFlag() &&
                uav_10.getInitSphereFlag() && uav_11.getInitSphereFlag() && uav_12.getInitSphereFlag() &&
                uav_13.getInitSphereFlag() && uav_14.getInitSphereFlag() && uav_15.getInitSphereFlag())
        {
            timerFlag = true;
            allReachSphereTimestamp = high_resolution_clock::now();
            cout<<"[INFO] All UAVs have arrived the surface, activate the timer."<<endl;
        }
        // check the timer
        timeTillNow = duration_cast<duration<double>>(high_resolution_clock::now() - allReachSphereTimestamp).count();
        if(timerFlag && timeTillNow >= RUN_TIME && checkAllIn())
        {
            cout<<"[INFO] All UAVs are inside the sphere, program ends "<<timeTillNow<<" seconds after activating the timer."<<endl;
            uav_1.setEndFlag(); uav_2.setEndFlag(); uav_3.setEndFlag();
            uav_4.setEndFlag(); uav_5.setEndFlag(); uav_6.setEndFlag();
            uav_7.setEndFlag(); uav_8.setEndFlag(); uav_9.setEndFlag();
            uav_10.setEndFlag(); uav_11.setEndFlag(); uav_12.setEndFlag();
            uav_13.setEndFlag(); uav_14.setEndFlag(); uav_15.setEndFlag();
            endThreadFlag = true;
            break;
        }
        // set the update rate
        this_thread::sleep_for(chrono::milliseconds(30));
    }

    oglThread.join();
    colThread.join();
    return -1;

}
/*
 * Check if the uav collide with others
 * @return true if a uav collides with others
 * */
bool checkUavCollision(Vector3d posA, Vector3d posB)
{
    double distance = 0.;
    for (int i = 0; i < 3; ++i)
    {
        distance += ((posA[i] - posB[i]) * (posA[i] - posB[i]));
    }
    if(distance>0.)
    {
        distance = sqrt(distance);
    }
    if(distance<=0.2*2)
    {
        return true;
    }
    else
    {
        return false;
    }

}

/*
 * Check if all uavs have gone into the sphere
 * @return true if all uavs are in the sphere
 * */
bool checkAllIn()
{
    for (auto u:UAVs)
    {
        double distance = 0.;
        double bias = 0.5;
        for (int i = 0; i < 3; ++i)
        {
            distance += (((*u)[i] - sphereOrigin[i]) * ((*u)[i] - sphereOrigin[i]));
        }
        if (distance > 0.)
            distance = sqrt(distance);
        if (distance>10.0+bias)
            return false;
    }
    return true;
}
/*
 * The thread function to handle the collision between uavs
 *
 * */
void collisionThreadFunc()
{
    unsigned long timer =0;
    set<int> collisionSet;
    do
    {
        timer++;
        if (timer%500==0)
            collisionSet.clear();
        for (int i = 0; i < 15; ++i)
        {
            for (int j = 0; j < 15; ++j)
            {
                if(i!=j && collisionSet.find(i)==collisionSet.end() && collisionSet.find(j)==collisionSet.end()
                   && checkUavCollision(*UAVs[i],*UAVs[j]))
                {
                    cout<<"[INFO] UAV-"<<i+1<<" collides with UAV-"<<j+1<<"."<<endl;
                    Vector3d posTmp = *UAVs[i];
                    *UAVs[i] = *UAVs[j];
                    *UAVs[j] = posTmp;
                    collisionSet.insert(i);
                    collisionSet.insert(j);
                }
            }
        }
        // set the update rate
        this_thread::sleep_for(chrono::milliseconds(30));
    }
    while(!endThreadFlag);
    return;
}
/*
 * The thread function to draw the playground and uavs and update uavs' position
 * */
int oglThreadFunc()
{
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
    window = glfwCreateWindow(SCREEN_LEN, SCREEN_WID, "Gatech Bowl - Yinuo Wang", NULL, NULL);
    glfwSetWindowPos(window,300,300);
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
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, SCREEN_LEN/2, SCREEN_WID/2);

    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // create and bind VAO
    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
#ifdef DRAW_UAV
    /* **************************************************************************
     * Shaders and VAO, VBP, EBO of UAVs
     * **************************************************************************/
    // Create and compile our GLSL program from the shaders
    GLuint shaderProgramUAV = LoadShaders( "uavVertexShader.vertexshader",
                                           "uavFragmentShader.fragmentshader" );
    // Load our .obj file
    std::vector<glm::vec3> vUAV;
    std::vector<glm::vec2> vtUAV;
    std::vector<glm::vec3> vnUAV; // Won't be used at the moment
    loadOBJ("../OBJ files/shipA_OBJ/ship2.obj", vUAV, vtUAV, vnUAV);
    // Load the texture
    GLuint texture2 = loadBMP_custom("../img.bmp");
    GLuint uavTexture  = glGetUniformLocation(shaderProgramUAV, "UAVTexture");
    GLuint uavColorFactor = glGetUniformLocation(shaderProgramUAV, "UAVColorFactor");

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load it into a VBO
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vUAV.size() * sizeof(glm::vec3), &vUAV[0], GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, vtUAV.size() * sizeof(glm::vec3), &vtUAV[0], GL_STATIC_DRAW);

    // Get a handle for our "MVP" uniform
    GLuint UAVMatrixID = glGetUniformLocation(shaderProgramUAV, "UAV_MVP");

#endif
#ifdef DRAW_FF
    /* **************************************************************************
     * Shaders and VAO, VBP, EBO of football field
     * **************************************************************************/

    static const GLfloat ff_vertex_buffer_data[] =
            {
                //Normalized Device Coordinates
                //x      y   depth
                -1.f,-0.5f, 0.0f,    // left bottom
                1.f, -0.5f, 0.0f,    // right bottom
                1.f,  0.5f, 0.0f,    // right top
                1.f,  0.5f, 0.0f,    // right top
                -1.f, 0.5f, 0.0f,    // left top
                -1.f,-0.5f, 0.0f     // left bottom
            };
    static const GLfloat ff_uv_buffer_data[] =
            {   // UV Coordinate
                    0.06f, 0.f,
                    1.0f,0.f,
                    1.f, 1.f,
                    1.f, 1.f,
                    0.06f, 1.f,
                    0.06f, 0.f
            };
    // Create and compile our GLSL program from the shaders
    GLuint shaderProgram = LoadShaders( "ffVertexShader.vertexshader",
                                        "ffFragmentShader.fragmentshader" );
    // Get a handle for our "MVP" uniform
    GLuint FFMatrixID = glGetUniformLocation(shaderProgram, "FF_MVP");
    // Load the texture using any two methods
    GLuint texture1 = loadBMP_custom("../ff.bmp");
    // Get a handle for our "myTextureSampler" uniform
    GLuint ffTexture  = glGetUniformLocation(shaderProgram, "ffTexture");
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // generate a VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    //  set buffer type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //  copy vertex data into buffer(graphics card RAM)
    glBufferData(GL_ARRAY_BUFFER, sizeof(ff_vertex_buffer_data), ff_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint UV;
    glGenBuffers(1, &UV);
    glBindBuffer(GL_ARRAY_BUFFER, UV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ff_uv_buffer_data), ff_uv_buffer_data, GL_STATIC_DRAW);

#endif
#ifdef DRAW_SPHERE
    /* **************************************************************************
     * Shaders and VAO, VBP, EBO of SPHERE
     * **************************************************************************/

    // Create and compile our GLSL program from the shaders
    GLuint shaderProgramSPH = LoadShaders( "SphereVertexShader.vertexshader",
                                           "SphereFragmentShader.fragmentshader" );
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load our .obj file
    std::vector<glm::vec3> vSPH;
    loadNoUVOBJ("../OBJ files/sphere.obj", vSPH);
    // Get a handle for our "MVP" uniform
    GLuint SPHMatrixID = glGetUniformLocation(shaderProgramSPH, "SPH_MVP");
    // Load it into a VBO
    GLuint vSphereBuffer;
    glGenBuffers(1, &vSphereBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vSphereBuffer);
    glBufferData(GL_ARRAY_BUFFER, vSPH.size() * sizeof(glm::vec3), &vSPH[0], GL_STATIC_DRAW);

#endif
    /* **********************************************************************************
     * Draw something
     * ************************************************************************************/

    do//render loop
    {
        // Dark background
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        // clear screen before a new render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* **********************************************************************************
        * Camera Projection Matrix, View Matrix
        * ************************************************************************************/
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();

#ifdef DRAW_UAV
        /* **********************************************************************************
         * Draw UAVs
         * ************************************************************************************/
        {
            // Use  shader
            glUseProgram(shaderProgramUAV);
            // Bind our texture in texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture2);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            float timeValue = glfwGetTime();
            float colorFactor =  0.5 * sin(timeValue) + 1;//0.5 -1.5
            glUniform1i(uavTexture, 0);
            glUniform1f(uavColorFactor,colorFactor);

            glm::vec3 gOrientation(PI/2, 0., 0.);
            for (auto u:UAVs)
            {
                    glm::vec3 gPosition((*u)[1],(*u)[0],(*u)[2]);
                    // Build the model matrix
                    glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.), gOrientation.x, glm::vec3(1.0, 0.0, 0.0));
                    glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0), gPosition); // A bit to the left
                    glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.06, 0.06, 0.05));
                    glm::mat4 ModelMatrix =  TranslationMatrix * RotationMatrix * ScalingMatrix;//
                    glm::mat4 UAV_MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
                    // Send our transformation to the currently bound shader,in the "MVP" uniform
                    glUniformMatrix4fv(UAVMatrixID, 1, GL_FALSE, &UAV_MVP[0][0]);

                    // 1st attribute buffer : vertices
                    glEnableVertexAttribArray(0);
                    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                    glVertexAttribPointer(
                            0,                  // attribute
                            3,                  // size
                            GL_FLOAT,           // type
                            GL_FALSE,           // normalized?
                            0,                  // stride
                            (void*)0            // array buffer offset
                    );
                    // 2nd attribute buffer : UVs
                    glEnableVertexAttribArray(1);
                    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
                    glVertexAttribPointer(
                            1,                                // attribute
                            2,                                // size
                            GL_FLOAT,                         // type
                            GL_FALSE,                         // normalized?
                            0,                                // stride
                            (void*)0                          // array buffer offset
                    );
                    // Draw UAVs
                    glDrawArrays(GL_TRIANGLES, 0, vUAV.size());
                    glDisableVertexAttribArray(0);
                    glDisableVertexAttribArray(1);
            }
        }
#endif
#ifdef DRAW_FF
        /* **********************************************************************************
         * Draw football field
         * ************************************************************************************/
        {
            glDisable(GL_BLEND);
            // Use  shader
            glUseProgram(shaderProgram);

            // Model matrix
            glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0), glm::vec3(62.0, 60.0, 1.0));
            glm::mat4 ModelMatrix = ScalingMatrix;
            // Our ModelViewProjection : multiplication of our 3 matrices
            glm::mat4 FF_MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around

            // Send our transformation to the currently bound shader,in the "MVP" uniform
            glUniformMatrix4fv(FFMatrixID, 1, GL_FALSE, &FF_MVP[0][0]);
            // Bind our texture in texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(ffTexture, 0);

            // 1st attribute buffer : vertices
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                    0,                  // attribute no.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    3*sizeof(float),   // stride
                    (void*)0           // array buffer offset
            );

            // 2nd attribute buffer
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, UV);
            glVertexAttribPointer(
                    1,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    2*sizeof(float),
                    (void*)(0*sizeof(float))
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }
#endif
#ifdef DRAW_SPHERE
        /* **********************************************************************************
         * Draw Sphere
         * ************************************************************************************/
        {
            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Use  shader
            glUseProgram(shaderProgramSPH);
            // Model matrix
            glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0., 0., 50.));
            glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0), glm::vec3(10., 10., 10.));
            glm::mat4 ModelMatrix = TranslationMatrix * ScalingMatrix;
            // Our ModelViewProjection : multiplication of our 3 matrices
            glm::mat4 SPH_MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around
            // Send our transformation to the currently bound shader,in the "MVP" uniform
            glUniformMatrix4fv(SPHMatrixID, 1, GL_FALSE, &SPH_MVP[0][0]);
            // 1st attribute buffer : vertices
            glBindBuffer(GL_ARRAY_BUFFER, vSphereBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                    0,                  // attribute no.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    3 * sizeof(float),   // stride
                    (void *) 0           // array buffer offset
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, vSPH.size());
            glDisableVertexAttribArray(0);
            glDisable(GL_BLEND);
        }
#endif
        // Swap buffers-- draw new screen
        glfwSwapBuffers(window);
        // check events(key,mouse,window)
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(30));
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 && !endThreadFlag);

#ifdef DRAW_FF
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture1);
#endif
#ifdef DRAW_UAV
    //uav delete
    glDeleteProgram(shaderProgramUAV);
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteTextures(1, &texture2);
#endif
#ifdef DRAW_SPHERE
    glDeleteBuffers(1, &vSphereBuffer);
    glDeleteProgram(shaderProgramSPH);
#endif
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

