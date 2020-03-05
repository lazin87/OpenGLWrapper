cmake_minimum_required(VERSION 3.11)
project(Lesson05 VERSION 1.0.0 LANGUAGES CXX)

find_package(glm REQUIRED)

add_executable(Lesson05
    src/main.cpp
    )
target_link_libraries(Lesson05
    PRIVATE
    Rafal::OpenGLHelper
    Glad::Glad
    )
target_compile_features(Lesson05
    PRIVATE
    cxx_std_17)

add_custom_target(ResourcesL05
    SOURCES
    src/resources/fs.frag
    src/resources/vs.vert
    src/resources/awesomeface.png
    src/resources/container.jpg
    )
file(REMOVE_RECURSE
    ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
    DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR})

