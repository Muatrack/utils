cmake_minimum_required(VERSION 3.16)
project(Utils)
add_library(utils src/utils.c)
target_include_directories(utils PUBLIC inc)
target_include_directories(utils PUBLIC inc/utils)

message(STATUS "CurPlatform: ${CMAKE_SYSTEM_NAME}")