FIND_PACKAGE( Boost
		COMPONENTS filesystem system )
IF (NOT Boost_FOUND)
	message(FATAL_ERROR "Boost not found!")
ENDIF()


INCLUDE_DIRECTORIES(
		${Boost_INCLUDE_DIRS}
		../common
		${CMAKE_CURRENT_BINARY_DIR}/../interface )

SET_SOURCE_FILES_PROPERTIES(
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.h
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		PROPERTIES GENERATED 1)

ADD_EXECUTABLE ( server
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		../common/util.cpp
		main.cpp server-common.cpp c2sI.cpp s2sI.cpp )

TARGET_LINK_LIBRARIES (server Ice IceUtil ${Boost_LIBRARIES})

ADD_DEPENDENCIES ( server gen_rpc_interface )


CONFIGURE_FILE (config.server ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
