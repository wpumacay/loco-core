cmake_minimum_required( VERSION 3.0.0 )

project( tysoc )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}" )

add_definitions( -DTYSOCCORE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/" )

# Main Sources
file( GLOB_RECURSE TYSOC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
# External dependencies
set( TYSOC_SOURCES 
     ${TYSOC_SOURCES}
     "ext/tinyxml2/tinyxml2.cpp" )

# Main includes
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include/" )
# External dependencies
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/ext/tinyxml2" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/ext/json" )

add_library( tysocCore
			 ${TYSOC_SOURCES} )

# Configure tests
add_subdirectory( tests )
# Configure unittests
add_subdirectory( unittest )