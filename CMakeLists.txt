cmake_minimum_required(VERSION 3.19)
project(Sphere)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(VERTEX_PATH "${SHADERS_DIR}/vObj.glsl")
set(FRAGMENT_PATH "${SHADERS_DIR}/fObj.glsl")

configure_file(
    ${CMAKE_SOURCE_DIR}/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
)

add_executable(
    ${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/src/main.cpp 
    ${CMAKE_SOURCE_DIR}/src/cubesphere.cpp
    ${CMAKE_SOURCE_DIR}/src/shader.cpp 
    ${CMAKE_SOURCE_DIR}/src/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRECTORIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES} dl)

target_compile_options(${PROJECT_NAME} PRIVATE ${GLFW_CFLAGS_OTHER})