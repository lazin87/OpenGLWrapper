cmake_minimum_required(VERSION 3.11)
project(OpenGLHelper VERSION 1.0.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
add_subdirectory(3rdPart/glad)

add_library(OpenGLHelper
    src/IRenderer.cpp
    src/Logger.cpp
    src/Program.cpp
    src/VertexArrayObject.cpp
    src/WindowContext.cpp
    )
target_include_directories(OpenGLHelper
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_features(OpenGLHelper
    PRIVATE
    cxx_std_14)
target_link_libraries(OpenGLHelper
    PRIVATE
    Glad::Glad
    glfw3
    GL)
add_library(Rafal::OpenGLHelper ALIAS OpenGLHelper)

add_executable(TestRunner
    src/main.cpp
    src/lessons/lesson02_triangle.cpp
    src/lessons/lesson03_textures.cpp
    )
target_include_directories(TestRunner
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)
target_link_libraries(TestRunner
    PRIVATE
    Rafal::OpenGLHelper
    Glad::Glad
    )

