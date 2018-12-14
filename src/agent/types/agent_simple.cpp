
#include <agent/types/agent_simple.h>



namespace tysoc{
namespace agent{

    TAgentSimple::TAgentSimple( const std::string& name,
                                const TGeometry& geometry )
        : TIAgent( name, geometry.worldTransform.getPosition() )
    {
        m_type  = AGENT_TYPE_SIMPLE;
        m_speed = { 0.0, 0.0, 0.0 };

        // add the geometry (single one here)
        m_geometries.push_back( geometry );
    }

    TAgentSimple::~TAgentSimple()
    {
        
    }

    void TAgentSimple::_updateAgentInternal( float dt )
    {
        m_position = m_position + m_speed * dt;
    }

    void TAgentSimple::setSpeed( const TVec3& speed )
    {
        m_speed = speed;
    }

    TVec3 TAgentSimple::getSpeed()
    {
        return m_speed;
    }

}}