cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
find_package(PkgConfig REQUIRED)

#Find and link GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
 message(Error " GLEW not found")
endif(NOT GLEW_FOUND)

#Find and link glfw3
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})
add_definitions(${GLFW_DEFINITIONS})

if(NOT GLFW_FOUND)
        message(Error "GLFW not found")
endif(NOT GLFW_FOUND)

#Find and link glm
find_package(glm REQUIRED)
include_directories(${glm_INCLUDE_DIRS})
link_directories(${glm_LIBRARY_DIRS})
add_definitions(${glm_DEFINITIONS})

if(NOT glm_FOUND)
        message(Error "GLM not found")
endif(NOT glm_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OpenGL_FOUND)
        message(Error "OpenGL not found")
endif(NOT OpenGL_FOUND)

#Include Files
aux_source_directory(. DIR_SRCS)
add_executable(test_engine ${DIR_SRCS})

target_link_libraries(test_engine ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${glm_LIBRARIES})


