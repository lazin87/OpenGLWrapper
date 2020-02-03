cmake_minimum_required(VERSION 3.5)
project(StbImage VERSION 1.0.0 LANGUAGES CXX)

add_library(stbImage src/stb_image.cpp)
target_include_directories(stbImage
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )
add_library(Stb::Image ALIAS stbImage)
