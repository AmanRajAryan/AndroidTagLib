cmake_minimum_required(VERSION 3.10)
project(opusfile)

add_library(opusfile STATIC
    src/info.c
    src/internal.c
    src/opusfile.c
    src/stream.c
)

# Opusfile depends on opus and ogg
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ../opus-1.3.1/include
    ../libogg-1.3.5/include
)
target_link_libraries(opusfile PRIVATE opus ogg)
