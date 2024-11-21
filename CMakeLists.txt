cmake_minimum_required(VERSION 3.16)

project(Utils)

add_library(utils 
            src/utils.c 
            src/base64.c
            src/cycle_array.c
            src/json_compress.c
            src/url_code.c
            )
target_include_directories(utils PUBLIC inc)
target_include_directories(utils PUBLIC inc/utils)

message(STATUS "CurPlatform: ${CMAKE_SYSTEM_NAME}")
