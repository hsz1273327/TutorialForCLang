# Install script for directory: /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build")
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
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/lib/libexception.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/lib" TYPE STATIC_LIBRARY FILES "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/lib/libexception.a")
  if(EXISTS "$ENV{DESTDIR}/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/lib/libexception.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/lib/libexception.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/lib/libexception.a")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/include/assert.h;/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/include/except.h;/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/include/error.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/build/include" TYPE FILE FILES
    "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/source/include/assert.h"
    "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/source/include/except.h"
    "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/source/include/error.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
