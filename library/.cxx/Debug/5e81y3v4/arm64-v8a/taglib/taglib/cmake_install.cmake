# Install script for directory: /workspaces/AndroidTagLib/library/taglib/taglib

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/workspaces/AndroidTagLib/library/.cxx/Debug/5e81y3v4/arm64-v8a/taglib/taglib/libtag.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/taglib" TYPE FILE FILES
    "/workspaces/AndroidTagLib/library/taglib/taglib/tag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/fileref.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/audioproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/taglib_export.h"
    "/workspaces/AndroidTagLib/library/.cxx/Debug/5e81y3v4/arm64-v8a/taglib/taglib/../taglib_config.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/taglib.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tstring.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tlist.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tlist.tcc"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tstringlist.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tbytevector.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tbytevectorlist.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tvariant.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tbytevectorstream.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tiostream.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tfilestream.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tmap.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tmap.tcc"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tpicturetype.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tpropertymap.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tdebuglistener.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/toolkit/tversionnumber.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/mpegfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/mpegproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/mpegheader.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/xingheader.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v1/id3v1tag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v1/id3v1genres.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2extendedheader.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2frame.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2header.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2synchdata.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2footer.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2framefactory.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/id3v2tag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/attachedpictureframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/commentsframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/eventtimingcodesframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/ownershipframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/popularimeterframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/privateframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/relativevolumeframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/synchronizedlyricsframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/textidentificationframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/unknownframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/urllinkframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/chapterframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/tableofcontentsframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpeg/id3v2/frames/podcastframe.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/oggfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/oggpage.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/oggpageheader.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/xiphcomment.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/vorbis/vorbisfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/vorbis/vorbisproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/flac/oggflacfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/speex/speexfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/speex/speexproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/opus/opusfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ogg/opus/opusproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/flac/flacfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/flac/flacpicture.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/flac/flacproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/flac/flacmetadatablock.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ape/apefile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ape/apeproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ape/apetag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ape/apefooter.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/ape/apeitem.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpc/mpcfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mpc/mpcproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/wavpack/wavpackfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/wavpack/wavpackproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/trueaudio/trueaudiofile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/trueaudio/trueaudioproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/riff/rifffile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/riff/aiff/aifffile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/riff/aiff/aiffproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/riff/wav/wavfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/riff/wav/wavproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/riff/wav/infotag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/asf/asffile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/asf/asfproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/asf/asftag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/asf/asfattribute.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/asf/asfpicture.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mp4/mp4file.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mp4/mp4atom.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mp4/mp4tag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mp4/mp4item.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mp4/mp4properties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mp4/mp4coverart.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mp4/mp4itemfactory.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mod/modfilebase.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mod/modfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mod/modtag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/mod/modproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/it/itfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/it/itproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/s3m/s3mfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/s3m/s3mproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/xm/xmfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/xm/xmproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/dsf/dsffile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/dsf/dsfproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/dsdiff/dsdifffile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/dsdiff/dsdiffproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/dsdiff/dsdiffdiintag.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/shorten/shortenfile.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/shorten/shortenproperties.h"
    "/workspaces/AndroidTagLib/library/taglib/taglib/shorten/shortentag.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib/taglib-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib/taglib-targets.cmake"
         "/workspaces/AndroidTagLib/library/.cxx/Debug/5e81y3v4/arm64-v8a/taglib/taglib/CMakeFiles/Export/lib/cmake/taglib/taglib-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib/taglib-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib/taglib-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib" TYPE FILE FILES "/workspaces/AndroidTagLib/library/.cxx/Debug/5e81y3v4/arm64-v8a/taglib/taglib/CMakeFiles/Export/lib/cmake/taglib/taglib-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib" TYPE FILE FILES "/workspaces/AndroidTagLib/library/.cxx/Debug/5e81y3v4/arm64-v8a/taglib/taglib/CMakeFiles/Export/lib/cmake/taglib/taglib-targets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/taglib" TYPE FILE FILES
    "/workspaces/AndroidTagLib/library/.cxx/Debug/5e81y3v4/arm64-v8a/taglib/taglib-config.cmake"
    "/workspaces/AndroidTagLib/library/.cxx/Debug/5e81y3v4/arm64-v8a/taglib/taglib-config-version.cmake"
    )
endif()

