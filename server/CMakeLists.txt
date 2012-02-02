FIND_PACKAGE( Boost
		COMPONENTS filesystem system )
IF (NOT Boost_FOUND)
	message(FATAL_ERROR "Boost not found!")
ENDIF()


CONFIGURE_FILE (config.server ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)


ADD_CUSTOM_COMMAND (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/../flecs.h ${CMAKE_CURRENT_BINARY_DIR}/../flecs.cpp
		COMMAND slice2cpp --output-dir=${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../flecs.ice
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../flecs.ice )


INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} ../common ${CMAKE_CURRENT_BINARY_DIR}/.. )

ADD_EXECUTABLE ( server
		${CMAKE_CURRENT_BINARY_DIR}/../flecs.cpp
		../common/util.cpp
		main.cpp server.cpp server-common.cpp c2sI.cpp s2sI.cpp )

TARGET_LINK_LIBRARIES (server Ice IceUtil ${Boost_LIBRARIES})
