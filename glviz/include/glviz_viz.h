#pragma once

#include <viz/viz.h>
#include <glviz_common.h>
#include <glviz_kintree.h>
#include <glviz_terrainGen.h>
#include <glviz_drawable.h>
#include <glviz_ui.h>

#include <components/body.h>

using namespace engine;

namespace tysoc {

    class TGLScenarioUtilsLayer;

    class TGLVisualizer : public TIVisualizer
    {

    public :

        TGLVisualizer( TScenario* scenarioPtr );
        ~TGLVisualizer();

        void addBody( TBody* bodyPtr );
        void addAgent( TAgent* agentPtr );

        engine::CFrameBuffer* fboRgb() const { return m_targetFrameRgb.get(); }
        engine::CFrameBuffer* fboDepth() const { return m_targetFrameDepth.get(); }
        engine::CFrameBuffer* fboSemantic() const { return m_targetFrameSemantic.get(); };

    protected :

        bool _initializeInternal() override;
        void _updateInternal() override;
        bool _isActiveInternal() override;

        void _drawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color ) override;
        void _drawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color ) override;
        void _drawCameraInternal( const TMat4& cameraTransform, const TVec3& color ) override;
        void _drawSolidCylinderInternalX( float radius, float height, const TMat4& transform, const TVec4& color ) override;
        void _drawSolidCylinderInternalY( float radius, float height, const TMat4& transform, const TVec4& color ) override;
        void _drawSolidCylinderInternalZ( float radius, float height, const TMat4& transform, const TVec4& color ) override;
        void _drawSolidArrowInternalX( float length, const TMat4& transform, const TVec4& color) override;
        void _drawSolidArrowInternalY( float length, const TMat4& transform, const TVec4& color) override;
        void _drawSolidArrowInternalZ( float length, const TMat4& transform, const TVec4& color) override;

        int _remapKeyInternal( int keyCode ) override;
        bool _isKeyDownInternal( int keyCode ) override;
        bool _checkSingleKeyPressInternal( int keyCode ) override;

        TIVizCamera* _createCameraInternal( const std::string& name,
                                            const std::string& type,
                                            const TVec3& pos,
                                            const TMat3& rot ) override;
        void _changeToCameraInternal( TIVizCamera* cameraPtr ) override;
        void _grabCameraFrameInternal( TIVizCamera* cameraPtr,
                                       TIVizTexture& rgbTexture,
                                       TIVizTexture& depthTexture,
                                       TIVizTexture& semanticTexture ) override;

        TIVizLight* _createLightInternal( const std::string& name,
                                          const std::string& type,
                                          const TVec3& pos ) override;

    private :

        void _setupGlRenderingEngine();
        void _setupGlRenderTargets();
        void _collectSingleBodies( TBody* bodyPtr );
        void _collectKinTreeAgent( TAgent* agentPtr );
        void _collectTerrainGenerator( TITerrainGenerator* terrainGeneratorPtr );
        void _renderSensorReading( TISensor* sensorPtr );
        void _collectCustomTargets();
        void _readPixels( CFrameBuffer* fbuffer, TIVizTexture& texture );

    private :

        // rendering engine resources
        CScene* m_glScene;
        CFixedCamera* m_glSensorCamera;
        std::unique_ptr< CApplication > m_glApplication;
        TGLScenarioUtilsLayer* m_guiScenarioLayer;

        // render targets used to grab frame data
        std::unique_ptr< CFrameBuffer > m_targetFrameRgb;
        std::unique_ptr< CFrameBuffer > m_targetFrameDepth;
        std::unique_ptr< CFrameBuffer > m_targetFrameSemantic;
        // render options used to generate the targets
        engine::CRenderOptions m_renderOptionsTargetRgb;
        engine::CRenderOptions m_renderOptionsTargetDepth;
        engine::CRenderOptions m_renderOptionsTargetSemantic;

        // visualization wrappers
        std::vector< std::unique_ptr< TGLVizKinTree > >             m_vizKinTreeWrappers;
        std::vector< std::unique_ptr< TGLVizTerrainGenerator > >    m_vizTerrainGeneratorWrappers;

    };

    extern "C" TIVisualizer* visualizer_createVisualizer( TScenario* scenarioPtr );

}