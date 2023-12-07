if(NOT TARGET loco::core)
  return()
endif()

set(LOCO_CORE_EXAMPLES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/example_00_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_01_drawable.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/example_02_scenario.cpp
)

foreach(example_filepath IN LISTS LOCO_CORE_EXAMPLES_LIST)
  loco_setup_single_file_example(${example_filepath} TARGET_DEPENDENCIES
                                 loco::core)
endforeach()
