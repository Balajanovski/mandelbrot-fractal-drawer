cmake_minimum_required(VERSION 3.6)
project(mandelbrot-fractal-drawer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(GLFW 3.0.0)
if (GLFW_FOUND)
    add_definitions( -DGLFW_FOUND )
    message("- GLFW Located")
    include_directories(${GLFW_INCLUDE_DIR})
else()
    message("- GLFW not located\n->Windowed Mode Disabled")
endif()
if(GLFW_FOUND AND (GLFW_VERSION VERSION_EQUAL 3.0 OR GLFW_VERSION VERSION_GREATER 3.0))
    add_definitions( -DGLFW_VERSION_3 )
endif()

find_package(GLEW)
if (GLEW_FOUND)
    add_definitions( -DGLEW_FOUND )
    include_directories(${GLEW_INCLUDE_DIR})
    message("- GLEW Located")
else()
    message("- GLEW not located\n->Windowed Mode Disabled")
endif()

find_package(OPENGL)
if (OPENGL_FOUND)
    add_definitions( -DOPENGL_FOUND )
    include_directories(${OPENGL_INCLUDE_DIR})
    message("- OpenGL Located")
else()
    message("- OpenGL not located\n->Windowed Mode Disabled")
endif()

find_package(PNG)
if (PNG_FOUND)
    add_definitions( -DPNG_FOUND )
    message("- LibPNG Located")
    include_directories(${PNG_INCLUDE_DIRS})
    add_definitions(${PNG_DEFINITIONS})
    include_directories(${PNG_INCLUDE_DIRS})
else()
    message("- LibPNG not located\n->Image Mode Disabled")
endif()

find_package(OpenCL REQUIRED)

file(GLOB SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/*.h"
        "${PROJECT_SOURCE_DIR}/*.hpp"
        "${PROJECT_SOURCE_DIR}/*.cpp"
        "${PROJECT_SOURCE_DIR}/OpenCL/*.h"
        "${PROJECT_SOURCE_DIR}/OpenCL/*.hpp"
        "${PROJECT_SOURCE_DIR}/OpenCL/*.cpp"
        )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})



include_directories(${OPENCL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
                      ${GLFW_LIBRARY}
                      ${GLEW_LIBRARY}
                      ${OPENGL_LIBRARY}
                      ${PNG_LIBRARIES}
                      ${OPENCL_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/Includes
        ${PROJECT_SOURCE_DIR}
        )