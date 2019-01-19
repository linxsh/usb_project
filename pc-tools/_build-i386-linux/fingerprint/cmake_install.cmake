# Install script for directory: /home/linxsh/soure-code/usb_project/pc-tools/fingerprint

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/opt/mylib/i386-linux")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/opt/mylib/i386-linux/bin/fingerprint" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/mylib/i386-linux/bin/fingerprint")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/opt/mylib/i386-linux/bin/fingerprint"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/mylib/i386-linux/bin/fingerprint")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/mylib/i386-linux/bin" TYPE EXECUTABLE FILES "/home/linxsh/soure-code/usb_project/pc-tools/_build-i386-linux/fingerprint/fingerprint")
  if(EXISTS "$ENV{DESTDIR}/opt/mylib/i386-linux/bin/fingerprint" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/mylib/i386-linux/bin/fingerprint")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/opt/mylib/i386-linux/bin/fingerprint"
         OLD_RPATH "/opt/mylib/i386-linux/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/opt/mylib/i386-linux/bin/fingerprint")
    endif()
  endif()
endif()

