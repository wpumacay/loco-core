
#pragma once

#include <viz/viz.h>
#include <glviz_common.h>
#include <glviz_kintree.h>
#include <glviz_terrainGen.h>
#include <glviz_drawable.h>

#include <components/bodies.h>

using namespace engine;

namespace tysoc {

    class TGLVisualizer : public TIVisualizer
    {

    public :

        TGLVisualizer( TScenario* scenarioPtr );
        ~TGLVisualizer();

        void addBody( TBody* bodyPtr );
        void addAgent( TAgent* agentPtr );

    protected :

        bool _initializeInternal() override;
        void _updateInternal() override;
        bool _isActiveInternal() override;

        void _drawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color ) override;
        void _drawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color ) override;

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
                                       TIVizTexture& depthTexture ) override;

        TIVizLight* _createLightInternal( const std::string& name,
                                          const std::string& type,
                                          const TVec3& pos ) override;

    private :

        void _setupGlRenderingEngine();
        void _collectSingleBodies( TBody* bodyPtr );
        void _collectKinTreeAgent( TAgent* agentPtr );
        void _collectTerrainGenerator( TITerrainGenerator* terrainGeneratorPtr );
        void _renderSensorReading( TISensor* sensorPtr );

    private :

        // rendering engine resources
        CScene* m_glScene;
        std::unique_ptr< CApplication > m_glApplication;
        
        // visualization wrappers
        std::vector< std::unique_ptr< TGLVizKinTree > >             m_vizKinTreeWrappers;
        std::vector< std::unique_ptr< TGLVizTerrainGenerator > >    m_vizTerrainGeneratorWrappers;

    };

    extern "C" TIVisualizer* visualizer_createVisualizer( TScenario* scenarioPtr );

}