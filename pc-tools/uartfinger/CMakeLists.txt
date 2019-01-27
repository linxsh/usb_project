INCLUDE(../cmake/compile.cmake)

LINK_DIRECTORIES   (${MY_LIBS_PATH}/lib     )

AUX_SOURCE_DIRECTORY(.  SRC)

SET(LIBS pthread m dl)

ADD_EXECUTABLE(uart-finger ${SRC})
TARGET_LINK_LIBRARIES(uart-finger ${LIBS})
INSTALL(TARGETS uart-finger DESTINATION ${MY_LIBS_PATH}/bin)
