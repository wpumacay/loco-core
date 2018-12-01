
#include <agent/agent_base.h>



namespace tysocAgent
{


    TIAgent::TIAgent( const std::string& name,
                      float startX, 
                      float startY, 
                      float startZ )
    {
        m_posX = startX;
        m_posY = startY;
        m_posZ = startZ;
    }

    TIAgent::~TIAgent()
    {
        // Override this
    }

    void TIAgent::setPosition( float x, float y, float z )
    {
        m_posX = x;
        m_posY = y;
        m_posZ = z;
        _setPositionInternal( x, y, z );
    }

    void TIAgent::getPosition( float& x, float& y, float& z )
    {
        x = m_posX;
        y = m_posY;
        z = m_posZ;
    }

    void TIAgent::update()
    {
        _updateInternal();
    }

    std::string TIAgent::getName()
    {
        return m_name;
    }


}