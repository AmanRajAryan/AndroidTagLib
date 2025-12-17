# Install script for directory: /storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/jni/opusfile-0.12

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
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/data/user/0/com.tom.rv2ide/files/home/android-sdk/ndk/29.0.14033849/toolchains/llvm/prebuilt/linux-arm64/bin/llvm-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/jni/opusfile-0.12/libopusfile.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opus" TYPE FILE FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/jni/opusfile-0.12/include/opusfile.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/jni/opusfile-0.12/libopusurl.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opus" TYPE FILE FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/jni/opusfile-0.12/include/opusfile.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/opusfile/OpusFileTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/opusfile/OpusFileTargets.cmake"
         "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/jni/opusfile-0.12/CMakeFiles/Export/5b4f1099bc6a6fc52dc0348a9c6b3f98/OpusFileTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/opusfile/OpusFileTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/opusfile/OpusFileTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/opusfile" TYPE FILE FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/jni/opusfile-0.12/CMakeFiles/Export/5b4f1099bc6a6fc52dc0348a9c6b3f98/OpusFileTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/opusfile" TYPE FILE FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/jni/opusfile-0.12/CMakeFiles/Export/5b4f1099bc6a6fc52dc0348a9c6b3f98/OpusFileTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/opusfile" TYPE FILE FILES
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/jni/opusfile-0.12/OpusFileConfig.cmake"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/jni/opusfile-0.12/OpusFileConfigVersion.cmake"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/jni/opusfile-0.12/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
