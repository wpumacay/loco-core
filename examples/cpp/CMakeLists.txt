if(NOT TARGET loco::core)
  return()
endif()

# cmake-format: off
set(LOCO_CORE_EXAMPLES_LIST
    ##${CMAKE_CURRENT_SOURCE_DIR}/example_01_scenario.cpp
)
# cmake-format: on

foreach(example_filepath IN LISTS LOCO_CORE_EXAMPLES_LIST)
  loco_setup_single_file_example(${example_filepath} TARGET_DEPENDENCIES
                                 loco::core)
endforeach()
