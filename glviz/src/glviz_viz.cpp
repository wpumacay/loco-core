
#include <glviz_viz.h>


namespace tysoc {

    TGLVisualizer::TGLVisualizer( TScenario* scenarioPtr )
        : TIVisualizer( scenarioPtr )
    {
        m_glScene        = nullptr;
        m_glApplication  = nullptr;
    }

    TGLVisualizer::~TGLVisualizer()
    {
        m_glScene        = nullptr;
        m_glApplication  = nullptr;

        m_vizKinTreeWrappers.clear();
        m_vizTerrainGeneratorWrappers.clear();
    }

    bool TGLVisualizer::_initializeInternal()
    {
        if ( !m_scenarioPtr )
        {
            std::cout << "ERROR> scenario reference is nullptr" << std::endl;
            return false;
        }

        // set up rendering engine stuff
        _setupGlRenderingEngine();

        // Create drawable adapters for single bodies
        auto _bodies = m_scenarioPtr->getBodies();
        for ( size_t i = 0; i < _bodies.size(); i++ )
            _collectSingleBodies( _bodies[i] );

        // Create visualization wrappers for the terrain generator
        auto _terrainGenerators = m_scenarioPtr->getTerrainGenerators();
        for ( size_t i = 0; i < _terrainGenerators.size(); i++ )
            _collectTerrainGenerator( _terrainGenerators[i] );

        // Create visualization wrappers for the agents
        auto _agents = m_scenarioPtr->getAgents();
        for ( size_t i = 0; i < _agents.size(); i++ )
            _collectKinTreeAgent( _agents[i] );

        return true;
    }

