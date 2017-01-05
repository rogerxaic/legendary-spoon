include_directories(
        ../lib/
)

add_subdirectory(launcher)

add_executable(users users.c)
add_executable(agence agence.c)
add_executable(tirage tirage.c)
add_executable(display display.c)
#target_link_libraries(display ${CURSES_LIBRARIES})