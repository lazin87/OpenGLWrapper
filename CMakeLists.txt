cmake_minimum_required(VERSION 3.11)
project(LearnOpenGL VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(3rdPart/glad)
add_subdirectory(3rdPart/stb)

add_subdirectory(OpenGLHelper)

add_subdirectory(Lessons/Lesson02)
add_subdirectory(Lessons/Lesson03)

