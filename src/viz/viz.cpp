
#include <viz/viz.h>


namespace tysoc {
namespace viz {


    TIVisualizer::TIVisualizer( TScenario* scenarioPtr )
    {
        m_uiPtr         = NULL;
        m_scenarioPtr   = scenarioPtr;
    }

    TIVisualizer::~TIVisualizer()
    {
        m_scenarioPtr = NULL;

        if ( m_uiPtr )
        {
            delete m_uiPtr;
            m_uiPtr = NULL;
        }
    }

    void TIVisualizer::initialize()
    {
        _initializeInternal();
    }

    void TIVisualizer::update()
    {
        _updateInternal();
    }

    void TIVisualizer::renderUI()
    {
        _renderUIInternal();
    }

    bool TIVisualizer::isActive()
    {
        return _isActiveInternal();
    }

    void TIVisualizer::addCamera( const std::string& name, 
                                  const std::string& type,
                                  const TVec3& pos,
                                  const TMat3& rot )
    {
        // create a new camera using the specific backend
        auto _camera = _createCameraInternal( name, type, pos, rot );
        if ( !_camera )
        {
            return;
        }

        // add camera if not already there
        if ( m_cameras.find( name ) != m_cameras.end() )
        {
            m_cameras[ name ] = _camera;
        }
        else
        {
            std::cout << "WARNING> Camera with name: " << name << " already exists" << std::endl;
        }
    }

    void TIVisualizer::changeToCamera( const std::string& name )
    {
        if ( m_cameras.find( name ) != m_cameras.end() )
        {
            _changeToCameraInternal( m_cameras[ name ] );
        }
        else
        {
            std::cout << "WARNING> Can't change to non-existent camera with name: " << name << std::endl;
        }
    }

    void TIVisualizer::grabCameraFrame( const std::string& name,
                                        TIVizTexture& rgbTexture,
                                        TIVizTexture& depthTexture )
    {
        if ( m_cameras.find( name ) != m_cameras.end() )
        {
            _grabCameraFrameInternal( m_cameras[ name ], rgbTexture, depthTexture );
        }
        else
        {
            std::cout << "WARNING> Can't grab data from non-existent camera with name: " << name << std::endl;
        }
    }

    void TIVisualizer::addLight( const std::string& name,
                                 const std::string& type,
                                 const TVec3& pos )
    {
        auto _light = _createLightInternal( name, type, pos );
        if ( !_light )
        {
            return;
        }

        // add light if not already there
        if ( m_lights.find( name ) != m_lights.end() )
        {
            m_lights[ name ] = _light;
        }
        else
        {
            std::cout << "WARNING> Light with name: " << name << " already exists" << std::endl;
        }
    }

    TIVisualizerUI* TIVisualizer::getUI()
    {
        return m_uiPtr;
    }

    bool TIVisualizer::isKeyDown( int keyCode )
    {
        int _keyCodeMapped = _remapKeyInternal( keyCode );
        return _isKeyDownInternal( _keyCodeMapped );
    }

    bool TIVisualizer::checkSingleKeyPress( int keyCode )
    {
        int _keyCodeMapped = _remapKeyInternal( keyCode );
        return _checkSingleKeyPressInternal( _keyCodeMapped );
    }

}}