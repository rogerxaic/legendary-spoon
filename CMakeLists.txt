cmake_minimum_required (VERSION 2.6)
project (Traveller C)

set (Traveller_VERSION_MAJOR 1)
set (Traveller_VERSION_MINOR 0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_executable(users users.c)
