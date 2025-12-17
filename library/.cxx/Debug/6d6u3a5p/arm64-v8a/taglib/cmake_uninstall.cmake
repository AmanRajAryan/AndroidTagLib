if (NOT EXISTS "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/install_manifest.txt\"")
endif()

file(READ "/storage/emulated/0/AndroidIDEProjects/AndroidTagLib/library/.cxx/Debug/6d6u3a5p/arm64-v8a/taglib/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    execute_process(
      COMMAND /data/user/0/com.tom.rv2ide/files/home/android-sdk/cmake/4.1.2/bin/cmake -E remove "$ENV{DESTDIR}${file}"
      OUTPUT_VARIABLE rm_out
      RESULT_VARIABLE rm_retval
    )
    if(NOT ${rm_retval} EQUAL 0)
      message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif ()
  else ()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif ()
endforeach()
