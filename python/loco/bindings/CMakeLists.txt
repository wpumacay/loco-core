# ~~~
# CMake configuration rules used to generate Python Bindings via Pybind11
# ~~~
if(NOT TARGET loco::core)
  loco_message("Bindings require the 'loco::core' target" LOG_LEVEL FATAL_ERROR)
  return()
endif()
if(NOT TARGET math::math_py_helpers)
  loco_message("Bindings requires the 'math::math_py_helpers' target" LOG_LEVEL
               FATAL_ERROR)
  return()
endif()

set(BINDINGS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/body_py.cpp)

pybind11_add_module(loco MODULE ${BINDINGS_SOURCES})
target_include_directories(loco PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(loco PRIVATE loco::core math::math_py_helpers)
