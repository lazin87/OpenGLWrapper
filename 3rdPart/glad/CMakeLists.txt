cmake_minimum_required(VERSION 3.5)
project(glad VERSION 1.0.0 LANGUAGES C)

add_library(Glad src/glad.c)
target_include_directories(Glad
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(Glad
    PUBLIC
    GL
    dl)
add_library(Glad::Glad ALIAS Glad)
