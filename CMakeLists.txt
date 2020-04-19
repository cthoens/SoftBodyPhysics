cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(SoftBodyPhysics)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)

file(GLOB_RECURSE CORE_HDR src/*.h)
file(GLOB_RECURSE CORE_SRC src/*.cpp)

add_definitions("-std=c++11")
add_executable(index ${CORE_SRC} ${CORE_HDR})

set_target_properties(index PROPERTIES COMPILE_FLAGS "-s USE_SDL=2 -s DISABLE_EXCEPTION_CATCHING=0")
set_target_properties(index PROPERTIES LINK_FLAGS "-s WASM=1 -s USE_SDL=2 -s DISABLE_EXCEPTION_CATCHING=0")
