cmake_minimum_required(VERSION 3.11)
project(Lesson10 VERSION 1.0.0 LANGUAGES CXX)

find_package(GLM REQUIRED)

add_executable(Lesson10
    src/main.cpp
    )
target_link_libraries(Lesson10
    PRIVATE
    Rafal::OpenGLHelper
    Glad::Glad
    )
target_compile_features(Lesson10
    PRIVATE
    cxx_std_17)

add_custom_target(ResourcesL10
    SOURCES
    src/resources/vs.vert
    src/resources/fs.frag
    src/resources/metal.png
    src/resources/marble.jpg
    )
file(REMOVE_RECURSE
    ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
    DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR})

