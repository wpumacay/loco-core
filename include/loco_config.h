
#pragma once

#include <string>

#ifndef LOCO_BACKEND_GRAPHICS_GLVIZ_GLFW
    #if defined( _WIN32 )
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_GLFW "./libs/liblocoRenderingGlvizGLFW.dll"
    #elif defined( __APPLE__ )
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_GLFW "./libs/liblocoRenderingGlvizGLFW.dylib"
    #else
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_GLFW "./libs/liblocoRenderingGlvizGLFW.so"
    #endif
#endif

#ifndef LOCO_BACKEND_GRAPHICS_GLVIZ_EGL
    #if defined( _WIN32 )
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_EGL "./libs/liblocoRenderingGlvizEGL.dll"
    #elif defined( __APPLE__ )
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_EGL "./libs/liblocoRenderingGlvizEGL.dylib"
    #else
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_EGL "./libs/liblocoRenderingGlvizEGL.so"
    #endif
#endif

namespace loco {
namespace config {

    namespace rendering
    {
        const std::string NONE = "null"; // creates null-obj visualizer (dummy)
        const std::string GLVIZ_GLFW = LOCO_BACKEND_GRAPHICS_GLVIZ_GLFW;
        const std::string GLVIZ_EGL = LOCO_BACKEND_GRAPHICS_GLVIZ_EGL;
    }

    // Add available physics backend in this namespace
    namespace physics
    {
        const std::string NONE = "null"; // creates null-obj backend (dummy)
    }

}}