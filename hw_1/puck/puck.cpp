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



    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    GLuint programID = LoadShaders( "VertexShader.vertexshader", "FragmentShader.fragmentshader" );


    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);

    glm::mat4 Model      = glm::mat4(1.0f);



    int N_perimeter = 200;
    float H = 5;
    float R = 10;
    glm::vec3 center(0, 0, 0);

    glm::vec3 top_points[N_perimeter];
    glm::vec3 bot_points[N_perimeter];

    float PI = glm::pi<float>();
    float angle = 2 * PI / N_perimeter;

    for (int i = 0; i < N_perimeter; ++i) {
        top_points[i] = center + glm::vec3(R * glm::cos(angle * i), R * glm::sin(angle * i), H);
        bot_points[i] = top_points[i];
        bot_points[i].z = 0;
    }

    int N_triangles = (N_perimeter - 2) * 2 + 2 * N_perimeter;
    GLfloat triangles[N_triangles * 9];
    GLfloat colors[N_triangles * 9];

    int triangle_id = 0;
    for (int i = 1; i < N_perimeter - 1; ++i) {
        triangles[triangle_id * 9] = top_points[0].x;
        triangles[triangle_id * 9 + 1] = top_points[0].y;
        triangles[triangle_id * 9 + 2] = top_points[0].z;

        triangles[triangle_id * 9 + 3] = top_points[i].x;
        triangles[triangle_id * 9 + 4] = top_points[i].y;
        triangles[triangle_id * 9 + 5] = top_points[i].z;

        triangles[triangle_id * 9 + 6] = top_points[i + 1].x;
        triangles[triangle_id * 9 + 7] = top_points[i + 1].y;
        triangles[triangle_id * 9 + 8] = top_points[i + 1].z;

        ++triangle_id;
    }

    for (int i = 1; i < N_perimeter - 1; ++i) {
        triangles[triangle_id * 9] = bot_points[0].x;
        triangles[triangle_id * 9 + 1] = bot_points[0].y;
        triangles[triangle_id * 9 + 2] = bot_points[0].z;

        triangles[triangle_id * 9 + 3] = bot_points[i].x;
        triangles[triangle_id * 9 + 4] = bot_points[i].y;
        triangles[triangle_id * 9 + 5] = bot_points[i].z;

        triangles[triangle_id * 9 + 6] = bot_points[i + 1].x;
        triangles[triangle_id * 9 + 7] = bot_points[i + 1].y;
        triangles[triangle_id * 9 + 8] = bot_points[i + 1].z;

        ++triangle_id;
    }

    for (int i = 0; i < N_perimeter; ++i) {
        int j = (i + 1) % N_perimeter;

        triangles[triangle_id * 9] = bot_points[i].x;
        triangles[triangle_id * 9 + 1] = bot_points[i].y;
        triangles[triangle_id * 9 + 2] = bot_points[i].z;

        triangles[triangle_id * 9 + 3] = bot_points[j].x;
        triangles[triangle_id * 9 + 4] = bot_points[j].y;
        triangles[triangle_id * 9 + 5] = bot_points[j].z;

        triangles[triangle_id * 9 + 6] = top_points[i].x;
        triangles[triangle_id * 9 + 7] = top_points[i].y;
        triangles[triangle_id * 9 + 8] = top_points[i].z;

        for (int x = triangle_id * 9; x < triangle_id * 9 + 9; ++x) {
            printf("%f ", triangles[x]);
        }
        printf("\n");

        ++triangle_id;

        triangles[triangle_id * 9] = top_points[i].x;
        triangles[triangle_id * 9 + 1] = top_points[i].y;
        triangles[triangle_id * 9 + 2] = top_points[i].z;

        triangles[triangle_id * 9 + 3] = top_points[j].x;
        triangles[triangle_id * 9 + 4] = top_points[j].y;
        triangles[triangle_id * 9 + 5] = top_points[j].z;

        triangles[triangle_id * 9 + 6] = bot_points[j].x;
        triangles[triangle_id * 9 + 7] = bot_points[j].y;
        triangles[triangle_id * 9 + 8] = bot_points[j].z;

        ++triangle_id;
    }

    for (int i = 0; i < N_triangles * 3; ++i) {
        float x = triangles[i * 3 + 0];
        float y = triangles[i * 3 + 1];
        float z = triangles[i * 3 + 2];

        float r = (x + y + z) + R + H;
        float b = R + H - (x + y + z);
        colors[i * 3 + 0] = r / (r + b);
        colors[i * 3 + 1] = 0;
        colors[i * 3 + 2] = b / (r + b);
    }

//    static const GLfloat trig[] = {
//         0, 0, 0,
//         3, 3, 4,
//         7, 5, 1
//    };

//    for (int i = 0; i < 9; ++i) {
//        triangle_data[i] = trig[i];
//    }

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    int iter = 0;
    float time_scale = 200;
    float Rx = 50;
    float Ry = 30;
    float h_fly = 3;
    float t_start = -glm::pi<float>() / 2;
    float t_h_start = -glm::pi<float>() / 2;

    do{

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        float t = iter / time_scale;
        glm::vec3 cur_pos = glm::vec3(Rx * glm::cos(t + t_start), Ry * glm::sin(t + t_start), h_fly + 20 * glm::cos(t + t_h_start));

        glm::mat4 cur_view = glm::lookAt(
                                    cur_pos,
                                    center,
                                    glm::vec3(0,0,1)
                               );

        glm::mat4 cur_MVP = Projection * cur_view * Model;


        glUseProgram(programID);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &cur_MVP[0][0]);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );


        glDrawArrays(GL_TRIANGLES, 0, N_triangles * 3);


        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

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
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

