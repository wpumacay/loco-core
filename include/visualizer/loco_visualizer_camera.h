#pragma once

#include <loco_common.h>

namespace loco
{
    enum class eVizCameraType : uint8_t
    {
        ORBIT = 0,
        FPS,
        FIXED
    };

    std::string ToString( const eVizCameraType& type );

    class TVizCameraAdapter;

    class TVizCamera
    {
    public :

        TVizCamera( const std::string& name,
                    const eVizCameraType& type,
                    const TVec3& position,
                    const TVec3& target );
        ~TVizCamera();

        void Update();
        void Reset();

        void SetAdapter( TVizCameraAdapter* adapterRef );

        void SetPosition( const TVec3& position );
        void SetTarget( const TVec3& target );

        TVec3 position() const { return m_position; }
        TVec3 target() const { return m_target; }

        std::string name() const { return m_name; }
        eVizCameraType type() const { return m_type; }

    private :

        // Unique identifier of this camera on the visualizer
        std::string m_name;
        // Type of camera used (check enum above for supported types)
        eVizCameraType m_type;
        // Position of the camera on the scenario
        TVec3 m_position;
        // Initial position of the camera on the scenario
        TVec3 m_position0;
        // Target that the camera points to on the scenario
        TVec3 m_target;
        // Initial target the camera points to on the scenario
        TVec3 m_target0;
        // Adapter for backend-specific camera functionality
        TVizCameraAdapter* m_adapterRef;
    };

    class TVizCameraAdapter
    {
    public :

        TVizCameraAdapter( TVizCamera* cameraRef ) : m_cameraRef( cameraRef ) {}
        virtual ~TVizCameraAdapter() {}

        virtual void Build() = 0;
        virtual void Detach() = 0;
        virtual void Reset() = 0;

        virtual void SetPosition( const TVec3& position ) = 0;
        virtual void SetTarget( const TVec3& target ) = 0;

        virtual void GetPosition( TVec3& dstPosition ) = 0;
        virtual void GetTarget( TVec3& dstTarget ) = 0;

    protected :

        // Handle to the camera-object the user is exposed to
        TVizCamera* m_cameraRef;
    };
}