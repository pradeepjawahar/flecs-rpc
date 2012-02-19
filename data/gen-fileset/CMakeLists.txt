INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

ADD_EXECUTABLE ( gen-fileset gen-fileset.cpp )

TARGET_LINK_LIBRARIES (gen-fileset ${Boost_LIBRARIES})
