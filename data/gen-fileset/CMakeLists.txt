FIND_PACKAGE( Boost
		COMPONENTS filesystem system )
IF (NOT Boost_FOUND)
	message(FATAL_ERROR "Boost not found!")
ENDIF()

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

ADD_EXECUTABLE ( gen-fileset gen-fileset.cpp )

TARGET_LINK_LIBRARIES (gen-fileset ${Boost_LIBRARIES})
