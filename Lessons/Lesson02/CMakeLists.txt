cmake_minimum_required(VERSION 3.11)
project(Lesson02 VERSION 1.0.0 LANGUAGES CXX)

add_executable(Lesson02
    src/main.cpp
    )
target_link_libraries(Lesson02
    PRIVATE
    Rafal::OpenGLHelper
    Glad::Glad
    )

add_custom_target(ResourcesL02
    SOURCES
    src/resources/fs.frag
    src/resources/vs.vert
    )
file(REMOVE_RECURSE
    ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
    DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR})

