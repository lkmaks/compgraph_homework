#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

#include <iostream>
#include <time.h>

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 03 - Matrices", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);



    // Dark background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    // Create and compile our GLSL program from the shaders
    GLuint programID1 = LoadShaders( "VertexShader.vertexshader", "FragmentShader1.fragmentshader" );
    GLuint programID2 = LoadShaders( "VertexShader.vertexshader", "FragmentShader2.fragmentshader" );

    GLuint MatrixID1 = glGetUniformLocation(programID1, "MVP");
    GLuint MatrixID2 = glGetUniformLocation(programID2, "MVP");

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);

    glm::mat4 Model      = glm::mat4(1.0f);

    static const GLfloat triangle_data[] = {
         5.0f, -3.0f, 0.0f,
         0.0f, 2.0f, 0.0f,
         1.0f, 1.0f, 3.0f,
         0, 0, 0,
         3, 3, 4,
         7, 5, 1
    };


    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_data), triangle_data, GL_STATIC_DRAW);

    int iter = 0;
    float time_scale = 100;
    float Rx = 50;
    float Ry = 30;
    float t_start = -glm::pi<float>() / 2;

    do{

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        float t = iter / time_scale;
        glm::vec3 cur_pos = glm::vec3(Rx * glm::cos(t + t_start), Ry * glm::sin(t + t_start), 3);

        glm::mat4 cur_view = glm::lookAt(
                                    cur_pos,
                                    glm::vec3(2,2,3),
                                    glm::vec3(0,0,1)
                               );

        glm::mat4 cur_MVP = Projection * cur_view * Model;


        // Use shader 1
        glUseProgram(programID1);

        glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &cur_MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Use shader 2
        glUseProgram(programID2);

        glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &cur_MVP[0][0]);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)(sizeof(triangle_data) / 2)
        );

        glDrawArrays(GL_TRIANGLES, 0, 3);



        glDisableVertexAttribArray(0);

        iter += 1;

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        struct timespec tw = {0, 10000000};
        struct timespec tr;
        nanosleep(&tw, &tr);

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID1);
    glDeleteProgram(programID2);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

