cmake_minimum_required(VERSION 3.11)
project(Lesson09 VERSION 1.0.0 LANGUAGES CXX)

find_package(GLM REQUIRED)

add_executable(Lesson09
    src/main.cpp
    )
target_link_libraries(Lesson09
    PRIVATE
    Rafal::OpenGLHelper
    Glad::Glad
    )
target_compile_features(Lesson09
    PRIVATE
    cxx_std_17)

add_custom_target(ResourcesL09
    SOURCES
    src/resources/fs_light.frag
    src/resources/fs_light_mixed.frag
    src/resources/vs.vert
    src/resources/container2.png
    src/resources/container2_specular.png
    )
file(REMOVE_RECURSE
    ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
    DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR})

