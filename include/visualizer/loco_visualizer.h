#pragma once

#include <loco_scenario.h>
#include <visualizer/loco_keycodes.h>
#include <visualizer/loco_drawable.h>
#include <visualizer/loco_visualizer_light.h>
#include <visualizer/loco_visualizer_camera.h>
#include <loco_simulation.h>
#include <loco_runtime.h>


namespace loco
{
    class TISimulation;
    class TRuntime;

    class TIVisualizer
    {
    public :

        TIVisualizer( TScenario* scenarioRef,
                      size_t windowWidth, size_t windowHeight,
                      bool windowResizable, bool renderOffscreen );
        virtual ~TIVisualizer();

        void ChangeScenario( TScenario* scenarioRef );
        void Initialize();
        void Update();
        void Reset();

        TVizCamera* CreateCamera( const std::string& name,
                                  const eVizCameraType& type,
                                  const TVec3& position,
                                  const TVec3& target );

        TVizLight* CreateLight( const std::string& name,
                                const eVizLightType& type,
                                const TVec3& ambient,
                                const TVec3& diffuse,
                                const TVec3& specular );

        void SetSimulation( TISimulation* simulationRef );
        void SetRuntime( TRuntime* runtimeRef );

        TVizCamera* GetCurrentCamera();
        TVizLight* GetCurrentLight();

        const TVizCamera* GetCurrentCamera() const;
        const TVizLight* GetCurrentLight() const;

        void ChangeCurrentCamera( const std::string& name );
        void ChangeCurrentLight( const std::string& light );

        bool HasCameraNamed( const std::string& name ) const;
        bool HasLightNamed( const std::string& name ) const;

        size_t GetNumCameras() const;
        size_t GetNumLights() const;

        const TVizCamera* GetCameraByName( const std::string& name ) const;
        const TVizLight* GetLightByName( const std::string& name ) const;

        TVizCamera* GetCameraByName( const std::string& name );
        TVizLight* GetLightByName( const std::string& name );

        bool IsActive() const;
        bool IsKeyDown( int key ) const;
        bool CheckSingleKeyPress( int key ) const;
        bool IsMouseDown( int button ) const;
        TVec2 GetCursorPosition() const;

        const TVizCamera* get_camera( ssize_t index ) const;
        const TVizLight* get_light( ssize_t index ) const;

        TVizCamera* get_mutable_camera( ssize_t index );
        TVizLight* get_mutable_light( ssize_t index );

        void DrawLine( const TVec3& start, const TVec3& end, const TVec3& color );
        void DrawAABB( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color );
        void DrawSolidCylinderX( float radius, float height, const TMat4& transform, const TVec4& color );
        void DrawSolidCylinderY( float radius, float height, const TMat4& transform, const TVec4& color );
        void DrawSolidCylinderZ( float radius, float height, const TMat4& transform, const TVec4& color );
        void DrawSolidArrowX( float length, const TMat4& transform, const TVec4& color);
        void DrawSolidArrowY( float length, const TMat4& transform, const TVec4& color);
        void DrawSolidArrowZ( float length, const TMat4& transform, const TVec4& color);

        TScenario* scenario() { return m_scenarioRef; }

        const TScenario* scenario() const { return m_scenarioRef; }

        TISimulation* simulation() { return m_simulationRef; }

        const TISimulation* simulation() const { return m_simulationRef; }

        std::string backendId() const { return m_backendId; }

    protected :

        virtual void _ChangeScenarioInternal() = 0;
        virtual void _InitializeInternal() = 0;
        virtual void _UpdateInternal() = 0;
        virtual void _ResetInternal() = 0;
        virtual void _SetSimulationInternal( TISimulation* simulationRef ) {};
        virtual void _SetRuntimeInternal( TRuntime* runtimeRef ) {};

