#pragma once

#include <loco_common.h>
#include <visualizer/loco_visualizer_camera.h>

namespace loco {
namespace visualizer {

    class TVizCamera;

    class TVizCameraAdapter
    {
    public :

        TVizCameraAdapter( TVizCamera* cameraRef ) : m_cameraRef( cameraRef ), m_awaitingDeletion( false ) {}
        virtual ~TVizCameraAdapter() {}

        virtual void Build() = 0;
        virtual void Detach() = 0;
        virtual void Reset() = 0;

        virtual void SetPosition( const TVec3& position ) = 0;
        virtual void SetTarget( const TVec3& target ) = 0;

        virtual void GetPosition( TVec3& dstPosition ) = 0;
        virtual void GetTarget( TVec3& dstTarget ) = 0;

        bool IsAwaitingDeletion() const { return m_awaitingDeletion; }

    protected :

        // Handle to the camera-object the user is exposed to
        TVizCamera* m_cameraRef;
        // Whether or not the adapter is ready to be freed
        bool m_awaitingDeletion;
    };

}}