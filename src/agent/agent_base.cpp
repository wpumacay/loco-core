
#include <agent/agent_base.h>



namespace tysoc{
namespace agent{

    TIAgent::TIAgent( const std::string& name,
                      const TVec3& position )
    {
        m_name = name;
        m_position = position;
        m_startPosition = position;
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

    void TIAgent::setStartPosition( const TVec3& position )
    {
        m_startPosition = position;
    }

    TVec3 TIAgent::getPosition()
    {
        return m_position;
    }

    TVec3 TIAgent::getStartPosition()
    {
        return m_startPosition;
    }

    void TIAgent::update( float dt )
    {
        // make the agent's logic update the inner data
        _updateAgentInternal( dt );
    }

    void TIAgent::reset()
    {
        m_position = m_startPosition;
        _resetAgentInternal();
    }

    std::string TIAgent::name()
    {
        return m_name;
    }

    std::string TIAgent::type()
    {
        return m_type;
    }

    const std::vector< TGeometry >& TIAgent::getGeometries() const
    {
        return m_geometries;
    }
    
}}