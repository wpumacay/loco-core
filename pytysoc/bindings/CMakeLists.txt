
set( CMAKE_CXX_FLAGS " -g ${CMAKE_CXX_FLAGS}" )

# includes from bindings folder
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
# includes from tysoc root folder
include_directories( "${CMAKE_SOURCE_DIR}/include" )

pybind11_add_module( tysoc_bindings
                     src/pytysoc_common.cpp
                     src/pytysoc_tests.cpp
                     src/pytysoc_agent_core.cpp
                     src/pytysoc_terrainGen.cpp
                     src/pytysoc_sensors.cpp
                     src/pytysoc_scenario.cpp
                     src/pytysoc_simulation.cpp
                     src/pytysoc_visualizer.cpp
                     src/pytysoc_runtime.cpp
                     src/pytysoc_bindings.cpp )
target_link_libraries( tysoc_bindings PRIVATE
                       tysocCore )