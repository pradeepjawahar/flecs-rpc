ADD_CUSTOM_COMMAND (OUTPUT agent.h agent.cpp
		COMMAND slice2cpp ${CMAKE_CURRENT_SOURCE_DIR}/agent.ice
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/agent.ice)

ADD_CUSTOM_TARGET (gen_agent_interface ALL
		DEPENDS agent.h agent.cpp)

INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_BINARY_DIR})

SET_SOURCE_FILES_PROPERTIES(
		${CMAKE_CURRENT_BINARY_DIR}/agent.h
		${CMAKE_CURRENT_BINARY_DIR}/agent.cpp
		PROPERTIES GENERATED 1)

ADD_EXECUTABLE ( agent-server
		${CMAKE_CURRENT_BINARY_DIR}/agent.cpp
		server.cpp agentI.cpp )

TARGET_LINK_LIBRARIES (agent-server Ice IceUtil ${Boost_LIBRARIES} log4cxx)

ADD_DEPENDENCIES ( agent-server gen_agent_interface )


ADD_EXECUTABLE ( agent-client
		${CMAKE_CURRENT_BINARY_DIR}/agent.cpp
		client.cpp )

TARGET_LINK_LIBRARIES (agent-client Ice IceUtil ${Boost_LIBRARIES} log4cxx)

ADD_DEPENDENCIES ( agent-client gen_agent_interface )

CONFIGURE_FILE (config.agent.server ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (config.agent.client ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (log4j.properties ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
