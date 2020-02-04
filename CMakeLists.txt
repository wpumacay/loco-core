cmake_minimum_required( VERSION 3.0.0 )

#### aka Tysoc-Core project
project( LocoCore )
message( "|---------------------------------------------------------|" )
message( "|               LOCOMOTION SIMULATION TOOLKIT             |" )
message( "|---------------------------------------------------------|" )
message( "
                               .--~~,__
                  :-....,-------`~~'._.'
                   `-,,,  ,_      ;'~U'
                    _,-' ,'`-__; '--.
                   (_/'~~      ''''(;" )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-format-security" )
set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} )

# in case nobody set the project type, set it (parent project might have set it for all)
if ( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

option( LOCO_CORE_BUILD_DOCS "Build documentation (requires Doxygen)" ON )
option( LOCO_CORE_BUILD_EXAMPLES "Build C/C++ examples" ON )
option( LOCO_CORE_BUILD_PYTHON_BINDINGS "Build Python-bindings" ON )
option( LOCO_CORE_BUILD_WITH_LOGS "Build using logging functionality" ON )
option( LOCO_CORE_BUILD_WITH_TRACK_ALLOCS "Build using tracking of objects allocations|deallocations" ON )

if ( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    message( "LOCO::CORE >>> Sending generated libs to OWN build directory" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs" )
else()
    message( "LOCO::CORE >>> Sending generated libs to EXTERNAL build directory" )
endif()
message( "LOCO::CORE >>> LIBRARY_OUT_DIR: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

set( LOCO_CORE_IS_MASTER_PROJECT OFF )
if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( "LOCO::CORE >>> Configuring as MASTER project" )
    set( LOCO_CORE_IS_MASTER_PROJECT ON )
else()
    message( "LOCO::CORE >>> Configuring as CHILD project" )
endif()

if ( LOCO_CORE_BUILD_WITH_LOGS OR ( CMAKE_BUILD_TYPE STREQUAL Debug ) )
    set( LOCO_CORE_USE_LOGS "YES" )
    add_definitions( -DLOCO_CORE_USE_LOGS )
else()
    set( LOCO_CORE_USE_LOGS "NO" )
endif()

if ( LOCO_CORE_BUILD_WITH_TRACK_ALLOCS OR ( CMAKE_BUILD_TYPE STREQUAL Debug ) )
    set( LOCO_CORE_USE_TRACK_ALLOCS "YES" )
    add_definitions( -DLOCO_CORE_USE_TRACK_ALLOCS )
else()
    set( LOCO_CORE_USE_TRACK_ALLOCS "NO" )
endif()

# Expose definitions and properties for other projects to se (resources path, generated dynamic-library paths)
# Python-bindings don't use these paths, but the ones obtained during installation (target folder)
add_definitions( -DLOCO_PATH_RESOURCES="${CMAKE_CURRENT_SOURCE_DIR}/res/" )
set_property( GLOBAL PROPERTY PROP_LOCO_PATH_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/res/" )
if ( WIN32 )
    add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/liblocoRenderingGlviz.dll" )
    set_property( GLOBAL PROPERTY PROP_LOCO_BACKEND_GRAPHICS_GLVIZ "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/liblocoRenderingGlviz.dll" )
elseif ( UNIX )
    if ( APPLE )
        add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/liblocoRenderingGlviz.dylib" )
        set_property( GLOBAL PROPERTY PROP_LOCO_BACKEND_GRAPHICS_GLVIZ "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/liblocoRenderingGlviz.dylib" )
    else ()
        add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/liblocoRenderingGlviz.so" )
        set_property( GLOBAL PROPERTY PROP_LOCO_BACKEND_GRAPHICS_GLVIZ "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/liblocoRenderingGlviz.so" )
    endif ()
endif()

# Test that properties point where they should -----------------------------------------------------
get_property( VAR_LOCO_PATH_RESOURCES GLOBAL PROPERTY PROP_LOCO_PATH_RESOURCES )
get_property( VAR_LOCO_BACKEND_GRAPHICS_GLVIZ GLOBAL PROPERTY PROP_LOCO_BACKEND_GRAPHICS_GLVIZ )

message( "PROP_LOCO_PATH_RESOURCES           : ${VAR_LOCO_PATH_RESOURCES}" )
message( "PROP_LOCO_BACKEND_GRAPHICS_GLVIZ   : ${VAR_LOCO_BACKEND_GRAPHICS_GLVIZ}" )
# --------------------------------------------------------------------------------------------------

if ( LOCO_CORE_IS_MASTER_PROJECT )
    add_subdirectory( ext )
endif()

# Assimp: required for some mesh-computations. (use same version as in TinyRenderer)
find_package( ASSIMP REQUIRED )

set( LOCO_CORE_SRCS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_common.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_math.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_logger.cpp"
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
     "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/single_body.cpp"
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
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/tinyxml2/tinyxml2.cpp"
)

set( LOCO_CORE_INCLUDE_DIRS
     "${CMAKE_CURRENT_SOURCE_DIR}/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/tinyxml2"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/json"
     "${CMAKE_SOURCE_DIR}/ext/spdlog/include"
     "${CMAKE_SOURCE_DIR}/ext/tinymath/include" )
include_directories( ${LOCO_CORE_INCLUDE_DIRS} )

add_library( loco_core SHARED
             ${LOCO_CORE_SRCS} )
target_link_libraries( loco_core
                       tinymath_cpp_lib
                       assimp
                       spdlog
                       dl )

#### # Configure glviz @todo: add option to select if visualizer is enabled or not
#### add_subdirectory( glviz )

if ( LOCO_CORE_BUILD_PYTHON_BINDINGS )
    add_subdirectory( bindings )
endif()

#### if ( LOCO_CORE_IS_MASTER_PROJECT AND LOCO_CORE_BUILD_DOCS )
####     add_subdirectory( docs )
#### endif()

#### if ( LOCO_CORE_IS_MASTER_PROJECT AND LOCO_CORE_BUILD_EXAMPLES )
####     add_subdirectory( examples )
#### endif()

message( "***************************************************************************************" )
message( "LOCO::CORE >>> BUILD-OPTIONS SUMMARY:" )
message( "CMAKE_BUILD_TYPE                  : ${CMAKE_BUILD_TYPE}" )
message( "LOCO_CORE_BUILD_DOCS              : ${LOCO_CORE_BUILD_DOCS}" )
message( "LOCO_CORE_BUILD_EXAMPLES          : ${LOCO_CORE_BUILD_EXAMPLES}" )
message( "LOCO_CORE_BUILD_PYTHON_BINDINGS   : ${LOCO_CORE_BUILD_PYTHON_BINDINGS}" )
message( "LOCO_CORE_BUILD_WITH_LOGS         : ${LOCO_CORE_BUILD_WITH_LOGS}" )
message( "LOCO_CORE_BUILD_WITH_TRACK_ALLOCS : ${LOCO_CORE_BUILD_WITH_TRACK_ALLOCS}" )
message( "LOCO_CORE_USE_LOGS                : ${LOCO_CORE_USE_LOGS}" )
message( "LOCO_CORE_USE_TRACK_ALLOCS        : ${LOCO_CORE_USE_TRACK_ALLOCS}" )
message( "***************************************************************************************" )