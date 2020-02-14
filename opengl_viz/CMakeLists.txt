message( "LOCO::CORE::opengl_viz >>> Configuring OpenGL-based visualizer" )

include_directories( "${LOCO_CORE_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/tiny_renderer/include" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/imgui" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/imgui/examples" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set( LOCO_VISUALIZER_OPENGL_SRCS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_visualizer_opengl_common.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_visualizer_opengl_camera.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_visualizer_opengl_light.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_visualizer_opengl_drawable.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/loco_visualizer_opengl.cpp" )

if ( LOCO_CORE_BUILD_WINDOWED_VISUALIZER )
    add_library( locoRenderingGlvizGLFW SHARED
                 ${LOCO_VISUALIZER_OPENGL_SRCS} )
    target_link_libraries( locoRenderingGlvizGLFW
                           tinyrenderer_cpp_lib_glfw
                           loco_core
                           imgui )
    target_compile_definitions( locoRenderingGlvizGLFW PRIVATE LOCO_OPENGL_VISUALIZER_WINDOWED=1 )
endif()

if ( LOCO_CORE_BUILD_HEADLESS_VISUALIZER )
    add_library( locoRenderingGlvizEGL SHARED
                 ${LOCO_VISUALIZER_OPENGL_SRCS} )
    target_link_libraries( locoRenderingGlvizEGL
                           tinyrenderer_cpp_lib_egl
                           loco_core )
    target_compile_definitions( locoRenderingGlvizEGL PRIVATE LOCO_OPENGL_VISUALIZER_HEADLESS=1 )
endif()