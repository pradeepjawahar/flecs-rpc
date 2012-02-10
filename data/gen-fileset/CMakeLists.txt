CMAKE_MINIMUM_REQUIRED (VERSION 2.6 FATAL_ERROR)

FIND_PACKAGE( Boost
		COMPONENTS filesystem system )
IF (NOT Boost_FOUND)
	message(FATAL_ERROR "Boost not found!")
ENDIF()

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

LIST (APPEND CMAKE_CXX_FLAGS "-Wall -Werror")

ADD_EXECUTABLE ( gen-fileset gen-fileset.cpp )

TARGET_LINK_LIBRARIES (gen-fileset ${Boost_LIBRARIES})
