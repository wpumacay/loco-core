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

#ifndef LOCO_BACKEND_GRAPHICS_GLVIZ_EDITOR
    #if defined( _WIN32 )
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_EDITOR "./libs/liblocoRenderingGlvizEditor.dll"
    #elif defined( __APPLE__ )
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_EDITOR "./libs/liblocoRenderingGlvizEditor.dylib"
    #else
        #define LOCO_BACKEND_GRAPHICS_GLVIZ_EDITOR "./libs/liblocoRenderingGlvizEditor.so"
    #endif
#endif

#ifndef LOCO_BACKEND_PHYSICS_MUJOCO
    #if defined( _WIN32 )
        #define LOCO_BACKEND_PHYSICS_MUJOCO "./libs/liblocoPhysicsMUJOCO.dll"
    #elif defined( __APPLE__ )
        #define LOCO_BACKEND_PHYSICS_MUJOCO "./libs/liblocoPhysicsMUJOCO.dylib"
    #else
        #define LOCO_BACKEND_PHYSICS_MUJOCO "./libs/liblocoPhysicsMUJOCO.so"
    #endif
#endif

#ifndef LOCO_BACKEND_PHYSICS_BULLET
    #if defined( _WIN32 )
        #define LOCO_BACKEND_PHYSICS_BULLET "./libs/liblocoPhysicsBULLET.dll"
    #elif defined( __APPLE__ )
        #define LOCO_BACKEND_PHYSICS_BULLET "./libs/liblocoPhysicsBULLET.dylib"
    #else
        #define LOCO_BACKEND_PHYSICS_BULLET "./libs/liblocoPhysicsBULLET.so"
    #endif
#endif

#ifndef LOCO_BACKEND_PHYSICS_RAISIM
    #if defined( _WIN32 )
        #define LOCO_BACKEND_PHYSICS_RAISIM "./libs/liblocoPhysicsRAISIM.dll"
    #elif defined( __APPLE__ )
        #define LOCO_BACKEND_PHYSICS_RAISIM "./libs/liblocoPhysicsRAISIM.dylib"
    #else
        #define LOCO_BACKEND_PHYSICS_RAISIM "./libs/liblocoPhysicsRAISIM.so"
    #endif
#endif

#ifndef LOCO_BACKEND_PHYSICS_DART
    #if defined( _WIN32 )
        #define LOCO_BACKEND_PHYSICS_DART "./libs/liblocoPhysicsDART.dll"
    #elif defined( __APPLE__ )
        #define LOCO_BACKEND_PHYSICS_DART "./libs/liblocoPhysicsDART.dylib"
    #else
        #define LOCO_BACKEND_PHYSICS_DART "./libs/liblocoPhysicsDART.so"
    #endif
#endif

namespace loco {
namespace config {

    namespace rendering
    {
        const std::string NONE = "null"; // creates null-obj visualizer (dummy)
        const std::string GLVIZ_GLFW = LOCO_BACKEND_GRAPHICS_GLVIZ_GLFW;
        const std::string GLVIZ_EGL = LOCO_BACKEND_GRAPHICS_GLVIZ_EGL;
        const std::string GLVIZ_EDITOR = LOCO_BACKEND_GRAPHICS_GLVIZ_EDITOR;
    }

    // Add available physics backend in this namespace
    namespace physics
    {
        const std::string NONE = "null"; // null-obj backend (dummy)
        const std::string MUJOCO = LOCO_BACKEND_PHYSICS_MUJOCO; // mujoco-obj backend
        const std::string BULLET = LOCO_BACKEND_PHYSICS_BULLET; // bullet-obj backend
        const std::string RAISIM = LOCO_BACKEND_PHYSICS_RAISIM; // raisim-obj backend
        const std::string DART = LOCO_BACKEND_PHYSICS_DART; // dart-obj backend
    }

}}