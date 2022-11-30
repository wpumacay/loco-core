include_guard()

if(NOT TARGET LocoCoreCpp)
  return()
endif()

if(LOCO_BUILD_LOGS)
  target_compile_definitions(LocoCoreCpp PUBLIC -DLOCO_LOGS_ENABLED)
endif()

if(LOCO_BUILD_PROFILING)
  target_compile_definitions(LocoCoreCpp PUBLIC -DLOCO_PROFILING_ENABLED)
endif()
