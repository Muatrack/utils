cmake_minimum_required(VERSION 3.16)

project(UTILS-TEST VERSION 1.0.0)

include_directories( "../inc" )
add_executable(TUtimer test_timer.c )
target_link_directories(TUtimer PRIVATE ../)

include(CTest)

add_test( NAME   UTIMER-10s     COMMAND     ls)
add_test( NAME   UTIMER-output  COMMAND     TUtimer)
