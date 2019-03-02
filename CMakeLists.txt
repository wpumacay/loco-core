cmake_minimum_required( VERSION 3.0.0 )

project( tysoc )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}" )

## set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs" )

# Definitions used for development. Python bindings do not use these definitions, but ...
# instead we load the paths dynamically according to the user configuration/installation.
add_definitions( -DTYSOCCORE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/" )
add_definitions( -DTYSOCCORE_WORKING_DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/xml/" )
add_definitions( -DTYSOCCORE_MJCF_TEMPLATES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/templates/mjcf/" )
add_definitions( -DTYSOCCORE_URDF_TEMPLATES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/templates/urdf/" )
add_definitions( -DTYSOCCORE_RLSIM_TEMPLATES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/templates/rlsim/" )
add_definitions( -DTYSOCCORE_GLVIZ_LIB="${CMAKE_BINARY_DIR}/glviz/libtysocGlviz.so" )

add_subdirectory( ext )

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

add_library( tysocCore SHARED
			 ${TYSOC_SOURCES} )
target_link_libraries( tysocCore
                       dl )

# Configure glviz
add_subdirectory( glviz )
# Configure pybind11
add_subdirectory( pybind11 )
# Configure python bindings
add_subdirectory( pytysoc )
# Configure tests
add_subdirectory( tests )
# Configure unittests
add_subdirectory( unittest )