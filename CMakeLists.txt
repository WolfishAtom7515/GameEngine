cmake_minimum_required(VERSION 3.20)
project(LostInTime)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

add_library(glad ${CMAKE_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include/headers)

add_library(game ${CMAKE_SOURCE_DIR}/src/game.cpp)
add_library(shader ${CMAKE_SOURCE_DIR}/src/shader.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include/headers)  

add_executable(LostInTime src/main.cpp)

target_link_libraries(LostInTime
    glad
    ${OPENGL_gl_LIBRARY}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    game
    shader
)