    void TGLVisualizer::_updateInternal()
    {
        // Update terrain visualization wrappers
        for ( auto& _vizTerrainGen : m_vizTerrainGeneratorWrappers )
            _vizTerrainGen->update();

        // and also the agent visualization wrappers
        for ( auto& _vizKinTree : m_vizKinTreeWrappers )
            _vizKinTree->update();

        // and the sensor readings (render them directly, seems like ...
        // wrapping them would be wasteful?)
        auto _sensors = m_scenarioPtr->getSensors();
        for ( auto _sensor : _sensors )
            _renderSensorReading( _sensor );

        // and finally request to the rendering engine *****************

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_G ) )
        {
            ENGINE_TRACE( "Toggling ui state" );
            m_glApplication->setGuiActive( !m_glApplication->guiActive() );
        }
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_U ) )
        {
            ENGINE_TRACE( "Toggling ui-utils state" );
            m_glApplication->setGuiUtilsActive( !m_glApplication->guiUtilsActive() );
        }

        m_glApplication->update();
        m_glApplication->begin();
        m_glApplication->render();
        m_glApplication->end();

        // *************************************************************
    }

    bool TGLVisualizer::_isActiveInternal()
    {
        return m_glApplication->active();
    }

    void TGLVisualizer::_drawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color )
    {
        engine::CDebugDrawer::DrawLine( fromTVec3( start ), fromTVec3( end ), fromTVec3( color ) );
    }

    void TGLVisualizer::_drawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color )
    {
        engine::CDebugDrawer::DrawAABB( fromTVec3( aabbMin ), fromTVec3( aabbMax ), fromTMat4( aabbWorldTransform ), fromTVec3( color ) );
    }

    TIVizCamera* TGLVisualizer::_createCameraInternal( const std::string& name,
                                                           const std::string& type,
                                                           const TVec3& pos,
                                                           const TMat3& rot )
    {
        // @TODO|@WIP
        return nullptr;
    }

    void TGLVisualizer::_changeToCameraInternal( TIVizCamera* cameraPtr )
    {
        // @TODO|@WIP
    }

    void TGLVisualizer::_grabCameraFrameInternal( TIVizCamera* cameraPtr,
                                                  TIVizTexture& rgbTexture,
                                                  TIVizTexture& depthTexture )
    {
        // @TODO|@WIP
    }

    TIVizLight* TGLVisualizer::_createLightInternal( const std::string& name,
                                                         const std::string& type,
                                                         const TVec3& pos )
    {
        // @TODO|@WIP
        return nullptr;
    }

    void TGLVisualizer::_setupGlRenderingEngine()
    {
        auto _windowProperties = engine::CWindowProps();
        _windowProperties.width = 1024;
        _windowProperties.height = 768;
        _windowProperties.title = "resizable-application";
        _windowProperties.clearColor = { 0.6f, 0.659f, 0.690f, 1.0f };
        _windowProperties.resizable = true;

        m_glApplication = std::unique_ptr< engine::CApplication >( new engine::CApplication( _windowProperties ) );
        m_glScene = m_glApplication->scene();

        /* create some lights for the scene ***********************************************************/
        auto _dirlight = new engine::CDirectionalLight( "directional",
                                                        { 0.5f, 0.5f, 0.5f },
                                                        { 0.8f, 0.8f, 0.8f },
                                                        { 0.8f, 0.8f, 0.8f },
                                                        { -1.0f, -1.0f, -1.0f } );

        m_glScene->addLight( std::unique_ptr< engine::CILight >( _dirlight ) );

        auto _pointlight = new engine::CPointLight( "point",
                                                    { 0.5f, 0.5f, 0.5f },
                                                    { 0.8f, 0.8f, 0.8f },
                                                    { 0.8f, 0.8f, 0.8f },
                                                    { 3.0f, 3.0f, 3.0f },
                                                    1.0f, 0.0f, 0.0f );

        m_glScene->addLight( std::unique_ptr< engine::CILight >( _pointlight ) );
        /* create some cameras for the scene **********************************************************/
        auto _cameraProjData = engine::CCameraProjData();
        _cameraProjData.aspect = m_glApplication->window()->aspect();
        _cameraProjData.width = 10.0f * m_glApplication->window()->aspect();
        _cameraProjData.height = 10.0f;

        auto _orbitCamera = new engine::COrbitCamera( "orbit",
                                                      { 3.0f, 3.0f, 3.0f },
                                                      { 0.0f, 0.0f, 1.0f },
                                                      engine::eAxis::Z,
                                                      _cameraProjData,
                                                      m_glApplication->window()->width(),
                                                      m_glApplication->window()->height() );

        m_glScene->addCamera( std::unique_ptr< engine::CICamera >( _orbitCamera ) );

        const float _cameraSensitivity  = 0.1f;
        const float _cameraSpeed        = 50.0f;
        const float _cameraMaxDelta     = 10.0f;
        
        auto _fpsCamera = new engine::CFpsCamera( "fps",
                                                  { 3.0f, 3.0f, 3.0f },
                                                  { 0.0f, 0.0f, 1.0f },
                                                  engine::eAxis::Z,
                                                  _cameraProjData,
                                                  _cameraSensitivity,
                                                  _cameraSpeed,
                                                  _cameraMaxDelta );

        m_glScene->addCamera( std::unique_ptr< engine::CICamera >( _fpsCamera ) );
        /* add some effects like fog and a skybox *****************************************************/

        auto _skybox = new engine::CSkybox();
        _skybox->setCubemap( engine::CTextureManager::GetCachedTextureCube( "cloudtop" ) );

        m_glScene->addSkybox( std::unique_ptr< engine::CSkybox >( _skybox ) );

        /**********************************************************************************************/

        m_glApplication->renderOptions().useShadowMapping = true;
        m_glApplication->renderOptions().useSkybox = true;
        m_glApplication->renderOptions().shadowMapRangeConfig.type = engine::eShadowRangeType::FIXED_USER;
        m_glApplication->renderOptions().shadowMapRangeConfig.worldUp = { 0.0f, 0.0f, 1.0f };
        m_glApplication->renderOptions().shadowMapRangeConfig.cameraPtr = _orbitCamera;
        m_glApplication->renderOptions().shadowMapRangeConfig.clipSpaceWidth   = 40.0f;
        m_glApplication->renderOptions().shadowMapRangeConfig.clipSpaceHeight  = 40.0f;
        m_glApplication->renderOptions().shadowMapRangeConfig.clipSpaceDepth   = 40.0f;
        m_glApplication->renderOptions().shadowMapRangeConfig.dirLightPtr = _dirlight;
    }

    void TGLVisualizer::_collectKinTreeAgent( TAgent* agentPtr )
    {
        // create the kintree viz wrapper
        auto _vizKinTreeWrapper = new TGLVizKinTree( agentPtr,
                                                     m_glScene );
        // and add it to the buffer of kintree vizs
        m_vizKinTreeWrappers.push_back( std::unique_ptr< TGLVizKinTree >( _vizKinTreeWrapper ) );
    }

    void TGLVisualizer::_collectSingleBodies( TBody* bodyPtr )
    {
        if ( !bodyPtr )
            return;

        auto _collisions = bodyPtr->collisions();
        for ( auto _collision : _collisions )
        {
            if ( !_collision )
                continue;

            auto _collisionDrawable = new TGLDrawable( _collision->data() );

            if ( !_collisionDrawable )
                continue;

            _collisionDrawable->show( true );
            _collisionDrawable->wireframe( true );
            _collision->setDrawable( _collisionDrawable );

            // scene keeps ownership of gl-renderable resource, drawable keeps only a reference
            m_glScene->addRenderable( std::unique_ptr< engine::CIRenderable >( _collisionDrawable->renderable() ) );

            // @todo: keep ownership by using unique_ptr in viz.h
            m_vizDrawables.push_back( _collisionDrawable );
        }

        auto _visuals = bodyPtr->visuals();
        for ( auto _visual : _visuals )
        {
            if ( !_visual )
                continue;

            auto _visualDrawable = new TGLDrawable( _visual->data() );

            if ( !_visualDrawable )
                continue;

            _visualDrawable->setAmbientColor( _visual->data().ambient );
            _visualDrawable->setDiffuseColor( _visual->data().diffuse );
            _visualDrawable->setSpecularColor( _visual->data().specular );
            _visualDrawable->setShininess( _visual->data().shininess );

            // @todo: allow grabbing texture from id in visual-data
            // ...

            _visualDrawable->show( true );
            _visualDrawable->wireframe( false );
            _visual->setDrawable( _visualDrawable );

            // scene keeps ownership of gl-renderable resource, drawable keeps only a reference
            m_glScene->addRenderable( std::unique_ptr< engine::CIRenderable >( _visualDrawable->renderable() ) );

            // @todo: keep ownership by using unique_ptr in viz.h
            m_vizDrawables.push_back( _visualDrawable );
        }

    }

    void TGLVisualizer::_collectTerrainGenerator( TITerrainGenerator* terrainGeneratorPtr )
    {
        // create the terrainGenrator viz wrapper
        auto _vizTerrainGeneratorWrapper = new TGLVizTerrainGenerator( terrainGeneratorPtr,
                                                                       m_glScene );
        // and add it to the buffer of terrainGenerator vizs
        m_vizTerrainGeneratorWrappers.push_back( std::unique_ptr< TGLVizTerrainGenerator >( _vizTerrainGeneratorWrapper ) );
    }

    void TGLVisualizer::_renderSensorReading( TISensor* sensorPtr )
    {
        auto _measurement = sensorPtr->getSensorMeasurement();

        // @TODO: Move this part to a separate module (kind of ...
        // delegate the functionality of rendering specifics to
        // other submodules of the visualizer)

        if ( _measurement->type == eSensorType::EXT_HEIGHTFIELD_1D )
        {
            // draw profile from sensor reading
            auto _pathMeasurement = reinterpret_cast< TSectionsTerrainSensorMeasurement* >
                                        ( _measurement );

            std::vector< engine::CLine > _lines;
            for ( size_t i = 0; i < ( _pathMeasurement->profile.size() / 3 ); i++ )
            {
                engine::CLine _pline;


                if ( !_pathMeasurement->usesComplement )
                {
                    _pline.start.x = _pathMeasurement->profile[3 * i + 0];
                    _pline.start.y = _pathMeasurement->profile[3 * i + 1];
                    _pline.start.z = _pathMeasurement->agentProjection.z;
                    
                    _pline.end.x = _pathMeasurement->profile[3 * i + 0];
                    _pline.end.y = _pathMeasurement->profile[3 * i + 1];
                    _pline.end.z = _pline.start.z + _pathMeasurement->profile[3 * i + 2];
                }
                else
                {
                    _pline.start.x = _pathMeasurement->profile[3 * i + 0];
                    _pline.start.y = _pathMeasurement->profile[3 * i + 1];
                    _pline.start.z = _pathMeasurement->agentPosition.z;

                    _pline.end.x = _pathMeasurement->profile[3 * i + 0];
                    _pline.end.y = _pathMeasurement->profile[3 * i + 1];
                    _pline.end.z = _pline.start.z - _pathMeasurement->profile[3 * i + 2];
                }

                _lines.push_back( _pline );
            }

            engine::CDebugDrawer::DrawLinesBatch( _lines, { 0.0f, 0.0f, 1.0f } );
        }
        else if ( _measurement->type == eSensorType::PROP_JOINT )
        {
            auto _agentMeasurement = reinterpret_cast< TAgentIntrinsicsSensorMeasurement* >
                                        ( _measurement );

            std::vector< engine::CLine > _lines;
            for ( size_t i = 0; i < ( _agentMeasurement->bodiesRelativePosition.size() / 3 ); i++ )
            {
                engine::CLine _pline;

                _pline.start.x = _agentMeasurement->rootPosition.x;
                _pline.start.y = _agentMeasurement->rootPosition.y;
                _pline.start.z = _agentMeasurement->rootPosition.z;

                _pline.end.x = _pline.start.x + _agentMeasurement->bodiesRelativePosition[3 * i + 0];
                _pline.end.y = _pline.start.y + _agentMeasurement->bodiesRelativePosition[3 * i + 1];
                _pline.end.z = _pline.start.z + _agentMeasurement->bodiesRelativePosition[3 * i + 2];


                _lines.push_back( _pline );
            }

            engine::CDebugDrawer::DrawLinesBatch( _lines, { 1.0f, 0.1f, 0.0f } );
        }
    }

    int TGLVisualizer::_remapKeyInternal( int keyCode )
    {
        /*
        *   This rendering backend uses glfw, so no mapping required as the
        *   standard keycodes values are defined as in glfw3 keycode definitions
        */
        return keyCode;
    }

    bool TGLVisualizer::_isKeyDownInternal( int keyCode )
    {
        return engine::CInputManager::IsKeyDown( keyCode );
    }

    bool TGLVisualizer::_checkSingleKeyPressInternal( int keyCode )
    {
        return engine::CInputManager::CheckSingleKeyPress( keyCode );
    }

    extern "C" TIVisualizer* visualizer_createVisualizer( TScenario* scenarioPtr )
    {
        return new TGLVisualizer( scenarioPtr );
    }

}
