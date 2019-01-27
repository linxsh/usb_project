INCLUDE(../cmake/compile.cmake)

LINK_DIRECTORIES   (${MY_LIBS_PATH}/lib     )
INCLUDE_DIRECTORIES(${MY_LIBS_PATH}/include/libusb-1.0 )

AUX_SOURCE_DIRECTORY(.  SRC)

SET(LIBS usb-1.0 pthread m dl)

ADD_EXECUTABLE(usb-finger ${SRC})
TARGET_LINK_LIBRARIES(usb-finger ${LIBS})
INSTALL(TARGETS usb-finger DESTINATION ${MY_LIBS_PATH}/bin)
