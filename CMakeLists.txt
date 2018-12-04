cmake_minimum_required( VERSION 3.0.0 )

project( tysoc )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}" )

file( GLOB_RECURSE TYSOC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include/" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/mjCpp/include/" ) # hack for now

add_library( tysoc
			 ${TYSOC_SOURCES} )
target_link_libraries( tysoc
                       mjcppnogl )
