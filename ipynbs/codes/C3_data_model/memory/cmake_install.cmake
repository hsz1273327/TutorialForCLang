# Install script for directory: /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C3_data_model/memory

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY" TYPE SHARED_LIBRARY FILES "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C3_data_model/memory/lib/libmemory.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY/libmemory.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY/libmemory.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libmemory.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY/libmemory.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY/libmemory.dylib")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY" TYPE SHARED_LIBRARY FILES "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C3_data_model/memory/lib/libmemcheck.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY/libmemcheck.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY/libmemcheck.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libmemcheck.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY/libmemcheck.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMAKE_LIBRARY_OUTPUT_DIRECTORY/libmemcheck.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C3_data_model/memory/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
