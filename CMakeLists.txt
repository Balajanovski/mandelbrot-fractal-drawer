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
if (GLEW_FOUND)
    add_definitions( -DGLEW_FOUND )
endif()

find_package(OPENGL REQUIRED)
find_package(PNG REQUIRED)
if (PNG_FOUND)
    add_definitions( -DPNG_FOUND )
endif()

find_package(OpenCL REQUIRED)

add_definitions(${PNG_DEFINITIONS})

set(SOURCE_FILES main.cpp Screen_Stream.cpp Screen_Stream.h Bounds2D.h Get_GL.h Pixel_Stream_Base.h Image_Stream.cpp Image_Stream.h RGB.h
        Pixel_Calculator.cpp Pixel_Calculator.h OpenCL/cl.hpp)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${OPENCL_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW_LIBRARY}
                      ${GLEW_LIBRARY}
                      ${OPENGL_LIBRARY}
                      ${PNG_LIBRARIES}
                      ${OPENCL_LIBRARIES})
