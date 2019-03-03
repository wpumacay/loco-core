cmake_minimum_required( VERSION 3.0.0 )

project( tysocCustomViz )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}" )

file( GLOB_RECURSE GLVIZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../include" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../ext/cat1/include" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../ext/json" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../ext/imgui" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../ext/imgui/examples" )

add_library( tysocRenderingGlviz SHARED
             ${GLVIZ_SOURCES} )
target_link_libraries( tysocRenderingGlviz
                       cat1
                       imgui
                       tysocCore )