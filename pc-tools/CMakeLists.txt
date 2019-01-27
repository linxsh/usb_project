CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(cmake/toolchain.cmake)

PROJECT(fingerprint)

INCLUDE_DIRECTORIES(${MY_LIBS}/include  )
INCLUDE(cmake/compile.cmake)

ADD_SUBDIRECTORY(./usbfinger)
ADD_SUBDIRECTORY(./uartfinger)
