INCLUDE_DIRECTORIES(../common)

ADD_EXECUTABLE (flecs-launcher launcher.cpp ../common/util.cpp)

TARGET_LINK_LIBRARIES (flecs-launcher log4cxx ${Boost_LIBRARIES})

CONFIGURE_FILE (log4j.properties ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (flecs.trigger.agent ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (flecs.trigger.master ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (flecs.trigger.server ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (flecs.trigger.client ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
CONFIGURE_FILE (flecs.trigger.regen-fileset ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
