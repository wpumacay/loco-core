
#include <agent/agent_base.h>



namespace tysoc{
namespace agent{

    TIAgent::TIAgent( const std::string& name,
                      const TVec3& position,
                      const TVec3& rotation )
    {
        m_name = name;
        m_position = position;
        m_rotation = rotation;
        m_startPosition = position;
        m_startRotation = rotation;
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

    void TIAgent::setRotation( const TVec3& rotation )
    {
        m_rotation = rotation;
    }

    void TIAgent::setStartRotation( const TVec3& rotation )
    {
        m_startRotation = rotation;
    }    

    TVec3 TIAgent::getPosition()
    {
        return m_position;
    }

    TVec3 TIAgent::getStartPosition()
    {
        return m_startPosition;
    }

    TVec3 TIAgent::getRotation()
    {
        return m_rotation;
    }

    TVec3 TIAgent::getStartRotation()
    {
        return m_startRotation;
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