
#include <viz/viz.h>


namespace tysoc {


    TIVisualizer::TIVisualizer( TScenario* scenarioPtr )
    {
        m_scenarioPtr   = scenarioPtr;
        m_simulationPtr = nullptr;
        m_useSensorReadings = false;
        m_useSensorReadingRgb = false;
        m_useSensorReadingDepth = false;
        m_useSensorReadingSemantic = false;

        m_sensorUsesPositionAndTarget = false;
        m_sensorUsesTransform = false;
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

    void TIVisualizer::setSensorsEnabled( bool enable )
    {
        m_useSensorReadings = enable;
    }

    void TIVisualizer::setSensorRgbEnabled( bool enable )
    {
        m_useSensorReadingRgb = enable;
    }

    void TIVisualizer::setSensorDepthEnabled( bool enable )
    {
        m_useSensorReadingDepth = enable;
    }

    void TIVisualizer::setSensorSemanticEnabled( bool enable )
    {
        m_useSensorReadingSemantic = enable;
    }

    void TIVisualizer::setSensorsView( const TVec3& position, const TVec3& target )
    {
        m_sensorUsesPositionAndTarget = true;
        m_sensorUsesTransform = false;

        m_sensorViewPosition = position;
        m_sensorViewTarget = target;
    }

    void TIVisualizer::setSensorsView( const TMat4& transform )
    {
        m_sensorUsesPositionAndTarget = false;
        m_sensorUsesTransform = true;

        m_sensorViewTransform = transform;
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

    bool TIVisualizer::useSensorReadings()
    {
        return m_useSensorReadings;
    }

    bool TIVisualizer::useSensorReadingRgb()
    {
        return m_useSensorReadingRgb;
    }

    bool TIVisualizer::useSensorReadingDepth()
    {
        return m_useSensorReadingDepth;
    }

    bool TIVisualizer::useSensorReadingSemantic()
    {
        return m_useSensorReadingSemantic;
    }

    void TIVisualizer::drawLine( const TVec3& start, const TVec3& end, const TVec3& color )
    {
        _drawLineInternal( start, end, color );
    }

    void TIVisualizer::drawAABB( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color )
    {
        _drawAABBInternal( aabbMin, aabbMax, aabbWorldTransform, color );
    }

    void TIVisualizer::drawCamera( const TMat4& cameraTransform, const TVec3& color )
    {
        _drawCameraInternal( cameraTransform, color );
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
                                        TIVizTexture& depthTexture,
                                        TIVizTexture& semanticTexture )
    {
        if ( m_cameras.find( name ) == m_cameras.end() )
        {
            std::cout << "WARNING> Can't grab data from non-existent camera with name: " << name << std::endl;
            return;
        }

        _grabCameraFrameInternal( m_cameras[ name ], rgbTexture, depthTexture, semanticTexture );
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