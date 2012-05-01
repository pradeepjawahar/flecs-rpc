INCLUDE_DIRECTORIES(
		${Boost_INCLUDE_DIRS}
		../common
		${CMAKE_CURRENT_BINARY_DIR}/../interface )

SET_SOURCE_FILES_PROPERTIES(
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.h
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		PROPERTIES GENERATED 1)


FOREACH (CLIENTS
		client-get
		client-put
		client-rand-get-append)

SET (CLIENT_EXE_NAME flecs-${CLIENTS})

ADD_EXECUTABLE ( ${CLIENT_EXE_NAME}
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		../common/util.cpp
		${CLIENTS}.cpp )
TARGET_LINK_LIBRARIES (${CLIENT_EXE_NAME} Ice IceUtil ${Boost_LIBRARIES} log4cxx)
ADD_DEPENDENCIES ( ${CLIENT_EXE_NAME} gen_rpc_interface )

ENDFOREACH (CLIENTS)


CONFIGURE_FILE (config.client ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (log4j.properties ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
