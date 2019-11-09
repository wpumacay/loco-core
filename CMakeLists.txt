cmake_minimum_required( VERSION 3.0.0 )

project( tysoc )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-g -Wno-format-security ${CMAKE_CXX_FLAGS}" )

# Add the cmake folder so the FindSphinx module is found
set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} )

set( TYSOCCORE_BUILD_DOCS OFF )

if ( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    message( "TYSOCCORE> Sending generated libs to OWN build directory" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs" )
else()
    message( "TYSOCCORE> Sending generated libs to EXTERNAL build directory" )
endif()

message( "TYSOCCORE> LIBRARY_OUT_DIR: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

set( TYSOCCORE_IS_MASTER_PROJECT OFF )
if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( "TYSOCCORE> Configuring as MASTER project" )
    set( TYSOCCORE_IS_MASTER_PROJECT ON )
else()
    message( "TYSOCCORE> Configuring as CHILD project" )
endif()

# Definitions used for development. Python bindings do not use these definitions, but ...
# instead we load the paths dynamically according to the user configuration/installation.
add_definitions( -DTYSOC_PATH_RESOURCES="${CMAKE_CURRENT_SOURCE_DIR}/res/" )
add_definitions( -DTYSOC_PATH_WORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/xml/" )
add_definitions( -DTYSOC_PATH_MJCF_TEMPLATES="${CMAKE_CURRENT_SOURCE_DIR}/res/templates/mjcf/" )
add_definitions( -DTYSOC_PATH_URDF_TEMPLATES="${CMAKE_CURRENT_SOURCE_DIR}/res/templates/urdf/" )
add_definitions( -DTYSOC_PATH_RLSIM_TEMPLATES="${CMAKE_CURRENT_SOURCE_DIR}/res/templates/rlsim/" )
add_definitions( -DTYSOC_PATH_MESHES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/meshes/" )
if ( WIN32 )
    add_definitions( -DTYSOC_BACKEND_GRAPHICS_GLVIZ="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libtysocRenderingGlviz.dll" )
elseif ( UNIX )
    if ( APPLE )
        add_definitions( -DTYSOC_BACKEND_GRAPHICS_GLVIZ="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libtysocRenderingGlviz.dylib" )
    else ()
        add_definitions( -DTYSOC_BACKEND_GRAPHICS_GLVIZ="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libtysocRenderingGlviz.so" )
    endif ()
endif()

set_property( GLOBAL PROPERTY PROP_TYSOC_PATH_RESOURCES         "${CMAKE_CURRENT_SOURCE_DIR}/res/" )
set_property( GLOBAL PROPERTY PROP_TYSOC_PATH_WORKING_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/res/xml/" )
set_property( GLOBAL PROPERTY PROP_TYSOC_PATH_MJCF_TEMPLATES    "${CMAKE_CURRENT_SOURCE_DIR}/res/templates/mjcf/" )
set_property( GLOBAL PROPERTY PROP_TYSOC_PATH_URDF_TEMPLATES    "${CMAKE_CURRENT_SOURCE_DIR}/res/templates/urdf/" )
set_property( GLOBAL PROPERTY PROP_TYSOC_PATH_RLSIM_TEMPLATES   "${CMAKE_CURRENT_SOURCE_DIR}/res/templates/rlsim/" )
set_property( GLOBAL PROPERTY PROP_TYSOC_PATH_MESHES_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/res/meshes/" )
if ( WIN32 )
    set_property( GLOBAL PROPERTY PROP_TYSOC_BACKEND_GRAPHICS_GLVIZ "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libtysocRenderingGlviz.dll" )
elseif ( UNIX )
    if ( APPLE )
        set_property( GLOBAL PROPERTY PROP_TYSOC_BACKEND_GRAPHICS_GLVIZ "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libtysocRenderingGlviz.dylib" )
    else ()
        set_property( GLOBAL PROPERTY PROP_TYSOC_BACKEND_GRAPHICS_GLVIZ "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libtysocRenderingGlviz.so" )
    endif ()
endif()

# test all properties --------------------------------------------------------------------------------------------------

get_property( VAR_TYSOC_PATH_RESOURCES          GLOBAL PROPERTY PROP_TYSOC_PATH_RESOURCES )
get_property( VAR_TYSOC_PATH_WORKING_DIR        GLOBAL PROPERTY PROP_TYSOC_PATH_WORKING_DIR )
get_property( VAR_TYSOC_PATH_MJCF_TEMPLATES     GLOBAL PROPERTY PROP_TYSOC_PATH_MJCF_TEMPLATES )
get_property( VAR_TYSOC_PATH_URDF_TEMPLATES     GLOBAL PROPERTY PROP_TYSOC_PATH_URDF_TEMPLATES )
get_property( VAR_TYSOC_PATH_RLSIM_TEMPLATES    GLOBAL PROPERTY PROP_TYSOC_PATH_RLSIM_TEMPLATES )
get_property( VAR_TYSOC_PATH_MESHES_DIR         GLOBAL PROPERTY PROP_TYSOC_PATH_MESHES_DIR )
get_property( VAR_TYSOC_BACKEND_GRAPHICS_GLVIZ  GLOBAL PROPERTY PROP_TYSOC_BACKEND_GRAPHICS_GLVIZ )

message( "PROP_TYSOC_PATH_RESOURCES:            ${VAR_TYSOC_PATH_RESOURCES}" )
message( "PROP_TYSOC_PATH_WORKING_DIR:          ${VAR_TYSOC_PATH_WORKING_DIR}" )
message( "PROP_TYSOC_PATH_MJCF_TEMPLATES:       ${VAR_TYSOC_PATH_MJCF_TEMPLATES}" )
message( "PROP_TYSOC_PATH_URDF_TEMPLATES:       ${VAR_TYSOC_PATH_URDF_TEMPLATES}" )
message( "PROP_TYSOC_PATH_RLSIM_TEMPLATES:      ${VAR_TYSOC_PATH_RLSIM_TEMPLATES}" )
message( "PROP_TYSOC_PATH_MESHES_DIR:           ${VAR_TYSOC_PATH_MESHES_DIR}" )
message( "PROP_TYSOC_BACKEND_GRAPHICS_GLVIZ:    ${VAR_TYSOC_BACKEND_GRAPHICS_GLVIZ}" )

# ----------------------------------------------------------------------------------------------------------------------

if ( TYSOCCORE_IS_MASTER_PROJECT )
    # Add dependencies from our own submodules
    add_subdirectory( ext )
endif()

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

# Configure python bindings
add_subdirectory( pytysoc )
## # Configure tests
## add_subdirectory( tests )
## # Configure unittests
## add_subdirectory( unittest )

if( TYSOCCORE_IS_MASTER_PROJECT )
    # Configure examples
    add_subdirectory( examples )
    
    # Check for docs
    if ( TYSOCCORE_BUILD_DOCS )
        # Configure documentation
        add_subdirectory( docs )
    endif()
endif()