
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_scenario.h>
#include <viz/viz.h>

namespace py = pybind11;

namespace pytysoc
{

    class PyVisualizer
    {

        private :

        tysoc::TIVisualizer* m_visualizerPtr;

        public :

        /**
        *   Creates a wrapper for a requested visualizer. Objects from this ...
        *   class can only be created through the runtime (the same for the sim.) ...
        *   in the same way TIVisualizer objects are created in C/C++.
        *
        *   @param pyScenarioPtr    The scenario we want to visualize
        *   @notexposed             Not exposed through python API
        */
        PyVisualizer( tysoc::TIVisualizer* visualizerPtr );

        /**
        *   Destructor for this wrapper. It just cleans some references, as the ...
        *   owner of the data is the creator, which in our case is the runtime.
        */
        ~PyVisualizer();

        /**
        *   Requests the initialization of the wrapped visualizer object, and ...
        *   returns whether or not the initialization was successful
        *
        *   @exposed    Exposed through python API
        */
        bool initialize();

        /**
        *   Requests the wrapped visualizer to render the scene
        *
        *   @exposed    Exposed through python API
        */
        void render();

        /**
        *   Checks if the visualizer should close
        *
        *   @exposed    Exposed through python API
        */
        bool isActive();

        /**
        *   Checks if the given key is currently being pressed
        *   
        *   @param keyCode  Int representing the key the user wants to query
        *   @exposed        Exposed through python API
        */
        bool isKeyDown( int keyCode );

        /**
        *   Checks if a given key has been pressed, and clears immediately ...
        *   the key state to listen for just a single press (like a latch of a switch)
        *
        *   @param keyCode  Int representing the key the user wants to query
        *   @exposed        Exposed through python API
        */
        bool checkSingleKeyPress( int keyCode );
    };


}

#define PYTYSOC_VISUALIZER_BINDINGS(m) \
    py::class_<pytysoc::PyVisualizer>( m, "PyVisualizer" ) \
        .def( "initialize", &pytysoc::PyVisualizer::initialize ) \
        .def( "render", &pytysoc::PyVisualizer::render ) \
        .def( "isActive", &pytysoc::PyVisualizer::isActive ) \
        .def( "isKeyDown", &pytysoc::PyVisualizer::isKeyDown ) \
        .def( "checkSingleKeyPress", &pytysoc::PyVisualizer::checkSingleKeyPress );

