cmake_minimum_required(VERSION 3.10)
project(TagLibJNI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_BINDINGS OFF CACHE BOOL "" FORCE)
set(VISIBILITY_HIDDEN ON CACHE BOOL "" FORCE)

set(WITH_MOD OFF CACHE BOOL "Build with Tracker modules" FORCE)
set(WITH_SHORTEN OFF CACHE BOOL "Build with Shorten" FORCE)
set(WITH_TRUEAUDIO OFF CACHE BOOL "Build with TrueAudio" FORCE)
set(WITH_DSF OFF CACHE BOOL "Build with DSF" FORCE)

add_subdirectory(taglib)

add_library(taglib_jni SHARED jni/TagLibJNI.cpp)
find_library(log-lib log)

target_include_directories(taglib_jni PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mp4
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/riff/wav
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/riff/aiff
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ogg
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ogg/opus
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ogg/vorbis
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mpeg/id3v2
    $<TARGET_PROPERTY:tag,INTERFACE_INCLUDE_DIRECTORIES>
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/toolkit
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mpeg
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/flac
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mpc
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ape
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/asf
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/dsf
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/riff
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/trueaudio
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/wavpack
    ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/shorten
    ${CMAKE_CURRENT_BINARY_DIR}/taglib/taglib
)

target_link_libraries(taglib_jni ${log-lib} tag)
target_link_options(taglib_jni PRIVATE -Wl,--gc-sections -Wl,--exclude-libs,ALL)
