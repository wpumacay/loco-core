message( "LOCOSIM::bindings >>> configuring build-process for python-bindings" )

include_directories( "${LOCO_CORE_INCLUDE_DIRS}" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/tiny_math/python/include" )

pybind11_add_module( loco_sim
                     loco_bindings_py.cpp
                     loco_common_py.cpp
                     components/loco_data_py.cpp
                     components/loco_components_py.cpp
                     primitives/loco_primitives_py.cpp
                     loco_runtime_py.cpp )
target_link_libraries( loco_sim PRIVATE loco_core tinymath_python_lib )