cmake_minimum_required(VERSION 3.11)
project(OpenGLHelper VERSION 1.0.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)

add_library(OpenGLHelper
    src/Camera.cpp
    src/FlyCameraControler.cpp
    src/Logger.cpp
    src/Program.cpp
    src/Texture2D.cpp
    src/VertexArrayObject.cpp
    src/Utilities.cpp
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
    cxx_std_17)
target_link_libraries(OpenGLHelper
    PRIVATE
    Glad::Glad
    Stb::Image
    glfw3
    GL)
add_library(Rafal::OpenGLHelper ALIAS OpenGLHelper)

file(GLOB_RECURSE HeaderFiles "*.h")
add_custom_target(headers SOURCES ${HeaderFiles})
