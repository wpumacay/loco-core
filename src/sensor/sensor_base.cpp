
#include <sensor/sensor.h>

namespace tysocsensor
{


    TSensor::TSensor( const std::string& name )
    {
        m_name = name;
    }

    TSensor::~TSensor()
    {
        
    }

    std::string TSensor::name()
    {
        return m_name;
    }

}