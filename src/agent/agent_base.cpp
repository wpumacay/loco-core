
#include <agent/agent_base.h>



namespace tysoc{
namespace agent{

    TIAgent::TIAgent( const std::string& name,
                      const TVec3& position )
    {
        m_name = name;
        m_position = position;
        m_type = "undefined";
    }

    TIAgent::~TIAgent()
    {
        m_geometries.clear();
    }

    void TIAgent::setPosition( const TVec3& position )
    {
        m_position = position;
    }

    TVec3 TIAgent::getPosition()
    {
        return m_position;
    }

    void TIAgent::update( float dt )
    {
        // make the agent's logic update the inner data
        _updateAgentInternal( dt );
    }

    std::string TIAgent::getName()
    {
        return m_name;
    }

    std::string TIAgent::getType()
    {
        return m_type;
    }

    const std::vector< TGeometry >& TIAgent::getGeometries() const
    {
        return m_geometries;
    }
    
}}