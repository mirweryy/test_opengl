cmake_minimum_required(VERSION 4.0.0 FATAL_ERROR)

set(project_name Test_opengl)

project(${project_name})

add_executable(${project_name} src/main.cpp)

add_subdirectory(external/glfw)

target_link_libraries(${project_name} glfw)

add_subdirectory(external/glad)

target_link_libraries(${project_name} glad)