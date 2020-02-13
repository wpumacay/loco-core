
#include <visualizer/loco_visualizer_camera.h>

namespace loco
{
    std::string ToString( const eVizCameraType& type )
    {
        if ( type == eVizCameraType::ORBIT ) return "orbit";
        if ( type == eVizCameraType::FPS ) return "fps";
        if ( type == eVizCameraType::FIXED ) return "fixed";

        return "undefined";
    }

    TVizCamera::TVizCamera( const std::string& name,
                            const eVizCameraType& type,
                            const TVec3& position,
                            const TVec3& target )
    {
        m_name = name;
        m_type = type;
        m_position = position;
        m_position0 = position;
        m_target = target;
        m_target0 = target;
        m_adapterRef = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TVizCamera {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TVizCamera " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TVizCamera::~TVizCamera()
    {
        if ( m_adapterRef )
            m_adapterRef->Detach();

        m_adapterRef = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TVizCamera {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TVizCamera " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TVizCamera::Update()
    {
        if ( !m_adapterRef )
            return;

        m_adapterRef->GetPosition( m_position );
        m_adapterRef->GetTarget( m_target );
    }

    void TVizCamera::Reset()
    {
        m_position = m_position0;
        m_target = m_target0;

        if ( m_adapterRef )
            m_adapterRef->Reset();
    }

    void TVizCamera::SetAdapter( TVizCameraAdapter* adapterRef )
    {
        m_adapterRef = adapterRef;
    }

    void TVizCamera::SetPosition( const TVec3& position )
    {
        m_position = position;
        if ( m_adapterRef )
            m_adapterRef->SetPosition( position );
    }

    void TVizCamera::SetTarget( const TVec3& target )
    {
        m_target = target;
        if ( m_adapterRef )
            m_adapterRef->SetTarget( target );
    }
}