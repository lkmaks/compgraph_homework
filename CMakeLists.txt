cmake_minimum_required(VERSION 3.0)

project(hw1)

find_package(OpenGL REQUIRED)


# Compile external dependencies
add_subdirectory (external)


include_directories(
        external/AntTweakBar-1.16/include/
        external/glfw-3.1.2/include/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/assimp-3.0.1270/include/
        external/bullet-2.81-rev2613/src/
        .
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)


add_executable(two_triangles
        two_triangles/two_triangles.cpp
        common/shader.cpp
        common/shader.hpp

        two_triangles/VertexShader.vertexshader
        two_triangles/FragmentShader1.fragmentshader
        two_triangles/FragmentShader2.fragmentshader
)
target_link_libraries(two_triangles
        ${ALL_LIBS}
)

add_executable(puck
        puck/puck.cpp
        common/shader.cpp
        common/shader.hpp

        puck/VertexShader.vertexshader
        puck/FragmentShader.fragmentshader
)
target_link_libraries(puck
        ${ALL_LIBS}
)


