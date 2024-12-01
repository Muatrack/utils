cmake_minimum_required(VERSION 3.16)

project(Utils)

add_library(utils 
            src/utils.c 
            src/base64.c
            src/cycle_array.c
            src/json_compress.c
            src/url_code.c
            )
target_include_directories(utils PUBLIC  inc)
target_include_directories(utils PUBLIC  inc/utils)
target_include_directories(utils PRIVATE priv)
target_include_directories(utils PRIVATE ${CMAKE_BINARY_DIR})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    # target_compile_definitions(utils PUBLIC COMPILE_SYS_LINUX)
    option(COMPILE_SYS_LINUX    ""  ON)
endif()

configure_file(
                    ${CMAKE_SOURCE_DIR}/utilsConfigure.h.in 
                    ${CMAKE_BINARY_DIR}/utilsConfigure.h
            )

message(STATUS "CurPlatform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_NAME :${CMAKE_SYSTEM_NAME}")