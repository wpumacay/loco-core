
#include <glviz_viz.h>

namespace tysoc {

    TGLVisualizer::TGLVisualizer( TScenario* scenarioPtr )
        : TIVisualizer( scenarioPtr )
    {
        m_glScene        = nullptr;
        m_glApplication  = nullptr;
        m_guiScenarioLayer = nullptr;

    #ifdef TYSOC_DEMO
        m_useSensorReadings = true;
        m_useSensorReadingRgb = true;
        m_useSensorReadingDepth = true;
        m_useSensorReadingSemantic = true;
    #endif
    }

    TGLVisualizer::~TGLVisualizer()
    {
        m_glScene        = nullptr;
        m_glApplication  = nullptr;
        m_guiScenarioLayer = nullptr;

        m_vizKinTreeWrappers.clear();
        m_vizTerrainGeneratorWrappers.clear();
    }

    bool TGLVisualizer::_initializeInternal()
    {
        if ( !m_scenarioPtr )
        {
            TYSOC_CORE_ERROR( "Scenario reference is nullptr" );
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
        {
            _collectKinTreeAgent( _agents[i] );
            m_vizKinTreeWrappers.back()->setMaskId( 1000 + i );
        }

        // add ui layer to control the elements of the scenario
        m_guiScenarioLayer = new TGLScenarioUtilsLayer( "Scenario-layer",
                                                        this,
                                                        m_scenarioPtr );
        m_glApplication->addGuiLayer( std::unique_ptr< engine::CImGuiLayer >( m_guiScenarioLayer ) );

        /* create custom render-targets to recover frame data */
        _setupGlRenderTargets();

        /* configure render options for our custom targets */
        m_renderOptionsTargetRgb.mode = engine::eRenderMode::NORMAL;
        m_renderOptionsTargetRgb.useFrustumCulling = false;
        m_renderOptionsTargetRgb.cullingGeom = engine::eCullingGeom::BOUNDING_BOX;
        m_renderOptionsTargetRgb.useFaceCulling = false;
        m_renderOptionsTargetRgb.useBlending = false;
        m_renderOptionsTargetRgb.useFog = false;
        m_renderOptionsTargetRgb.useSkybox = true;
        m_renderOptionsTargetRgb.useShadowMapping = true;
        m_renderOptionsTargetRgb.pcfCount = 0;
        m_renderOptionsTargetRgb.redrawShadowMap = false;
        m_renderOptionsTargetRgb.viewportWidth = m_glApplication->window()->width() / 2.0f;
        m_renderOptionsTargetRgb.viewportHeight = m_glApplication->window()->height() / 2.0f;
        m_renderOptionsTargetRgb.cameraPtr = m_glApplication->scene()->currentCamera();
        m_renderOptionsTargetRgb.lightPtr = m_glScene->mainLight();
        m_renderOptionsTargetRgb.shadowMapPtr = m_glApplication->renderer()->shadowMap();
        m_renderOptionsTargetRgb.fogPtr = nullptr;
        m_renderOptionsTargetRgb.skyboxPtr = m_glScene->skybox();
        m_renderOptionsTargetRgb.renderTargetPtr = m_targetFrameRgb.get();
        m_renderOptionsTargetRgb.shadowMapRangeConfig.type = engine::eShadowRangeType::FIXED_USER;
        m_renderOptionsTargetRgb.shadowMapRangeConfig.worldUp = { 0.0f, 0.0f, 1.0f };
        m_renderOptionsTargetRgb.shadowMapRangeConfig.cameraPtr = m_glApplication->scene()->currentCamera();

        auto _light = m_glApplication->scene()->mainLight();
        auto _lightType = _light->type();

        if ( _lightType == eLightType::DIRECTIONAL )
            m_renderOptionsTargetRgb.shadowMapRangeConfig.dirLightPtr = dynamic_cast< CDirectionalLight* >( _light );
        else if ( _lightType == eLightType::POINT )
            m_renderOptionsTargetRgb.shadowMapRangeConfig.pointLightPtr = dynamic_cast< CPointLight* >( _light );
        else if ( _lightType == eLightType::SPOT )
            m_renderOptionsTargetRgb.shadowMapRangeConfig.spotLightPtr = dynamic_cast< CSpotLight* >( _light );

        m_renderOptionsTargetDepth.mode = engine::eRenderMode::DEPTH_ONLY;
        m_renderOptionsTargetDepth.useFrustumCulling = true;
        m_renderOptionsTargetDepth.cullingGeom = engine::eCullingGeom::BOUNDING_BOX;
        m_renderOptionsTargetDepth.useFaceCulling = false;
        m_renderOptionsTargetDepth.useBlending = false;
        m_renderOptionsTargetDepth.useFog = false;
        m_renderOptionsTargetDepth.useSkybox = false;
        m_renderOptionsTargetDepth.useShadowMapping = false;
        m_renderOptionsTargetDepth.redrawShadowMap = false;
        m_renderOptionsTargetDepth.viewportWidth = m_glApplication->window()->width() / 2.0f;
        m_renderOptionsTargetDepth.viewportHeight = m_glApplication->window()->height() / 2.0f;
        m_renderOptionsTargetDepth.cameraPtr = m_glApplication->scene()->currentCamera();
        m_renderOptionsTargetDepth.lightPtr = nullptr;
        m_renderOptionsTargetDepth.shadowMapPtr = nullptr;
        m_renderOptionsTargetDepth.renderTargetPtr = m_targetFrameDepth.get();
        m_renderOptionsTargetDepth.depthViewZmin = 0.0f;
        m_renderOptionsTargetDepth.depthViewZmax = 20.0f;
        m_renderOptionsTargetDepth.depthViewZminColor = { 1.0f, 1.0f, 1.0f };
        m_renderOptionsTargetDepth.depthViewZmaxColor = { 0.0f, 0.0f, 0.0f };

        m_renderOptionsTargetSemantic.mode = engine::eRenderMode::SEMANTIC_ONLY;
        m_renderOptionsTargetSemantic.useFrustumCulling = true;
        m_renderOptionsTargetSemantic.cullingGeom = engine::eCullingGeom::BOUNDING_BOX;
        m_renderOptionsTargetSemantic.useFaceCulling = false;
        m_renderOptionsTargetSemantic.useBlending = false;
        m_renderOptionsTargetSemantic.useFog = false;
        m_renderOptionsTargetSemantic.useSkybox = false;
        m_renderOptionsTargetSemantic.useShadowMapping = false;
        m_renderOptionsTargetSemantic.redrawShadowMap = false;
        m_renderOptionsTargetSemantic.viewportWidth = m_glApplication->window()->width() / 2.0f;
        m_renderOptionsTargetSemantic.viewportHeight = m_glApplication->window()->height() / 2.0f;
        m_renderOptionsTargetSemantic.cameraPtr = m_glApplication->scene()->currentCamera();
        m_renderOptionsTargetSemantic.lightPtr = nullptr;
        m_renderOptionsTargetSemantic.shadowMapPtr = nullptr;
        m_renderOptionsTargetSemantic.renderTargetPtr = m_targetFrameSemantic.get();

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

        _collectCustomTargets();

        // *************************************************************
    }

    void TGLVisualizer::_collectCustomTargets()
    {
        if ( !m_useSensorReadings )
            return;

        auto _camera = m_glApplication->scene()->currentCamera();
        if ( m_sensorUsesPositionAndTarget )
        {
            m_glSensorCamera->setPosition( fromTVec3( m_sensorViewPosition ) );
            m_glSensorCamera->setTargetPoint( fromTVec3( m_sensorViewTarget ) );
            _camera = m_glSensorCamera;
        }
        else if ( m_sensorUsesTransform )
        {
            m_glSensorCamera->setCameraTransform( fromTMat4( m_sensorViewTransform ) );
            _camera = m_glSensorCamera;

            _drawCameraInternal( toTMat4( m_glSensorCamera->matView() ), { 0.1f, 0.4f, 0.9f } );
        }

        if ( m_useSensorReadingRgb && m_targetFrameRgb )
        {
            m_renderOptionsTargetRgb.cameraPtr = _camera;
            m_renderOptionsTargetRgb.lightPtr = m_glScene->mainLight();
            m_renderOptionsTargetRgb.shadowMapRangeConfig.cameraPtr = m_glApplication->scene()->currentCamera();

            auto _light = m_glApplication->scene()->mainLight();
            auto _lightType = _light->type();

            if ( _lightType == eLightType::DIRECTIONAL )
                m_renderOptionsTargetRgb.shadowMapRangeConfig.dirLightPtr = dynamic_cast< CDirectionalLight* >( _light );
            else if ( _lightType == eLightType::POINT )
                m_renderOptionsTargetRgb.shadowMapRangeConfig.pointLightPtr = dynamic_cast< CPointLight* >( _light );
            else if ( _lightType == eLightType::SPOT )
                m_renderOptionsTargetRgb.shadowMapRangeConfig.spotLightPtr = dynamic_cast< CSpotLight* >( _light );

            m_glApplication->renderer()->begin( m_renderOptionsTargetRgb );
            m_glApplication->renderer()->submit( m_glApplication->scene()->renderables() );
            m_glApplication->renderer()->render();
        }

        if ( m_useSensorReadingDepth && m_targetFrameDepth )
        {
            m_renderOptionsTargetDepth.cameraPtr = _camera;

            m_glApplication->renderer()->begin( m_renderOptionsTargetDepth );
            m_glApplication->renderer()->submit( m_glApplication->scene()->renderables() );
            m_glApplication->renderer()->render();
        }

        if ( m_useSensorReadingSemantic && m_targetFrameSemantic )
        {
            m_renderOptionsTargetSemantic.cameraPtr = _camera;

            m_glApplication->renderer()->begin( m_renderOptionsTargetSemantic );
            m_glApplication->renderer()->submit( m_glApplication->scene()->renderables() );
            m_glApplication->renderer()->render();
        }
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

    void TGLVisualizer::_drawCameraInternal( const TMat4& viewMatrix, const TVec3& color )
    {
        engine::CDebugDrawer::DrawClipVolume( engine::CMat4::perspective( 45.0f, 
                                                                          m_glApplication->window()->aspect(), 
                                                                          0.01f, 0.2f ) * fromTMat4( viewMatrix ), 
                                              fromTVec3( color ) );
    }

    void TGLVisualizer::_drawSolidCylinderInternalX( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        engine::CDebugDrawer::DrawSolidCylinder( radius, height, engine::eAxis::X, fromTMat4( transform ), fromTVec4( color ) );
    }

    void TGLVisualizer::_drawSolidCylinderInternalY( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        engine::CDebugDrawer::DrawSolidCylinder( radius, height, engine::eAxis::Y, fromTMat4( transform ), fromTVec4( color ) );
    }

    void TGLVisualizer::_drawSolidCylinderInternalZ( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        engine::CDebugDrawer::DrawSolidCylinder( radius, height, engine::eAxis::Z, fromTMat4( transform ), fromTVec4( color ) );
    }

    void TGLVisualizer::_drawSolidArrowInternalX( float length, const TMat4& transform, const TVec4& color)
    {
        engine::CDebugDrawer::DrawSolidArrow( length, engine::eAxis::X, fromTMat4( transform ), fromTVec4( color ) );
    }


    void TGLVisualizer::_drawSolidArrowInternalY( float length, const TMat4& transform, const TVec4& color)
    {
        engine::CDebugDrawer::DrawSolidArrow( length, engine::eAxis::Y, fromTMat4( transform ), fromTVec4( color ) );
    }


    void TGLVisualizer::_drawSolidArrowInternalZ( float length, const TMat4& transform, const TVec4& color)
    {
        engine::CDebugDrawer::DrawSolidArrow( length, engine::eAxis::Z, fromTMat4( transform ), fromTVec4( color ) );
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
                                                  TIVizTexture& depthTexture,
                                                  TIVizTexture& semanticTexture )
    {
        // @todo: move cameraptr to another configuration method (camera configuration)
        //        prior to making the render call, because we're doing the render-call 
        //        somewhere else to avoid repetition

        if ( m_useSensorReadingRgb )
            _readPixels( m_targetFrameRgb.get(), rgbTexture );

        if ( m_useSensorReadingDepth )
            _readPixels( m_targetFrameDepth.get(), depthTexture );

        if ( m_useSensorReadingSemantic )
            _readPixels( m_targetFrameSemantic.get(), semanticTexture );
    }

    void TGLVisualizer::_readPixels( CFrameBuffer* fbuffer, TIVizTexture& texture )
    {
        auto _colorAttachment = fbuffer->getTextureAttachment( "color_attachment" );
        auto _colorAttachmentConfig = fbuffer->getConfigAttachment( "color_attachment" );
    
        auto _width = _colorAttachmentConfig.width;
        auto _height = _colorAttachmentConfig.height;
    
        engine::uint8* _buffer = new engine::uint8[3 * _width * _height];
    
        fbuffer->bind();
        glReadPixels( 0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, _buffer );
        fbuffer->unbind();

        texture.data = _buffer;
        texture.width = _width;
        texture.height = _height;
        texture.channels = 3;
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

        auto _imguiProperties = engine::CImGuiProps();
    #ifdef TYSOC_DEMO
        _imguiProperties.useDockingSpace = true;
        _imguiProperties.useDockingSpacePassthrough = true;
        _imguiProperties.useAutosaveLayout = false;
        _imguiProperties.fileLayout = std::string( TYSOC_PATH_RESOURCES ) + "app_gui_layout.ini";
    #else
        _imguiProperties.useDockingSpace = false;
        _imguiProperties.useDockingSpacePassthrough = false;
        _imguiProperties.useAutosaveLayout = true;
        _imguiProperties.fileLayout = "";
    #endif

        m_glApplication = std::unique_ptr< engine::CApplication >( new engine::CApplication( _windowProperties, _imguiProperties ) );
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

        /* create a camera used for custom-targets ****************************************************/

        m_glSensorCamera = new engine::CFixedCamera( "fixed-sensor",
                                                     { 3.0f, 3.0f, 3.0f },
                                                     { 0.0f, 0.0f, 0.0f },
                                                     engine::eAxis::Z,
                                                     _cameraProjData );

        m_glScene->addCamera( std::unique_ptr< engine::CICamera >( m_glSensorCamera ) );

        /**********************************************************************************************/

        m_glApplication->renderOptions().useSkybox = true;
        m_glApplication->renderOptions().useBlending = true;
        m_glApplication->renderOptions().useShadowMapping = true;
        m_glApplication->renderOptions().pcfCount = 0;
        m_glApplication->renderOptions().shadowMapRangeConfig.type = engine::eShadowRangeType::FIXED_USER;
        m_glApplication->renderOptions().shadowMapRangeConfig.worldUp = { 0.0f, 0.0f, 1.0f };
        m_glApplication->renderOptions().shadowMapRangeConfig.cameraPtr = _orbitCamera;
        m_glApplication->renderOptions().shadowMapRangeConfig.dirLightPtr = _dirlight;
    }

    void TGLVisualizer::_setupGlRenderTargets()
    {
        engine::CAttachmentConfig _fbColorConfig;
        _fbColorConfig.name                 = "color_attachment";
        _fbColorConfig.attachment           = engine::eFboAttachment::COLOR;
        _fbColorConfig.width                = m_glApplication->window()->width() / 2.0f;
        _fbColorConfig.height               = m_glApplication->window()->height() / 2.0f;
        _fbColorConfig.texInternalFormat    = engine::eTextureFormat::RGB;
        _fbColorConfig.texFormat            = engine::eTextureFormat::RGB;
        _fbColorConfig.texPixelDataType     = engine::ePixelDataType::UINT_8;
        _fbColorConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbColorConfig.texWrapV             = engine::eTextureWrap::REPEAT;
    
        engine::CAttachmentConfig _fbDepthConfig;
        _fbDepthConfig.name                 = "depth_attachment";
        _fbDepthConfig.attachment           = engine::eFboAttachment::DEPTH;
        _fbDepthConfig.width                = m_glApplication->window()->width() / 2.0f;
        _fbDepthConfig.height               = m_glApplication->window()->height() / 2.0f;
        _fbDepthConfig.texInternalFormat    = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texFormat            = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texPixelDataType     = engine::ePixelDataType::UINT_32;
        _fbDepthConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbDepthConfig.texWrapV             = engine::eTextureWrap::REPEAT;
    
        m_targetFrameRgb = std::unique_ptr< engine::CFrameBuffer >( new engine::CFrameBuffer() );
        m_targetFrameDepth = std::unique_ptr< engine::CFrameBuffer >( new engine::CFrameBuffer() );
        m_targetFrameSemantic = std::unique_ptr< engine::CFrameBuffer >( new engine::CFrameBuffer() );

        m_targetFrameRgb->addAttachment( _fbColorConfig );
        m_targetFrameRgb->addAttachment( _fbDepthConfig );

        m_targetFrameDepth->addAttachment( _fbColorConfig );
        m_targetFrameDepth->addAttachment( _fbDepthConfig );

        m_targetFrameSemantic->addAttachment( _fbColorConfig );
        m_targetFrameSemantic->addAttachment( _fbDepthConfig );
    }

    void TGLVisualizer::_collectKinTreeAgent( TAgent* agentPtr )
    {
        // create the kintree viz wrapper
        auto _vizKinTreeWrapper = new TGLVizKinTree( agentPtr, m_glScene );
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

            _collisionDrawable->show( false ); // keep hidden, until the user wants to see it
            _collisionDrawable->wireframe( true );
            _collision->setDrawable( _collisionDrawable );

            // scene keeps ownership of gl-renderable resource, drawable keeps only a reference
            if ( _collisionDrawable->renderable() )
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
            _visualDrawable->setTexture( _visual->data().texture );

            _visualDrawable->show( true );
            _visualDrawable->wireframe( false );
            _visual->setDrawable( _visualDrawable );

            // scene keeps ownership of gl-renderable resource, drawable keeps only a reference
            if ( _visualDrawable->renderable() )
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
