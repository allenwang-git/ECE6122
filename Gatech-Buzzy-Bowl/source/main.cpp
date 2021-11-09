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
#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/euler_angles.hpp>

#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"
#include "../common/vboindexer.hpp"
#include "../include/ECE_UAV.h"

using namespace std;
using namespace glm;
#define Y0 0.0
#define Y1 25.0
#define Y2 50.0
#define X0 0.0
#define X1 25.0
#define DRAW_UAV
//#define DRAW_CUBE
#define DRAW_FF

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
    // Enable depth test
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
    std::vector<glm::vec3> vnUAV; // Won't be used at the moment.
    bool res = loadOBJ("../OBJ files/shipA_OBJ/shipA_OBJ.obj", vUAV, vtUAV, vnUAV);
    // Load the texture
    GLuint texture2 = loadDDS("../uvmap.DDS");
    GLuint uavTexture  = glGetUniformLocation(shaderProgramUAV, "UAVTexture");

    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    indexVBO(vUAV, vtUAV, vnUAV, indices, indexed_vertices, indexed_uvs, indexed_normals);

    // Load it into a VBO
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    GLuint vnBuffer;
    glGenBuffers(1, &vnBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vnBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

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
                //x      y   depth   texture
                -1.f,-0.5f, 0.0f,  0.06f, 0.f,  // left bottom
                1.f, -0.5f, 0.0f,  1.0f,0.f,    // right bottom
                1.f,  0.5f, 0.0f,  1.f, 1.f,    // right top
                -1.f, 0.5f, 0.0f,  0.06f, 1.f   // left top
            };
    GLuint ff_indices[] =
            {
                0,1,2, // first triangle
                3,0,2  // second triangle
            };
    // Create and compile our GLSL program from the shaders
    GLuint shaderProgram = LoadShaders( "ffVertexShader.vertexshader",
                                        "ffFragmentShader.fragmentshader" );
    /* **********************************************************************************
 * Camera, position, rotation
 * ************************************************************************************/
    // Get a handle for our "MVP" uniform
    GLuint FFMatrixID = glGetUniformLocation(shaderProgram, "FF_MVP");
    // Load the texture using any two methods
//    GLuint texture1 = loadBMP_custom("../ff.bmp");
//    // Get a handle for our "myTextureSampler" uniform
//    GLuint ffTexture  = glGetUniformLocation(shaderProgram, "ffTexture");
//    // set the texture wrapping parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    // set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

    // Create EBO and bind to buffer
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ff_indices), ff_indices, GL_STATIC_DRAW);



#endif
    // Projection matrix : 45D Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f),
                                                      4.0f / 3.0f, 0.1f, 1000.0f);
    // Camera matrix
    glm::mat4 ViewMatrix = glm::lookAt(
            glm::vec3(0,-9,9), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

#ifdef DRAW_CUBE
    // Everything here comes from Tutorial 4
	GLuint cubeProgramID = LoadShaders( "./TransformVertexShader.vertexshader", "./ColorFragmentShader.fragmentshader" );
	GLuint cubeMatrixID = glGetUniformLocation(cubeProgramID, "MVP");
	static const GLfloat g_cube_vertex_buffer_data[] = { -1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f, 1.0f, 1.0f,1.0f, 1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f,1.0f,-1.0f, 1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f, 1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f,1.0f,-1.0f, 1.0f,1.0f, 1.0f, 1.0f,1.0f,-1.0f,-1.0f,1.0f, 1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f, 1.0f, 1.0f,1.0f,-1.0f, 1.0f,1.0f, 1.0f, 1.0f,1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f,1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f,-1.0f, 1.0f, 1.0f,1.0f,-1.0f, 1.0f};
	static const GLfloat g_cube_color_buffer_data[] = { 0.583f,  0.771f,  0.014f,0.609f,  0.115f,  0.436f,0.327f,  0.483f,  0.844f,0.822f,  0.569f,  0.201f,0.435f,  0.602f,  0.223f,0.310f,  0.747f,  0.185f,0.597f,  0.770f,  0.761f,0.559f,  0.436f,  0.730f,0.359f,  0.583f,  0.152f,0.483f,  0.596f,  0.789f,0.559f,  0.861f,  0.639f,0.195f,  0.548f,  0.859f,0.014f,  0.184f,  0.576f,0.771f,  0.328f,  0.970f,0.406f,  0.615f,  0.116f,0.676f,  0.977f,  0.133f,0.971f,  0.572f,  0.833f,0.140f,  0.616f,  0.489f,0.997f,  0.513f,  0.064f,0.945f,  0.719f,  0.592f,0.543f,  0.021f,  0.978f,0.279f,  0.317f,  0.505f,0.167f,  0.620f,  0.077f,0.347f,  0.857f,  0.137f,0.055f,  0.953f,  0.042f,0.714f,  0.505f,  0.345f,0.783f,  0.290f,  0.734f,0.722f,  0.645f,  0.174f,0.302f,  0.455f,  0.848f,0.225f,  0.587f,  0.040f,0.517f,  0.713f,  0.338f,0.053f,  0.959f,  0.120f,0.393f,  0.621f,  0.362f,0.673f,  0.211f,  0.457f,0.820f,  0.883f,  0.371f,0.982f,  0.099f,  0.879f};
	GLuint cubevertexbuffer;
	glGenBuffers(1, &cubevertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubevertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_vertex_buffer_data), g_cube_vertex_buffer_data, GL_DYNAMIC_DRAW);
	GLuint cubecolorbuffer;
	glGenBuffers(1, &cubecolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_color_buffer_data), g_cube_color_buffer_data, GL_DYNAMIC_DRAW);
