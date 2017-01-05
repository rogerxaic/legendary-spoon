cmake_minimum_required (VERSION 2.6)
project (Traveller C)

set (Traveller_VERSION_MAJOR 1)
set (Traveller_VERSION_MINOR 0)

#find_package(Curses REQUIRED)
#include_directories(${CURSES_INCLUDE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
#add_executable(users users.c)
