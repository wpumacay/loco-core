
#include <sensor/sensor.h>

namespace tysoc {
namespace sensor {


    TISensor::TISensor( const std::string& name )
    {
        m_name = name;
        m_type = "undefined";
    }

    TISensor::~TISensor()
    {
        TYSOC_LOG( "Base-destroying sensor: " + m_name );
    }

    std::string TISensor::name()
    {
        return m_name;
    }

    std::string TISensor::type()
    {
        return m_type;
    }

}}