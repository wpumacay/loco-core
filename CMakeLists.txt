cmake_minimum_required( VERSION 3.0.0 )

#### aka Tysoc-Core project
project( LocoCore )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-format-security" )
set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} )

# in case nobody set the project type, set it (parent project might have set it for all)
if ( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

option( LOCO_CORE_BUILD_WINDOWED_VISUALIZER "Build OpenGL-based windowed(GLFW) visualizer" ON )
option( LOCO_CORE_BUILD_HEADLESS_VISUALIZER "Build OpenGL-based headless(EGL) visualizer" OFF )
option( LOCO_CORE_BUILD_FFMPEG_VISUALIZER_UTILS "Build Ffmpeg-based utils to decode/encode videos using the visualizer" OFF )
option( LOCO_CORE_BUILD_DOCS "Build documentation (requires Doxygen)" OFF )
option( LOCO_CORE_BUILD_EXAMPLES "Build C/C++ examples" ON )
option( LOCO_CORE_BUILD_TESTS "Build C/C++ tests" ON )
option( LOCO_CORE_BUILD_PYTHON_BINDINGS "Build Python-bindings" ON )
option( LOCO_CORE_BUILD_WITH_LOGS "Build using logging functionality" ON )
option( LOCO_CORE_BUILD_WITH_PROFILING "Build using profiling tools enabled" OFF )
option( LOCO_CORE_BUILD_WITH_TRACK_ALLOCS "Build using tracking of objects allocations|deallocations" OFF )

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

if ( LOCO_CORE_BUILD_WITH_LOGS )
    set( LOCO_CORE_USE_LOGS "YES" )
    add_definitions( -DLOCO_CORE_USE_LOGS )
else()
    set( LOCO_CORE_USE_LOGS "NO" )
endif()

if ( LOCO_CORE_BUILD_WITH_PROFILING )
    set( LOCO_CORE_USE_PROFILING "YES" )
    add_definitions( -DLOCO_CORE_USE_PROFILING )
else()
    set( LOCO_CORE_USE_PROFILING "NO" )
endif()

if ( LOCO_CORE_BUILD_WITH_TRACK_ALLOCS )
    set( LOCO_CORE_USE_TRACK_ALLOCS "YES" )
    add_definitions( -DLOCO_CORE_USE_TRACK_ALLOCS )
else()
    set( LOCO_CORE_USE_TRACK_ALLOCS "NO" )
endif()

# Resources path: if not given by other project|setup-script, then use the default (this project's res folder location)
if ( NOT LOCO_CORE_RESOURCES_PATH )
    set( LOCO_CORE_RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res/" )
endif()
# Libraries path: if not given by other project|setup-script, then use the default (this project's build-libs folder location)
if ( NOT LOCO_CORE_LIBRARIES_PATH )
    set( LOCO_CORE_LIBRARIES_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/" )
endif()

# Expose definitions and properties for other projects to use (resources path, generated dynamic-library paths).
# Python-bindings write the paths (via the variables above) according to the package installation folder.
add_definitions( -DLOCO_PATH_RESOURCES="${LOCO_CORE_RESOURCES_PATH}" )
set_property( GLOBAL PROPERTY PROP_LOCO_PATH_RESOURCES "${LOCO_CORE_RESOURCES_PATH}" )
if ( WIN32 )
    add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_GLFW="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizGLFW.dll" )
    add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_EGL="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizEGL.dll" )
    add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_EDITOR="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizEditor.dll" )
    add_definitions( -DLOCO_BACKEND_PHYSICS_MUJOCO="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsMUJOCO.dll" )
    add_definitions( -DLOCO_BACKEND_PHYSICS_BULLET="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsBULLET.dll" )
    add_definitions( -DLOCO_BACKEND_PHYSICS_RAISIM="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsRAISIM.dll" )
    add_definitions( -DLOCO_BACKEND_PHYSICS_DART="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsDART.dll" )