#define PYTYSOC_VISUALIZER_KEYCODES(m) \
    m.attr( "KEY_SPACE" )           = py::int_( tysoc::keys::KEY_SPACE ); \
    m.attr( "KEY_APOSTROPHE" )      = py::int_( tysoc::keys::KEY_APOSTROPHE ); \
    m.attr( "KEY_COMMA" )           = py::int_( tysoc::keys::KEY_COMMA ); \
    m.attr( "KEY_MINUS" )           = py::int_( tysoc::keys::KEY_MINUS ); \
    m.attr( "KEY_PERIOD" )          = py::int_( tysoc::keys::KEY_PERIOD ); \
    m.attr( "KEY_SLASH" )           = py::int_( tysoc::keys::KEY_SLASH ); \
    m.attr( "KEY_0" )               = py::int_( tysoc::keys::KEY_0 ); \
    m.attr( "KEY_1" )               = py::int_( tysoc::keys::KEY_1 ); \
    m.attr( "KEY_2" )               = py::int_( tysoc::keys::KEY_2 ); \
    m.attr( "KEY_3" )               = py::int_( tysoc::keys::KEY_3 ); \
    m.attr( "KEY_4" )               = py::int_( tysoc::keys::KEY_4 ); \
    m.attr( "KEY_5" )               = py::int_( tysoc::keys::KEY_5 ); \
    m.attr( "KEY_6" )               = py::int_( tysoc::keys::KEY_6 ); \
    m.attr( "KEY_7" )               = py::int_( tysoc::keys::KEY_7 ); \
    m.attr( "KEY_8" )               = py::int_( tysoc::keys::KEY_8 ); \
    m.attr( "KEY_9" )               = py::int_( tysoc::keys::KEY_9 ); \
    m.attr( "KEY_SEMICOLON" )       = py::int_( tysoc::keys::KEY_SEMICOLON ); \
    m.attr( "KEY_EQUAL" )           = py::int_( tysoc::keys::KEY_EQUAL ); \
    m.attr( "KEY_A" )               = py::int_( tysoc::keys::KEY_A ); \
    m.attr( "KEY_B" )               = py::int_( tysoc::keys::KEY_B ); \
    m.attr( "KEY_C" )               = py::int_( tysoc::keys::KEY_C ); \
    m.attr( "KEY_D" )               = py::int_( tysoc::keys::KEY_D ); \
    m.attr( "KEY_E" )               = py::int_( tysoc::keys::KEY_E ); \
    m.attr( "KEY_F" )               = py::int_( tysoc::keys::KEY_F ); \
    m.attr( "KEY_G" )               = py::int_( tysoc::keys::KEY_G ); \
    m.attr( "KEY_H" )               = py::int_( tysoc::keys::KEY_H ); \
    m.attr( "KEY_I" )               = py::int_( tysoc::keys::KEY_I ); \
    m.attr( "KEY_J" )               = py::int_( tysoc::keys::KEY_J ); \
    m.attr( "KEY_K" )               = py::int_( tysoc::keys::KEY_K ); \
    m.attr( "KEY_L" )               = py::int_( tysoc::keys::KEY_L ); \
    m.attr( "KEY_M" )               = py::int_( tysoc::keys::KEY_M ); \
    m.attr( "KEY_N" )               = py::int_( tysoc::keys::KEY_N ); \
    m.attr( "KEY_O" )               = py::int_( tysoc::keys::KEY_O ); \
    m.attr( "KEY_P" )               = py::int_( tysoc::keys::KEY_P ); \
    m.attr( "KEY_Q" )               = py::int_( tysoc::keys::KEY_Q ); \
    m.attr( "KEY_R" )               = py::int_( tysoc::keys::KEY_R ); \
    m.attr( "KEY_S" )               = py::int_( tysoc::keys::KEY_S ); \
    m.attr( "KEY_T" )               = py::int_( tysoc::keys::KEY_T ); \
    m.attr( "KEY_U" )               = py::int_( tysoc::keys::KEY_U ); \
    m.attr( "KEY_V" )               = py::int_( tysoc::keys::KEY_V ); \
    m.attr( "KEY_W" )               = py::int_( tysoc::keys::KEY_W ); \
    m.attr( "KEY_X" )               = py::int_( tysoc::keys::KEY_X ); \
    m.attr( "KEY_Y" )               = py::int_( tysoc::keys::KEY_Y ); \
    m.attr( "KEY_Z" )               = py::int_( tysoc::keys::KEY_Z ); \
    m.attr( "KEY_LEFT_BRACKET" )    = py::int_( tysoc::keys::KEY_LEFT_BRACKET ); \
    m.attr( "KEY_BACKSLASH" )       = py::int_( tysoc::keys::KEY_BACKSLASH ); \
    m.attr( "KEY_RIGHT_BRACKET" )   = py::int_( tysoc::keys::KEY_RIGHT_BRACKET ); \
    m.attr( "KEY_GRAVE_ACCENT" )    = py::int_( tysoc::keys::KEY_GRAVE_ACCENT ); \
    m.attr( "KEY_WORLD_1" )         = py::int_( tysoc::keys::KEY_WORLD_1 ); \
    m.attr( "KEY_WORLD_2" )         = py::int_( tysoc::keys::KEY_WORLD_2 ); \
    m.attr( "KEY_ESCAPE" )          = py::int_( tysoc::keys::KEY_ESCAPE ); \
    m.attr( "KEY_ENTER" )           = py::int_( tysoc::keys::KEY_ENTER ); \
    m.attr( "KEY_TAB" )             = py::int_( tysoc::keys::KEY_TAB ); \
    m.attr( "KEY_BACKSPACE" )       = py::int_( tysoc::keys::KEY_BACKSPACE ); \
    m.attr( "KEY_INSERT" )          = py::int_( tysoc::keys::KEY_INSERT ); \
    m.attr( "KEY_DELETE" )          = py::int_( tysoc::keys::KEY_DELETE ); \
    m.attr( "KEY_RIGHT" )           = py::int_( tysoc::keys::KEY_RIGHT ); \
    m.attr( "KEY_LEFT" )            = py::int_( tysoc::keys::KEY_LEFT ); \
    m.attr( "KEY_DOWN" )            = py::int_( tysoc::keys::KEY_DOWN ); \
    m.attr( "KEY_UP" )              = py::int_( tysoc::keys::KEY_UP ); \
    m.attr( "KEY_PAGE_UP" )         = py::int_( tysoc::keys::KEY_PAGE_UP ); \
    m.attr( "KEY_PAGE_DOWN" )       = py::int_( tysoc::keys::KEY_PAGE_DOWN ); \
    m.attr( "KEY_HOME" )            = py::int_( tysoc::keys::KEY_HOME ); \
    m.attr( "KEY_END" )             = py::int_( tysoc::keys::KEY_END ); \
    m.attr( "KEY_CAPS_LOCK" )       = py::int_( tysoc::keys::KEY_CAPS_LOCK ); \
    m.attr( "KEY_SCROLL_LOCK" )     = py::int_( tysoc::keys::KEY_SCROLL_LOCK ); \
    m.attr( "KEY_NUM_LOCK" )        = py::int_( tysoc::keys::KEY_NUM_LOCK ); \
    m.attr( "KEY_PRINT_SCREEN" )    = py::int_( tysoc::keys::KEY_PRINT_SCREEN ); \
    m.attr( "KEY_PAUSE" )           = py::int_( tysoc::keys::KEY_PAUSE ); \
    m.attr( "KEY_F1" )              = py::int_( tysoc::keys::KEY_F1 ); \
    m.attr( "KEY_F2" )              = py::int_( tysoc::keys::KEY_F2 ); \
    m.attr( "KEY_F3" )              = py::int_( tysoc::keys::KEY_F3 ); \
    m.attr( "KEY_F4" )              = py::int_( tysoc::keys::KEY_F4 ); \
    m.attr( "KEY_F5" )              = py::int_( tysoc::keys::KEY_F5 ); \
    m.attr( "KEY_F6" )              = py::int_( tysoc::keys::KEY_F6 ); \
    m.attr( "KEY_F7" )              = py::int_( tysoc::keys::KEY_F7 ); \
    m.attr( "KEY_F8" )              = py::int_( tysoc::keys::KEY_F8 ); \
    m.attr( "KEY_F9" )              = py::int_( tysoc::keys::KEY_F9 ); \
    m.attr( "KEY_F10" )             = py::int_( tysoc::keys::KEY_F10 ); \
    m.attr( "KEY_F11" )             = py::int_( tysoc::keys::KEY_F11 ); \
    m.attr( "KEY_F12" )             = py::int_( tysoc::keys::KEY_F12 ); \
    m.attr( "KEY_F13" )             = py::int_( tysoc::keys::KEY_F13 ); \
    m.attr( "KEY_F14" )             = py::int_( tysoc::keys::KEY_F14 ); \
    m.attr( "KEY_F15" )             = py::int_( tysoc::keys::KEY_F15 ); \
    m.attr( "KEY_F16" )             = py::int_( tysoc::keys::KEY_F16 ); \
    m.attr( "KEY_F17" )             = py::int_( tysoc::keys::KEY_F17 ); \
    m.attr( "KEY_F18" )             = py::int_( tysoc::keys::KEY_F18 ); \
    m.attr( "KEY_F19" )             = py::int_( tysoc::keys::KEY_F19 ); \
    m.attr( "KEY_F20" )             = py::int_( tysoc::keys::KEY_F20 ); \
    m.attr( "KEY_F21" )             = py::int_( tysoc::keys::KEY_F21 ); \
    m.attr( "KEY_F22" )             = py::int_( tysoc::keys::KEY_F22 ); \
    m.attr( "KEY_F23" )             = py::int_( tysoc::keys::KEY_F23 ); \
    m.attr( "KEY_F24" )             = py::int_( tysoc::keys::KEY_F24 ); \
    m.attr( "KEY_F25" )             = py::int_( tysoc::keys::KEY_F25 ); \
    m.attr( "KEY_KP_0" )            = py::int_( tysoc::keys::KEY_KP_0 ); \
    m.attr( "KEY_KP_1" )            = py::int_( tysoc::keys::KEY_KP_1 ); \
    m.attr( "KEY_KP_2" )            = py::int_( tysoc::keys::KEY_KP_2 ); \
    m.attr( "KEY_KP_3" )            = py::int_( tysoc::keys::KEY_KP_3 ); \
    m.attr( "KEY_KP_4" )            = py::int_( tysoc::keys::KEY_KP_4 ); \
    m.attr( "KEY_KP_5" )            = py::int_( tysoc::keys::KEY_KP_5 ); \
    m.attr( "KEY_KP_6" )            = py::int_( tysoc::keys::KEY_KP_6 ); \
    m.attr( "KEY_KP_7" )            = py::int_( tysoc::keys::KEY_KP_7 ); \
    m.attr( "KEY_KP_8" )            = py::int_( tysoc::keys::KEY_KP_8 ); \
    m.attr( "KEY_KP_9" )            = py::int_( tysoc::keys::KEY_KP_9 ); \
    m.attr( "KEY_KP_DECIMAL" )      = py::int_( tysoc::keys::KEY_KP_DECIMAL ); \
    m.attr( "KEY_KP_DIVIDE" )       = py::int_( tysoc::keys::KEY_KP_DIVIDE ); \
    m.attr( "KEY_KP_MULTIPLY" )     = py::int_( tysoc::keys::KEY_KP_MULTIPLY ); \
    m.attr( "KEY_KP_SUBTRACT" )     = py::int_( tysoc::keys::KEY_KP_SUBTRACT ); \
    m.attr( "KEY_KP_ADD" )          = py::int_( tysoc::keys::KEY_KP_ADD ); \
    m.attr( "KEY_KP_ENTER" )        = py::int_( tysoc::keys::KEY_KP_ENTER ); \
    m.attr( "KEY_KP_EQUAL" )        = py::int_( tysoc::keys::KEY_KP_EQUAL ); \
    m.attr( "KEY_LEFT_SHIFT" )      = py::int_( tysoc::keys::KEY_LEFT_SHIFT ); \
    m.attr( "KEY_LEFT_CONTROL" )    = py::int_( tysoc::keys::KEY_LEFT_CONTROL ); \
    m.attr( "KEY_LEFT_ALT" )        = py::int_( tysoc::keys::KEY_LEFT_ALT ); \
    m.attr( "KEY_LEFT_SUPER" )      = py::int_( tysoc::keys::KEY_LEFT_SUPER ); \
    m.attr( "KEY_RIGHT_SHIFT" )     = py::int_( tysoc::keys::KEY_RIGHT_SHIFT ); \
    m.attr( "KEY_RIGHT_CONTROL" )   = py::int_( tysoc::keys::KEY_RIGHT_CONTROL ); \
    m.attr( "KEY_RIGHT_ALT" )       = py::int_( tysoc::keys::KEY_RIGHT_ALT ); \
    m.attr( "KEY_RIGHT_SUPER" )     = py::int_( tysoc::keys::KEY_RIGHT_SUPER ); \
    m.attr( "KEY_MENU" )            = py::int_( tysoc::keys::KEY_MENU );
