/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 11/25/2021

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
#include "../include/main.h"

using namespace std;
using namespace glm;
GLFWwindow* window;
vector<Vector3f*> UAVs;

int main()
{
    thread ogl(oglThreadFunc);

    //  create UAV
/*    ECE_UAV uav_1(X0,Y0);
    ECE_UAV uav_2(X0,Y1);
    ECE_UAV uav_3(X0,-Y1);

    ECE_UAV uav_4(X1,Y0);
    ECE_UAV uav_5(X1,-Y1);
    ECE_UAV uav_6(X1,Y1);

    ECE_UAV uav_7(X2,Y1);
    ECE_UAV uav_8(X2,-Y1);
    ECE_UAV uav_9(X2,Y0);

    ECE_UAV uav_10(-X1,-Y1);
    ECE_UAV uav_11(-X1,Y0);
    ECE_UAV uav_12(-X1,Y1);

    ECE_UAV uav_13(-X2,Y0);
    ECE_UAV uav_14(-X2,Y1);
    ECE_UAV uav_15(-X2,-Y1);*/
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

    uav_1.start();
    uav_2.start();
    uav_3.start();
    uav_4.start();
    uav_5.start();
    uav_6.start();
    uav_7.start();
    uav_8.start();
    uav_9.start();
    uav_10.start();
    uav_11.start();
    uav_12.start();
    uav_13.start();
    uav_14.start();
    uav_15.start();

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

    ogl.join();

    return -1;
}
int oglThreadFunc()//vector<Vector3f*> uavPosition)
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
    window = glfwCreateWindow(SCREEN_LEN, SCREEN_WID, "Playground", NULL, NULL);
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
    // Cull triangles which normal is not towards the camera
//    glEnable(GL_CULL_FACE);

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
    std::vector<glm::vec3> vnUAV; // Won't be used at the moment.
//    bool res = loadOBJ("../OBJ files/shipA_OBJ/shipA_OBJ.obj", vUAV, vtUAV, vnUAV);
    loadOBJ("../OBJ files/shipA_OBJ/ship2.obj", vUAV, vtUAV, vnUAV);
    // Load the texture
    GLuint texture2 = loadBMP_custom("../img.bmp");
    GLuint uavTexture  = glGetUniformLocation(shaderProgramUAV, "UAVTexture");
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    std::vector<unsigned short> indices;
//    std::vector<glm::vec3> indexed_vertices;
//    std::vector<glm::vec2> indexed_uvs;
//    std::vector<glm::vec3> indexed_normals;
//    indexVBO(vUAV, vtUAV, vnUAV, indices, indexed_vertices, indexed_uvs, indexed_normals);

    // Load it into a VBO
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vUAV.size() * sizeof(glm::vec3), &vUAV[0], GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, vtUAV.size() * sizeof(glm::vec3), &vtUAV[0], GL_STATIC_DRAW);

//    GLuint vnBuffer;
//    glGenBuffers(1, &vnBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vnBuffer);
//    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
//    GLuint elementBuffer;
//    glGenBuffers(1, &elementBuffer);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

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
//                -0.5f,-1.f, 0.0f,    // left bottom
//                -0.5f,1.f,  0.0f,    // right bottom
//                0.5f,1.f,   0.0f,    // right top
//                0.5f,1.f,   0.0f,    // right top
//                0.5f,-1.f,  0.0f,    // left top
//                -0.5f,-1.f, 0.0f     // left bottom
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
//    GLuint ff_indices[] =
//            {
//                0,1,2, // first triangle
//                3,0,2  // second triangle
//            };
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create and bind VAO
//    GLuint VAO;
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);

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
    // Create EBO and bind to buffer
//    GLuint EBO;
//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ff_indices), ff_indices, GL_STATIC_DRAW);
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
//    double lastTime = glfwGetTime();
    do//render loop
    {
//        double currentTime = glfwGetTime();
////        float deltaTime = (float);
//        if(abs(round((currentTime - lastTime)*100)-30.f)<15.f)
//        {
//
//            cout<<currentTime-lastTime<<endl;
//            lastTime = currentTime;
//        }

        // Dark blue background
        glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
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
            glUniform1i(uavTexture, 0);

            glm::vec3 gOrientation(PI/2, 0.f, 0.f);
            for (auto u:UAVs)
            {
//                    cout<<"pos:"<<(*u)[1]<<" "<<(*u)[0]<<" "<<(*u)[2]<<endl;
//                    glm::vec3 gPosition(-50.f + j*25.f, -25.f + i*25.f, 2.f);
                    glm::vec3 gPosition((*u)[1],(*u)[0],(*u)[2]);
                    // Build the model matrix
                    glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.f), gOrientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
                    glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0), gPosition); // A bit to the left
                    glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.06f, 0.06f, 0.05f));
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

/*            // Index buffer
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

            // 3rd attribute buffer : normals
//    glEnableVertexAttribArray(2);
//    glBindBuffer(GL_ARRAY_BUFFER, vnBuffer);
//    glVertexAttribPointer(
//            2,                                // attribute
//            3,                                // size
//            GL_FLOAT,                         // type
//            GL_FALSE,                         // normalized?
//            0,                                // stride
//            (void*)0                          // array buffer offset
//    );
//            glBindVertexArray(vertexArray);
//            // Draw the UAV
//            glDrawElements(
//                    GL_TRIANGLES,      // mode
//                    indices.size(),    // count
//                    GL_UNSIGNED_SHORT,   // type
//                    (void*)0           // element array buffer offset
//            );*/

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
            glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(62.0f, 60.0f, 1.0f));
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
            // Use  shader
            glUseProgram(shaderProgramSPH);
            // Model matrix
            glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 50.f));
            glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
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
        }
#endif
        // Swap buffers-- draw new screen
        glfwSwapBuffers(window);
        // check events(key,mouse,window)
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(30));
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

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