        virtual void _DrawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color ) = 0;
        virtual void _DrawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color ) = 0;
        virtual void _DrawSolidCylinderInternalX( float radius, float height, const TMat4& transform, const TVec4& color ) = 0;
        virtual void _DrawSolidCylinderInternalY( float radius, float height, const TMat4& transform, const TVec4& color ) = 0;
        virtual void _DrawSolidCylinderInternalZ( float radius, float height, const TMat4& transform, const TVec4& color ) = 0;
        virtual void _DrawSolidArrowInternalX( float length, const TMat4& transform, const TVec4& color) = 0;
        virtual void _DrawSolidArrowInternalY( float length, const TMat4& transform, const TVec4& color) = 0;
        virtual void _DrawSolidArrowInternalZ( float length, const TMat4& transform, const TVec4& color) = 0;

        virtual bool _IsActiveInternal() const = 0;
        virtual bool _IsKeyDownInternal( int key ) const = 0;
        virtual bool _CheckSingleKeyPressInternal( int key ) const = 0;
        virtual bool _IsMouseDownInternal( int button ) const = 0;
        virtual TVec2 _GetCursorPositionInternal() const = 0;

        virtual void _CreateCameraInternal( TVizCamera* cameraRef ) = 0;
        virtual void _CreateLightInternal( TVizLight* lightRef ) = 0;

        virtual void _ChangeCurrentCameraInternal( TVizCamera* cameraRef ) = 0;
        virtual void _ChangeCurrentLightInternal( TVizLight* lightRef ) = 0;

    protected :

        std::string m_backendId;

        TScenario* m_scenarioRef;
        TISimulation* m_simulationRef;
        TRuntime* m_runtimeRef;

        std::vector< std::unique_ptr< TVizCamera > > m_vizCameras;
        std::vector< std::unique_ptr< TVizLight > > m_vizLights;

        std::vector< std::unique_ptr< TIDrawable > > m_vizDrawableAdapters;
        std::vector< std::unique_ptr< TVizCameraAdapter > > m_vizCameraAdapters;
        std::vector< std::unique_ptr< TVizLightAdapter > > m_vizLightAdapters;

        std::unordered_map< std::string, ssize_t > m_camerasMap;
        std::unordered_map< std::string, ssize_t > m_lightsMap;

        ssize_t m_currentCameraIndex;
        ssize_t m_currentLightIndex;
    };

    typedef TIVisualizer* FcnCreateViz( TScenario* scenarioRef,
                                        size_t windowWidth, size_t windowHeight,
                                        bool windowResizable, bool renderOffscreen );

    class TNullVisualizer : public TIVisualizer
    {
    public :

        TNullVisualizer( TScenario* scenarioRef,
                         size_t windowWidth, size_t windowHeight,
                         bool windowResizable, bool renderOffscreen );

        ~TNullVisualizer();

    protected :

        void _ChangeScenarioInternal() override {}
        void _InitializeInternal() override {}
        void _UpdateInternal() override {}
        void _ResetInternal() override {}

        void _DrawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color ) override {}
        void _DrawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color ) override {}
        void _DrawSolidCylinderInternalX( float radius, float height, const TMat4& transform, const TVec4& color ) override {}
        void _DrawSolidCylinderInternalY( float radius, float height, const TMat4& transform, const TVec4& color ) override {}
        void _DrawSolidCylinderInternalZ( float radius, float height, const TMat4& transform, const TVec4& color ) override {}
        void _DrawSolidArrowInternalX( float length, const TMat4& transform, const TVec4& color) override {}
        void _DrawSolidArrowInternalY( float length, const TMat4& transform, const TVec4& color) override {}
        void _DrawSolidArrowInternalZ( float length, const TMat4& transform, const TVec4& color) override {}

        bool _IsActiveInternal() const override { return true; }
        bool _IsKeyDownInternal( int key ) const override { return false; }
        bool _CheckSingleKeyPressInternal( int key ) const override { return false; }
        bool _IsMouseDownInternal( int button ) const override { return false; }
        TVec2 _GetCursorPositionInternal() const override { return TVec2( 0.0f, 0.0f ); }

        void _CreateCameraInternal( TVizCamera* cameraRef ) override {}
        void _CreateLightInternal( TVizLight* lightRef ) override {}

        void _ChangeCurrentCameraInternal( TVizCamera* cameraRef ) override {}
        void _ChangeCurrentLightInternal( TVizLight* lightRef ) override {}
    };
}