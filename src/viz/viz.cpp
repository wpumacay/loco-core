
#include <viz/viz.h>


namespace tysoc {


    TIVisualizer::TIVisualizer( TScenario* scenarioPtr )
    {
        m_scenarioPtr   = scenarioPtr;
        m_simulationPtr = nullptr;
    }

    TIVisualizer::~TIVisualizer()
    {
        m_scenarioPtr   = nullptr;
        m_simulationPtr = nullptr;
    }

    void TIVisualizer::setScenario( TScenario* scenarioPtr )
    {
        m_scenarioPtr = scenarioPtr;
    }

    void TIVisualizer::setSimulation( TISimulation* simulationPtr )
    {
        m_simulationPtr = simulationPtr;
    }

    bool TIVisualizer::initialize()
    {
        return _initializeInternal();
    }

    void TIVisualizer::update()
    {
        _updateInternal();
    }

    bool TIVisualizer::isActive()
    {
        return _isActiveInternal();
    }

    void TIVisualizer::drawLine( const TVec3& start, const TVec3& end, const TVec3& color )
    {
        _drawLineInternal( start, end, color );
    }

    void TIVisualizer::drawAABB( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color )
    {
        _drawAABBInternal( aabbMin, aabbMax, aabbWorldTransform, color );
    }

    void TIVisualizer::addCamera( const std::string& name, 
                                  const std::string& type,
                                  const TVec3& pos,
                                  const TMat3& rot )
    {
        if ( m_cameras.find( name ) != m_cameras.end() )
        {
            std::cout << "WARNING> Camera with name: " << name << " already exists" << std::endl;
            return;
        }

        // create a new camera using the specific backend
        auto _camera = _createCameraInternal( name, type, pos, rot );
        if ( !_camera )
            return;

        m_cameras[ name ] = _camera;
    }

    void TIVisualizer::changeToCamera( const std::string& name )
    {
        if ( m_cameras.find( name ) == m_cameras.end() )
        {
            std::cout << "WARNING> Can't change to non-existent camera with name: " << name << std::endl;
            return;
        }

        _changeToCameraInternal( m_cameras[ name ] );
    }

    void TIVisualizer::grabCameraFrame( const std::string& name,
                                        TIVizTexture& rgbTexture,
                                        TIVizTexture& depthTexture )
    {
        if ( m_cameras.find( name ) == m_cameras.end() )
        {
            std::cout << "WARNING> Can't grab data from non-existent camera with name: " << name << std::endl;
            return;
        }

        _grabCameraFrameInternal( m_cameras[ name ], rgbTexture, depthTexture );
    }

    void TIVisualizer::addLight( const std::string& name,
                                 const std::string& type,
                                 const TVec3& pos )
    {
        if ( m_lights.find( name ) != m_lights.end() )
        {
            std::cout << "WARNING> Light with name: " << name << " already exists" << std::endl;
            return;
        }

        auto _light = _createLightInternal( name, type, pos );
        if ( !_light )
            return;

        m_lights[ name ] = _light;
    }

    bool TIVisualizer::isKeyDown( int keyCode )
    {
        int _keyCodeMapped = _remapKeyInternal( keyCode );
        return _isKeyDownInternal( keyCode );
    }

    bool TIVisualizer::checkSingleKeyPress( int keyCode )
    {
        int _keyCodeMapped = _remapKeyInternal( keyCode );
        return _checkSingleKeyPressInternal( _keyCodeMapped );
    }

}