cmake_minimum_required(VERSION 3.10)
project(TagLibJNI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DISABLE_FIND_PACKAGE_Git TRUE)
set(GIT_EXECUTABLE "FALSE" CACHE FILEPATH "Force disable git" FORCE)
set(Git_FOUND FALSE CACHE BOOL "Force disable git" FORCE)

set(OP_GIT_VERSION "0.12" CACHE STRING "Force opusfile version" FORCE)
set(OP_GIT_TAG "v0.12" CACHE STRING "Force opusfile tag" FORCE)
set(PACKAGE_VERSION "0.12" CACHE STRING "Force package version" FORCE)

add_subdirectory(taglib)
add_subdirectory(jni/libogg-1.3.5)
add_subdirectory(jni/libvorbis-1.3.7)
add_subdirectory(jni/opus-1.3.1)
add_subdirectory(jni/opusfile-0.12)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/jni/libvorbis-1.3.7/include
    ${CMAKE_CURRENT_SOURCE_DIR}/jni/libvorbis-1.3.7/include/vorbis
    ${CMAKE_CURRENT_BINARY_DIR}/jni/libvorbis-1.3.7/include
    ${CMAKE_CURRENT_SOURCE_DIR}/jni/libogg-1.3.5/include
    ${CMAKE_CURRENT_SOURCE_DIR}/jni/opus-1.3.1/include
    ${CMAKE_CURRENT_SOURCE_DIR}/jni/opusfile-0.12/include
)

target_link_libraries(taglib_jni ${log-lib} tag vorbisfile Ogg::ogg opus opusfile)
