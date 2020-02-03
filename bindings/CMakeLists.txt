message( "LOCOSIM::bindings >>> configuring build-process for python-bindings" )

include_directories( "${LOCO_CORE_INCLUDE_DIRS}" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

#### pybind11_add_module( locosim_bindings
####                      src/locosim_common_py.cpp
####                      src/locosim_data_py.cpp
####                      src/locosim_components_py.cpp )
#### target_link_libraries( locosim_bindings PRIVATE loco_core )