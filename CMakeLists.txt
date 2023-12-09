cmake_minimum_required(VERSION 3.0.0...3.5.0)
project(Modern-OpenGL-Boucing-Ball-Demo)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)

set(PROJECT_DEPENDENCIES_DIR "${CMAKE_CURRENT_LIST_DIR}/Dependencies")
set(PROJECT_DEPENDENCIES_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/Dependencies")
include(BuildGLFW)
include(BuildGLEW)
include(BuildCGLM)


set(PROJECT_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Source")
set(PROJECT_SOURCES 
    "${PROJECT_SOURCE_DIRECTORY}/main.c"
    "${PROJECT_SOURCE_DIRECTORY}/utility.c"
    "${PROJECT_SOURCE_DIRECTORY}/utility.h"
)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCES})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(PROJECT_TARGET_NAME Ball-Demo)
add_executable(${PROJECT_TARGET_NAME})
target_sources(${PROJECT_TARGET_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_TARGET_NAME} PRIVATE ${PROJECT_INCLUDE_DIR})

add_dependencies(${PROJECT_TARGET_NAME} glfw)
add_dependencies(${PROJECT_TARGET_NAME} libglew_static)
add_dependencies(${PROJECT_TARGET_NAME} cglm)

target_link_libraries(${PROJECT_TARGET_NAME} PRIVATE OpenGL::GL glfw libglew_static cglm)
