cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS Off)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c" "external/*.cpp" "external/*.c")

add_executable(Engine ${SOURCES})
target_compile_definitions(Engine PRIVATE _GLIBCXX_DEBUG)

target_include_directories(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes ${CMAKE_CURRENT_SOURCE_DIR}/external)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(Engine PRIVATE glfw glm::glm ${OPENGL_gl_LIBRARY})