cmake_minimum_required(VERSION 3.11)
project(Lesson06 VERSION 1.0.0 LANGUAGES CXX)

find_package(glm REQUIRED)

add_executable(Lesson06
    src/main.cpp
    )
target_link_libraries(Lesson06
    PRIVATE
    Rafal::OpenGLHelper
    Glad::Glad
    glfw3
    )
target_compile_features(Lesson06
    PRIVATE
    cxx_std_17)

add_custom_target(ResourcesL06
    SOURCES
    src/resources/fs.frag
    src/resources/fs_light.frag
    src/resources/fs_material.frag
    src/resources/vs.vert
    )
file(REMOVE_RECURSE
    ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
    DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR})

