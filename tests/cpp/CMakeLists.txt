# ~~~
# CMake configuration for C++ unittests
# ~~~
if(NOT TARGET loco::core)
  loco_message("Unittests require target [loco::core], but it wasn't found"
               LOG_LEVEL WARNING)
  return()
endif()

if(NOT TARGET Catch2::Catch2)
  loco_message("Unittests require target [Catch2::Catch2], but it wasn't found"
               LOG_LEVEL WARNING)
  return()
endif()

include(CTest)
include(Catch)

add_executable(
  LocoCppTests
  ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_common_mesh_data.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_common_hfield_data.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_common.cpp)
target_link_libraries(LocoCppTests PRIVATE loco::core Catch2::Catch2)
target_include_directories(LocoCppTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# Discover tets and pick an integer as the random seed
catch_discover_tests(LocoCppTests)
