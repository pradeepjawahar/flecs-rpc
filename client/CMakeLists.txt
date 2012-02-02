INCLUDE_DIRECTORIES( ../common ${CMAKE_CURRENT_BINARY_DIR}/../interface )

SET_SOURCE_FILES_PROPERTIES(
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.h
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		PROPERTIES GENERATED 1)

ADD_EXECUTABLE ( client
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		../common/util.cpp
		client.cpp )

TARGET_LINK_LIBRARIES (client Ice IceUtil)

ADD_DEPENDENCIES ( client gen_rpc_interface )


CONFIGURE_FILE (config.client ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
