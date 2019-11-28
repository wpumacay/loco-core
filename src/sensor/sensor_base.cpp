
#include <sensor/sensor.h>

namespace tysoc {

    TISensor::TISensor( const std::string& name )
    {
        m_name = name;
        m_type = eSensorType::NONE;
    }

    TISensor::~TISensor()
    {
        TYSOC_CORE_TRACE( "Base-destroying sensor: {0}", m_name );
    }

}