elseif ( UNIX )
    if ( APPLE )
        add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_GLFW="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizGLFW.dylib" )
        add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_EGL="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizEGL.dylib" )
        add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_EDITOR="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizEditor.dylib" )
        add_definitions( -DLOCO_BACKEND_PHYSICS_MUJOCO="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsMUJOCO.dylib" )
        add_definitions( -DLOCO_BACKEND_PHYSICS_BULLET="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsBULLET.dylib" )
        add_definitions( -DLOCO_BACKEND_PHYSICS_RAISIM="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsRAISIM.dylib" )
        add_definitions( -DLOCO_BACKEND_PHYSICS_DART="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsDART.dylib" )
    else ()
        add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_GLFW="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizGLFW.so" )
        add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_EGL="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizEGL.so" )
        add_definitions( -DLOCO_BACKEND_GRAPHICS_GLVIZ_EDITOR="${LOCO_CORE_LIBRARIES_PATH}liblocoRenderingGlvizEditor.so" )
        add_definitions( -DLOCO_BACKEND_PHYSICS_MUJOCO="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsMUJOCO.so" )
        add_definitions( -DLOCO_BACKEND_PHYSICS_BULLET="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsBULLET.so" )
        add_definitions( -DLOCO_BACKEND_PHYSICS_RAISIM="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsRAISIM.so" )
        add_definitions( -DLOCO_BACKEND_PHYSICS_DART="${LOCO_CORE_LIBRARIES_PATH}liblocoPhysicsDART.so" )
    endif ()
endif()

# Test that properties point where they should -----------------------------------------------------
get_property( VAR_LOCO_PATH_RESOURCES GLOBAL PROPERTY PROP_LOCO_PATH_RESOURCES )

message( "PROP_LOCO_PATH_RESOURCES              : ${VAR_LOCO_PATH_RESOURCES}" )
message( "PROP_LOCO_BACKEND_GRAPHICS_GLVIZ_GLFW : ${VAR_LOCO_BACKEND_GRAPHICS_GLVIZ_GLFW}" )
message( "PROP_LOCO_BACKEND_GRAPHICS_GLVIZ_EGL  : ${VAR_LOCO_BACKEND_GRAPHICS_GLVIZ_EGL}" )
# --------------------------------------------------------------------------------------------------

# Add external dependencies only if master project. If not, parent project must configure it
if ( LOCO_CORE_IS_MASTER_PROJECT )
    add_subdirectory( ext )
endif()

# Assimp: required for some mesh-computations. (use same version as in TinyRenderer)
find_package( ASSIMP REQUIRED )

set( LOCO_CORE_SRCS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_common.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_math.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_object.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_data.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/loco_kinematic_tree_collider.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/loco_kinematic_tree_collider_adapter.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/loco_kinematic_tree_joint.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/loco_kinematic_tree_joint_adapter.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/loco_kinematic_tree_body.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/loco_kinematic_tree_body_adapter.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/loco_kinematic_tree.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/loco_kinematic_tree_adapter.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/parsers/loco_kinematic_tree_mjcf_parser.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/kinematic_trees/parsers/loco_kinematic_tree_urdf_parser.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/agent/formats/kintree_format_rlsim.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/loco_single_body.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/loco_single_body_primitives.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/loco_single_body_adapter.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/loco_single_body_collider.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/loco_single_body_collider_adapter.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/loco_single_body_constraint.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives/loco_single_body_constraint_adapter.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_scenario.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/sensor_base.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/types/sensor_agent_intrinsics.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/types/sensor_terrain_sections.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_simulation.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_runtime.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/terrain_base.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/sections/terrain_sections_base.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/sections/terrain_sections_blocky.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/sections/terrain_sections_connectedpath.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/sections/terrain_sections_connectedpath_profiles.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/types/static/terrain_static.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/terrain_wrapper.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/model_loader.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/rlsim/rlsim.cpp"
     #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/parsers/rlsim/rlsim_elements.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/loco_parsing_common.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/loco_parsing_schema.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/loco_parsing_element.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/visualizer/loco_visualizer_drawable.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/visualizer/loco_visualizer_camera.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/visualizer/loco_visualizer_light.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/visualizer/loco_visualizer.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/tinyxml2/tinyxml2.cpp"
)

set( LOCO_CORE_INCLUDE_DIRS
     "${CMAKE_CURRENT_SOURCE_DIR}/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/tinyxml2"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/json"
     "${CMAKE_SOURCE_DIR}/ext/spdlog/include"
     "${CMAKE_SOURCE_DIR}/ext/tiny_math/include"
     "${CMAKE_SOURCE_DIR}/ext/tiny_utils/include" )
