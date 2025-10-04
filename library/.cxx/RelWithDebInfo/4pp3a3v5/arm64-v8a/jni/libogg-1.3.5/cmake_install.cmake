# Install script for directory: /workspaces/AndroidTagLib/library/jni/libogg-1.3.5

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/workspaces/AndroidTagLib/android-sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/workspaces/AndroidTagLib/library/.cxx/RelWithDebInfo/4pp3a3v5/arm64-v8a/jni/libogg-1.3.5/libogg.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ogg" TYPE FILE FILES
    "/workspaces/AndroidTagLib/library/.cxx/RelWithDebInfo/4pp3a3v5/arm64-v8a/jni/libogg-1.3.5/include/ogg/config_types.h"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/include/ogg/ogg.h"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/include/ogg/os_types.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg/OggTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg/OggTargets.cmake"
         "/workspaces/AndroidTagLib/library/.cxx/RelWithDebInfo/4pp3a3v5/arm64-v8a/jni/libogg-1.3.5/CMakeFiles/Export/lib/cmake/Ogg/OggTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg/OggTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg/OggTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg" TYPE FILE FILES "/workspaces/AndroidTagLib/library/.cxx/RelWithDebInfo/4pp3a3v5/arm64-v8a/jni/libogg-1.3.5/CMakeFiles/Export/lib/cmake/Ogg/OggTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg" TYPE FILE FILES "/workspaces/AndroidTagLib/library/.cxx/RelWithDebInfo/4pp3a3v5/arm64-v8a/jni/libogg-1.3.5/CMakeFiles/Export/lib/cmake/Ogg/OggTargets-relwithdebinfo.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg" TYPE FILE FILES
    "/workspaces/AndroidTagLib/library/.cxx/RelWithDebInfo/4pp3a3v5/arm64-v8a/jni/libogg-1.3.5/OggConfig.cmake"
    "/workspaces/AndroidTagLib/library/.cxx/RelWithDebInfo/4pp3a3v5/arm64-v8a/jni/libogg-1.3.5/OggConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/workspaces/AndroidTagLib/library/.cxx/RelWithDebInfo/4pp3a3v5/arm64-v8a/jni/libogg-1.3.5/ogg.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libogg/html" TYPE FILE FILES
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/framing.html"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/index.html"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/oggstream.html"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/ogg-multiplex.html"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/fish_xiph_org.png"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/multiplex1.png"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/packets.png"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/pages.png"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/stream.png"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/vorbisword2.png"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/white-ogg.png"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/white-xifish.png"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/rfc3533.txt"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/rfc5334.txt"
    "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/skeleton.html"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libogg/html" TYPE DIRECTORY FILES "/workspaces/AndroidTagLib/library/jni/libogg-1.3.5/doc/libogg")
endif()

