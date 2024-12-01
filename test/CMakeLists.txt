cmake_minimum_required(VERSION 3.16)

project(UTILS-TEST VERSION 1.0.0)

include_directories( "../inc" )
add_executable(utils_test main.c )
target_link_directories(utils_test PRIVATE ../)

include(CTest)
add_test(   
            NAME        UtilsTest 
            COMMAND     ./utils_test
        )