include_directories( ${LOCO_CORE_INCLUDE_DIRS} )

add_library( loco_core SHARED
             ${LOCO_CORE_SRCS} )
target_link_libraries( loco_core
                       tinymath_cpp_lib
                       tinyutils_cpp_lib
                       assimp
                       spdlog
                       dl )

if ( LOCO_CORE_BUILD_WINDOWED_VISUALIZER OR LOCO_CORE_BUILD_HEADLESS_VISUALIZER )
    add_subdirectory( opengl_viz )
endif()

if ( LOCO_CORE_BUILD_PYTHON_BINDINGS )
    add_subdirectory( python/loco/bindings )
endif()

if ( LOCO_CORE_BUILD_DOCS )
    set_property( GLOBAL PROPERTY LOCO_INCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
    add_subdirectory( docs )
endif()

if ( LOCO_CORE_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()

if ( LOCO_CORE_BUILD_TESTS )
    enable_testing()
    add_subdirectory( tests )
endif()

if ( LOCO_CORE_IS_MASTER_PROJECT )
    message( "|---------------------------------------------------------|" )
    message( "|               LOCOMOTION SIMULATION TOOLKIT             |" )
    message( "|---------------------------------------------------------|" )
    message( "
                                   .--~~,__
                      :-....,-------`~~'._.'
                       `-,,,  ,_      ;'~U'
                        _,-' ,'`-__; '--.
                       (_/'~~      ''''(;" )
endif()

message( "***************************************************************************************" )
message( "LOCO::CORE >>> BUILD-OPTIONS SUMMARY:" )
message( "CMAKE_BUILD_TYPE                          : ${CMAKE_BUILD_TYPE}" )
message( "CMAKE_LIBRARY_OUTPUT_DIRECTORY            : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )
message( "CMAKE_BUILD_RPATH                         : ${CMAKE_BUILD_RPATH}" )
message( "CMAKE_INSTALL_RPATH                       : ${CMAKE_INSTALL_RPATH}" )
message( "LOCO_CORE_RESOURCES_PATH                  : ${LOCO_CORE_RESOURCES_PATH}" )
message( "LOCO_CORE_LIBRARIES_PATH                  : ${LOCO_CORE_LIBRARIES_PATH}" )
message( "LOCO_CORE_BUILD_WINDOWED_VISUALIZER       : ${LOCO_CORE_BUILD_WINDOWED_VISUALIZER}" )
message( "LOCO_CORE_BUILD_HEADLESS_VISUALIZER       : ${LOCO_CORE_BUILD_HEADLESS_VISUALIZER}" )
message( "LOCO_CORE_BUILD_FFMPEG_VISUALIZER_UTILS   : ${LOCO_CORE_BUILD_FFMPEG_VISUALIZER_UTILS}" )
message( "LOCO_CORE_BUILD_DOCS                      : ${LOCO_CORE_BUILD_DOCS}" )
message( "LOCO_CORE_BUILD_EXAMPLES                  : ${LOCO_CORE_BUILD_EXAMPLES}" )
message( "LOCO_CORE_BUILD_TESTS                     : ${LOCO_CORE_BUILD_TESTS}" )
message( "LOCO_CORE_BUILD_PYTHON_BINDINGS           : ${LOCO_CORE_BUILD_PYTHON_BINDINGS}" )
message( "LOCO_CORE_BUILD_WITH_LOGS                 : ${LOCO_CORE_BUILD_WITH_LOGS}" )
message( "LOCO_CORE_BUILD_WITH_PROFILING            : ${LOCO_CORE_BUILD_WITH_PROFILING}" )
message( "LOCO_CORE_BUILD_WITH_TRACK_ALLOCS         : ${LOCO_CORE_BUILD_WITH_TRACK_ALLOCS}" )
message( "LOCO_CORE_USE_LOGS                        : ${LOCO_CORE_USE_LOGS}" )
message( "LOCO_CORE_USE_PROFILING                   : ${LOCO_CORE_USE_PROFILING}" )
message( "LOCO_CORE_USE_TRACK_ALLOCS                : ${LOCO_CORE_USE_TRACK_ALLOCS}" )
message( "***************************************************************************************" )