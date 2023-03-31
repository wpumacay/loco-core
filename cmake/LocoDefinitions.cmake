include_guard()

if(NOT TARGET LocoCoreCpp)
  return()
endif()

# TODO(wilbert): find an option to get whether or not bullet was built using
# double precision, as we currently have to manually set this and expect that it
# indeed was compiled with those options

if(LOCO_BUILD_LOGS)
  target_compile_definitions(LocoCoreCpp PUBLIC -DLOCO_LOGS_ENABLED)
endif()

if(LOCO_BUILD_PROFILING)
  target_compile_definitions(LocoCoreCpp PUBLIC -DLOCO_PROFILING_ENABLED)
endif()

if(LOCO_BUILD_BACKEND_MUJOCO)
  target_compile_definitions(LocoCoreCpp PUBLIC -DLOCO_MUJOCO_ENABLED)
endif()

if(LOCO_BUILD_BACKEND_BULLET)
  target_compile_definitions(LocoCoreCpp PUBLIC -DLOCO_BULLET_ENABLED)
  # Hint that Bullet was compiled using double precision
  target_compile_definitions(LocoCoreCpp PUBLIC -DBT_USE_DOUBLE_PRECISION)
endif()

if(LOCO_BUILD_BACKEND_DART)
  target_compile_definitions(LocoCoreCpp PUBLIC -DLOCO_DART_ENABLED)
endif()