#endif
    /* **********************************************************************************
     * Draw something
     * ************************************************************************************/
    do//render loop
    {
        // Dark blue background
        glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
        // clear screen before a new render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef DRAW_CUBE
        // Again : this is just Tutorial 4 !
        glDisable(GL_BLEND);
        glUseProgram(cubeProgramID);
        glm::mat4 cubeModelMatrix(1.0f);
        cubeModelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 2.f, 2.f));
        glm::mat4 cubeMVP = ProjectionMatrix * ViewMatrix * cubeModelMatrix;
        glUniformMatrix4fv(cubeMatrixID, 1, GL_FALSE, &cubeMVP[0][0]);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubevertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0  );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT,  GL_FALSE, 0, (void*)0   );
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
#endif
        /* **********************************************************************************
         * Draw UAVs
         * ************************************************************************************/
#ifdef DRAW_UAV
        {
            // Use  shader
            glUseProgram(shaderProgramUAV);

            // Build the model matrix
            glm::mat4 RotationMatrix = glm::mat4(10.0f);//eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
            glm::mat4 TranslationMatrix = glm::mat4(10.0f);//translate(mat4(), gPosition1); // A bit to the left
            glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
            glm::mat4 ModelMatrix =  ScalingMatrix;//TranslationMatrix * RotationMatrix *
            glm::mat4 UAV_MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            // Send our transformation to the currently bound shader,in the "MVP" uniform
            glUniformMatrix4fv(UAVMatrixID, 1, GL_FALSE, &UAV_MVP[0][0]);
            // Bind our texture in texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture2);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(uavTexture, 0);
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
            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
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
            glBindVertexArray(vertexArray);
            // Draw the UAV
            glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
            );
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
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
//            glm::mat4 RotationMatrix = mat4_cast(gOrientation2);
//            glm::mat4 TranslationMatrix = translate(mat4(), gPosition2); // A bit to the right
            glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(60.0f, 60.0f, 1.0f));
            glm::mat4 ModelMatrix =  ScalingMatrix;
            // Our ModelViewProjection : multiplication of our 3 matrices
            glm::mat4 FF_MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around

            // Send our transformation to the currently bound shader,in the "MVP" uniform
            glUniformMatrix4fv(FFMatrixID, 1, GL_FALSE, &FF_MVP[0][0]);
            // Bind our texture in texture Unit 0
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, texture1);
//            // Set our "myTextureSampler" sampler to use Texture Unit 0
//            glUniform1i(ffTexture, 0);

            // 1st attribute buffer : vertices
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                    0,                  // attribute no.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    5*sizeof(float),   // stride
                    (void*)0           // array buffer offset
            );

            // 2nd attribute buffer
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                    1,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    5*sizeof(float),
                    (void*)(3*sizeof(float))
            );

//            glBindVertexArray(VAO);
            // Draw the triangle !
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }
#endif
        // Swap buffers-- draw new screen
        glfwSwapBuffers(window);
        // check events(key,mouse,window)
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // optional: de-allocate all resources once they've outlived their purpose:
//    glDeleteVertexArrays(1, &VAO);
#ifdef DRAW_FF
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
//    glDeleteTextures(1, &texture1);
#endif
#ifdef DRAW_UAV
    //uav delete
    glDeleteProgram(shaderProgramUAV);
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteTextures(1, &texture2);
#endif
//    glDeleteBuffers(1, &unBuffer);
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

