# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)
# lib/VL53L3CX 内のソースコードを追加
FILE(GLOB_RECURSE lib_sources ${CMAKE_SOURCE_DIR}/lib/VL53L3CX/*.cpp)


idf_component_register(
    SRCS ${app_sources} ${lib_sources}
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/lib/VL53L3CX
)
