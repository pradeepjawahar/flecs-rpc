INCLUDE_DIRECTORIES(
		${Boost_INCLUDE_DIRS}
		../common
		${CMAKE_CURRENT_BINARY_DIR}/../interface )

SET_SOURCE_FILES_PROPERTIES(
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.h
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		PROPERTIES GENERATED 1)

ADD_EXECUTABLE ( flecs-server
		${CMAKE_CURRENT_BINARY_DIR}/../interface/flecs.cpp
		../common/util.cpp
		main.cpp server.cpp c2sI.cpp sm2sI.cpp container.cpp globallock.cpp)

TARGET_LINK_LIBRARIES (flecs-server Ice IceUtil ${Boost_LIBRARIES} log4cxx)

ADD_DEPENDENCIES ( flecs-server gen_rpc_interface )


CONFIGURE_FILE (config.server ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (log4j.properties ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)


# Container plugins.

FOREACH (CONTAINER_PLUGIN
		rep-no-const
		rep-strong-const
		rep-redis)

ADD_LIBRARY (flecs-container-${CONTAINER_PLUGIN} MODULE container-${CONTAINER_PLUGIN}.cpp)

ADD_DEPENDENCIES (flecs-container-${CONTAINER_PLUGIN} gen_rpc_interface)
TARGET_LINK_LIBRARIES (flecs-container-${CONTAINER_PLUGIN} hiredis)

ENDFOREACH (CONTAINER_PLUGIN)
