CMAKE_MINIMUM_REQUIRED (VERSION 2.6 FATAL_ERROR)

PROJECT (FleCS)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

LIST (APPEND CMAKE_CXX_FLAGS "-Wall -Werror")

SET (CMAKE_BUILD_TYPE Release)
#SET (CMAKE_BUILD_TYPE Debug)

FIND_PACKAGE(Boost
		COMPONENTS filesystem system program_options
		REQUIRED)

ADD_SUBDIRECTORY (interface)

ADD_SUBDIRECTORY (data)

ADD_SUBDIRECTORY (master)
ADD_SUBDIRECTORY (server)
ADD_SUBDIRECTORY (client)

ADD_SUBDIRECTORY (agent)
ADD_SUBDIRECTORY (launcher)
