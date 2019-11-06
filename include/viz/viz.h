#pragma once

// scenario functionality
#include <scenario.h>
#include <viz/keys.h>
// drawable functionality
#include <viz/drawable.h>
// simulation (to grab a reference)
#include <simulation_base.h>

namespace tysoc {

    class TISimulation;

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

        TIVizTexture()
        {
            data        = nullptr;
            width       = 0;
            height      = 0;
            channels    = 0;
        }
    };

    class TIVisualizer
    {

    public :

        TIVisualizer( TScenario* scenarioPtr );
        ~TIVisualizer();

        void setSimulation( TISimulation* simulationPtr );
        void setScenario( TScenario* scenarioPtr );
        void setSensorsEnabled( bool enable );
        void setSensorRgbEnabled( bool enable );
        void setSensorDepthEnabled( bool enable );
        void setSensorSemanticEnabled( bool enable );
        void setSensorsView( const TVec3& position, const TVec3& target );

        bool initialize();
        void update();
        // void reset(); // @TODO: should add this functionality when links change sizes
        bool isActive();
        bool useSensorReadings();
        bool useSensorReadingRgb();
        bool useSensorReadingDepth();
        bool useSensorReadingSemantic();

        // Debug drawing helpers
        void drawLine( const TVec3& start, const TVec3& end, const TVec3& color );
        void drawAABB( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color );

        bool isKeyDown( int keyCode );
        bool checkSingleKeyPress( int keyCode );

        void addCamera( const std::string& name, 
                        const std::string& type,
                        const TVec3& pos,
                        const TMat3& rot );
        void changeToCamera( const std::string& name );
        void grabCameraFrame( const std::string& name,
                              TIVizTexture& rgbTexture,
                              TIVizTexture& depthTexture,
                              TIVizTexture& semanticTexture );

        void addLight( const std::string& name,
                       const std::string& type,
                       const TVec3& pos );

    protected :

        virtual bool _initializeInternal() = 0;
        virtual void _updateInternal() = 0;
        virtual bool _isActiveInternal() = 0;

        virtual void _drawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color ) = 0;
        virtual void _drawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color ) = 0;

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
                                               TIVizTexture& depthTexture,
                                               TIVizTexture& semanticTexture ) = 0;

        virtual TIVizLight* _createLightInternal( const std::string& name,
                                                  const std::string& type,
                                                  const TVec3& pos ) = 0;

    protected :

        TScenario*    m_scenarioPtr;
        TISimulation* m_simulationPtr;

        std::vector< TIDrawable* >              m_vizDrawables;
        std::map< std::string, TIVizCamera* >   m_cameras;
        std::map< std::string, TIVizLight* >    m_lights;

        bool m_useSensorReadings;
        bool m_useSensorReadingRgb;
        bool m_useSensorReadingDepth;
        bool m_useSensorReadingSemantic;

        TVec3 m_sensorViewPosition;
        TVec3 m_sensorViewTarget;
    };


    typedef TIVisualizer* FcnCreateViz( TScenario* scenarioPtr );

}