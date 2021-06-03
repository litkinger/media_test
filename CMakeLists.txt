cmake_minimum_required(VERSION 3.5)

project(ffmpeg_sdl_test LANGUAGES CXX)

SET(PROJECT_ROOT_PATH "${CMAKE_SOURCE_DIR}/build-ffmpeg_sdl_test-Desktop_Qt_5_12_10_clang_64bit-Debug")
MESSAGE( STATUS "this var key = ${PROJECT_ROOT_PATH}.")

add_subdirectory(src)
add_subdirectory(test)

