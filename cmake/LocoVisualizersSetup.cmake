include_guard()

if(NOT TARGET LocoCoreCpp)
  return()
endif()

# -------------------------------------
# Add sources according to the platforms that are enabled by the user
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/loco)

if(LOCO_BUILD_VISUALIZER_OPENGL)
  target_sources(
    LocoCoreCpp
    PRIVATE ${SOURCE_DIR}/visualizers/opengl/common_opengl.cpp
            ${SOURCE_DIR}/visualizers/opengl/visualizer_impl_opengl.cpp)
  target_link_libraries(LocoCoreCpp PUBLIC renderer::renderer)
  target_compile_definitions(LocoCoreCpp
                             PUBLIC -DLOCO_VISUALIZER_OPENGL_ENABLED)
endif()

if(LOCO_BUILD_VISUALIZER_MESHCAT)
  target_sources(
    LocoCoreCpp
    PRIVATE ${SOURCE_DIR}/visualizers/meshcat/common_meshcat.cpp
            ${SOURCE_DIR}/visualizers/meshcat/visualizer_impl_meshcat.cpp)
  target_link_libraries(LocoCoreCpp PUBLIC MeshcatCpp::MeshcatCpp)
  target_compile_definitions(LocoCoreCpp
                             PUBLIC -DLOCO_VISUALIZER_MESHCAT_ENABLED)
endif()
