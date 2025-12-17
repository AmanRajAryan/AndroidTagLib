# Install script for directory: /storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/taglib/libtag.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/taglib" TYPE FILE FILES
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/tag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/fileref.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/audioproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/taglib_export.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/taglib/../taglib_config.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/taglib.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tstring.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tlist.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tlist.tcc"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tstringlist.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tbytevector.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tbytevectorlist.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tvariant.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tbytevectorstream.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tiostream.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tfilestream.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tmap.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tmap.tcc"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tpicturetype.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tpropertymap.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tdebuglistener.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/toolkit/tversionnumber.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/mpegfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/mpegproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/mpegheader.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/xingheader.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v1/id3v1tag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v1/id3v1genres.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2extendedheader.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2frame.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2header.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2synchdata.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2footer.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2framefactory.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2tag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/attachedpictureframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/commentsframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/eventtimingcodesframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/ownershipframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/popularimeterframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/privateframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/relativevolumeframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/synchronizedlyricsframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/textidentificationframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/unknownframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/urllinkframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/chapterframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/tableofcontentsframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/podcastframe.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/oggfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/oggpage.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/oggpageheader.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/xiphcomment.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/vorbis/vorbisfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/vorbis/vorbisproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/flac/oggflacfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/speex/speexfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/speex/speexproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/opus/opusfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ogg/opus/opusproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/flac/flacfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/flac/flacpicture.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/flac/flacproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/flac/flacmetadatablock.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ape/apefile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ape/apeproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ape/apetag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ape/apefooter.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/ape/apeitem.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpc/mpcfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mpc/mpcproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/wavpack/wavpackfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/wavpack/wavpackproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/trueaudio/trueaudiofile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/trueaudio/trueaudioproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/riff/rifffile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/riff/aiff/aifffile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/riff/aiff/aiffproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/riff/wav/wavfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/riff/wav/wavproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/riff/wav/infotag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/asf/asffile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/asf/asfproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/asf/asftag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/asf/asfattribute.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/asf/asfpicture.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mp4/mp4file.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mp4/mp4atom.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mp4/mp4tag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mp4/mp4item.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mp4/mp4properties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mp4/mp4coverart.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mp4/mp4itemfactory.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mod/modfilebase.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mod/modfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mod/modtag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/mod/modproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/it/itfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/it/itproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/s3m/s3mfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/s3m/s3mproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/xm/xmfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/xm/xmproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/dsf/dsffile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/dsf/dsfproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/dsdiff/dsdifffile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/dsdiff/dsdiffproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/dsdiff/dsdiffdiintag.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/shorten/shortenfile.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/shorten/shortenproperties.h"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/taglib/taglib/shorten/shortentag.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib/taglib-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib/taglib-targets.cmake"
         "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/taglib/CMakeFiles/Export/398eef5e047a0959864f2888198961bf/taglib-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib/taglib-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib/taglib-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib" TYPE FILE FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/taglib/CMakeFiles/Export/398eef5e047a0959864f2888198961bf/taglib-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib" TYPE FILE FILES "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/taglib/CMakeFiles/Export/398eef5e047a0959864f2888198961bf/taglib-targets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib" TYPE FILE FILES
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/taglib-config.cmake"
    "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/taglib-config-version.cmake"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/taglib/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
