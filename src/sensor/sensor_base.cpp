
#include <sensor/sensor.h>

namespace tysocsensor
{


    TSensor::TSensor( tysocagent::TAgent* agentPtr,
                      tysocterrain::TTerrainGenerator* terrainGenPtr )
    {
        m_agentPtr = agentPtr;
        m_terrainGenPtr = terrainGenPtr;
    }

    TSensor::~TSensor()
    {
        m_agentPtr = NULL;
        m_terrainGenPtr = NULL;
    }

    std::string TSensor::name()
    {
        return m_name;
    }

}