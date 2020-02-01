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
get_property( VAR_TYSOC_BACKEND_GRAPHICS_GLVIZ  GLOBAL PROPERTY PROP_TYSOC_BACKEND_GRAPHICS_GLVIZ )

message( "PROP_TYSOC_PATH_RESOURCES:            ${VAR_TYSOC_PATH_RESOURCES}" )
message( "PROP_TYSOC_BACKEND_GRAPHICS_GLVIZ:    ${VAR_TYSOC_BACKEND_GRAPHICS_GLVIZ}" )

# ----------------------------------------------------------------------------------------------------------------------

if ( TYSOCCORE_IS_MASTER_PROJECT )
    # Add dependencies from our own submodules
    add_subdirectory( ext )
endif()

# Assimp: required for some mesh-computations. (use same version as in cat1-viz)
find_package( ASSIMP REQUIRED )

# Main Sources
#### file( GLOB_RECURSE TYSOC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
#### foreach(file ${TYSOC_SOURCES})
####     message( "src-file: ${file}" )
#### endforeach(file)
set( TYSOC_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/tysoc_common.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/tysoc_math.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/tysoc_logger.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/agent/agent_base.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/agent/agent_components.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/agent/formats/kintree_format_mjcf.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/agent/formats/kintree_format_rlsim.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/agent/formats/kintree_format_urdf.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/agent_wrapper.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/components/data.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/components/collision.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/components/visual.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/components/joint.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/components/body.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/compounds/compound.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/compounds/compound_body.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/model_loader.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/single_body.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/scenario.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/sensor_base.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/types/sensor_agent_intrinsics.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/types/sensor_terrain_sections.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/simulation_base.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/terrain_base.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/sections/terrain_sections_base.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/sections/terrain_sections_blocky.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/sections/terrain_sections_connectedpath.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/sections/terrain_sections_connectedpath_profiles.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/static/terrain_static.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain_wrapper.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/mjcf/mjcf.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/mjcf/mjcf_common.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/mjcf/mjcf_elements.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/parsers_common.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/parsers_json.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/parsers_xml.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/rlsim/rlsim.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/rlsim/rlsim_elements.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/urdf/urdf.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/urdf/urdf_elements.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/primitives_spawner.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/viz/viz.cpp"
)
# External dependencies
set( TYSOC_SOURCES 
     ${TYSOC_SOURCES}
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/tinyxml2/tinyxml2.cpp" )

# Main includes
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include/" )
# External dependencies
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/ext/tinyxml2" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/ext/json" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/spdlog/include" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/tiny_math/include" )

add_library( tysocCore SHARED
             ${TYSOC_SOURCES} )

target_link_libraries( tysocCore
                       tinymath_cpp_lib
                       assimp
                       spdlog
                       dl )

#### # Configure glviz @todo: add option to select if visualizer is enabled or not
#### add_subdirectory( glviz )

#### # Configure python bindings
#### add_subdirectory( pytysoc )

#### # Configure tests
#### add_subdirectory( tests )
#### # Configure unittests
#### add_subdirectory( unittest )

if( TYSOCCORE_IS_MASTER_PROJECT )
    #### # Configure examples
    #### add_subdirectory( examples )
    
    #### # Check for docs
    #### if ( TYSOCCORE_BUILD_DOCS )
    ####     # Configure documentation
    ####     add_subdirectory( docs )
    #### endif()
endif()