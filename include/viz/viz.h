
#pragma once

// scenario functionality
#include <scenario.h>
// UI functionality
#include <viz/ui.h>
#include <viz/keys.h>

namespace tysoc {
namespace viz {

    struct TIVizCamera
    {
        std::string     name;
        std::string     type;
        TVec3           pos;
        TMat3           rot;
        TMat4           frustum;
    };

    struct TIVizLight
    {
        std::string     name;
        std::string     type;
        TVec3           pos;
    };

    struct TIVizTexture
    {
        unsigned char*  data;
        unsigned int    width;
        unsigned int    height;
        unsigned int    channels;
        unsigned int    textureID;

        TIVizTexture()
        {
            data        = NULL;
            width       = 0;
            height      = 0;
            channels    = 0;
            textureID   = 0;
        }
    };

    class TIVisualizer
    {

        protected :

        // A reference to the scenario, to grab the agents, terraingens, etc.
        TScenario* m_scenarioPtr;
        // A reference to the UI handler
        TIVisualizerUI* m_uiPtr;

        // abstract camera pointers
        std::map< std::string, TIVizCamera* > m_cameras;
        // abstract light pointers
        std::map< std::string, TIVizLight* > m_lights;

        // type of the visualizer
        std::string m_type;
        // working directory (where to find the meshes)
        std::string m_workingDir;

        virtual bool _initializeInternal() = 0;
        virtual void _updateInternal() = 0;
        virtual void _renderUIInternal() = 0;
        virtual bool _isActiveInternal() = 0;

        virtual int _remapKeyInternal( int keyCode ) = 0;
        virtual bool _isKeyDownInternal( int keyCode ) = 0;
        virtual bool _checkSingleKeyPressInternal( int keyCode ) = 0;

        virtual TIVizCamera* _createCameraInternal( const std::string& name,
                                                    const std::string& type,
                                                    const TVec3& pos,
                                                    const TMat3& rot ) = 0;
        virtual void _changeToCameraInternal( TIVizCamera* cameraPtr ) = 0;
        virtual void _grabCameraFrameInternal( TIVizCamera* cameraPtr,
                                               TIVizTexture& rgbTexture,
                                               TIVizTexture& depthTexture ) = 0;

        virtual TIVizLight* _createLightInternal( const std::string& name,
                                                  const std::string& type,
                                                  const TVec3& pos ) = 0;

        virtual void _collectSimPayloadInternal( void* payload, const std::string& type );

        public :

        TIVisualizer( TScenario* scenarioPtr,
                      const std::string& workingDir );
        ~TIVisualizer();

        void setScenario( TScenario* scenarioPtr );
        void collectSimPayload( void* payload, const std::string& type );

        bool initialize();
        void update();
        // void reset(); // @TODO: should add this functionality when links changing sizes
        void renderUI();
        bool isActive();

        bool isKeyDown( int keyCode );
        bool checkSingleKeyPress( int keyCode );

        void addCamera( const std::string& name, 
                        const std::string& type,
                        const TVec3& pos,
                        const TMat3& rot );
        void changeToCamera( const std::string& name );
        void grabCameraFrame( const std::string& name,
                              TIVizTexture& rgbTexture,
                              TIVizTexture& depthTexture );

        void addLight( const std::string& name,
                       const std::string& type,
                       const TVec3& pos );

        TIVisualizerUI* getUI();

        std::string type();
    };


    typedef TIVisualizer* FcnCreateViz( TScenario* scenarioPtr,
                                        const std::string& workingDir );

}}