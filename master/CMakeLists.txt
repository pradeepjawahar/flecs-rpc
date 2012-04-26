INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_BINARY_DIR}/../interface )

SET_SOURCE_FILES_PROPERTIES(
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.h
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		PROPERTIES GENERATED 1)

ADD_EXECUTABLE ( flecs-master
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		main.cpp masterI.cpp )

TARGET_LINK_LIBRARIES (flecs-master Ice IceUtil log4cxx)

ADD_DEPENDENCIES ( flecs-master gen_rpc_interface )


CONFIGURE_FILE (config.master ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (log4j.properties ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
