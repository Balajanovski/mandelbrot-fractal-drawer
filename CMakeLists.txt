cmake_minimum_required(VERSION 3.6)
project(mandelbrot-fractal-drawer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(GLFW 3.0.0 REQUIRED)
if(GLFW_FOUND AND (GLFW_VERSION VERSION_EQUAL 3.0 OR GLFW_VERSION VERSION_GREATER 3.0))
    add_definitions( -DGLFW_VERSION_3 )
endif()

find_package(GLEW REQUIRED)
find_package(OPENGL REQUIRED)
find_package(PNG REQUIRED)

add_definitions(${PNG_DEFINITIONS})

set(SOURCE_FILES main.cpp Draw_Buffer.cpp Draw_Buffer.h Window.h Cleanup.h Get_GL.h Buffer_Base.h Image_Buffer.cpp Image_Buffer.h RGB.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW_LIBRARY}
                      ${GLEW_LIBRARY}
                      ${OPENGL_LIBRARY}
                      ${PNG_LIBRARIES})
