ADD_CUSTOM_COMMAND (OUTPUT flecs.h flecs.cpp
		COMMAND slice2cpp ${CMAKE_CURRENT_SOURCE_DIR}/flecs.ice
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/flecs.ice)

ADD_CUSTOM_TARGET (gen_rpc_interface ALL
		DEPENDS flecs.h flecs.cpp)
