cmake_minimum_required( VERSION 3.0.0 )

project( tysocterrain )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}" )

file( GLOB_RECURSE TYSOC_TERRAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include/" )

add_library( tysocterrain
			 ${TYSOC_TERRAIN_SOURCES} )
