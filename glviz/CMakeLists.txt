cmake_minimum_required( VERSION 3.0.0 )

project( tysocCustomViz )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}" )

file( GLOB_RECURSE GLVIZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
# include headers from own repo
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../include" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../ext/json" )
# include headers from root repo (could be our own, or another repo)
include_directories( "${CMAKE_SOURCE_DIR}/ext/tiny_math/include" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/tiny_renderer/include" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/imgui" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/imgui/examples" )

# Apple kind of needs to specify this (at least in the system I had available, ...
# which was a brand new macbookair from a friend)
if ( APPLE )
    include_directories( /usr/local/include )
    link_directories( /usr/local/lib )
endif()

add_library( tysocRenderingGlviz SHARED
             ${GLVIZ_SOURCES} )
target_link_libraries( tysocRenderingGlviz
                       cat1
                       imgui
                       tysocCore )