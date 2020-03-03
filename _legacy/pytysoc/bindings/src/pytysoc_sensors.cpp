
#include <pytysoc_sensors.h>


namespace pytysoc
{

    PySensor::PySensor( const std::string& name )
    {
        m_sensorPtr = NULL;
    }

    PySensor::~PySensor()
    {
        m_sensorPtr = NULL;
    }

    std::string PySensor::name()
    {
        if ( m_sensorPtr )
            return m_sensorPtr->name();

        return "undefined";
    }

    std::string PySensor::type()
    {
        if ( m_sensorPtr )
            return tysoc::toString( m_sensorPtr->type() );

        return "undefined";
    }

    MeasurementDict PySensor::getMeasurement()
    {
        if ( !m_sensorPtr )
            return MeasurementDict();

////         auto _type = m_sensorPtr->type();
////         auto _measurement = m_sensorPtr->getSensorMeasurement();
//// 
////         if ( _type == tysoc::SENSOR_TYPE_INTRINSICS )
////             return _packIntrinsics( (tysoc::TAgentIntrinsicsSensorMeasurement*) _measurement );
////         // else if ( _type == tysoc::SENSOR_TYPE_EXTRINSICS_TERRAIN )
////         //     return _packExtrinsicsTerrain( (tysoc::TSectionsTerrainSensorMeasurement*) _measurement );
//// 
////         std::cout << "WARNING> sensor type: " << _type << " is not supported" << std::endl;

        return MeasurementDict();
    }

    MeasurementDict PySensor::_packIntrinsics( tysoc::TAgentIntrinsicsSensorMeasurement* measurementPtr )
    {
        MeasurementDict _result;

////         _result["jointspos"] = vecArrayToNumpy( measurementPtr->thetas );
////         _result["jointsvel"] = vecArrayToNumpy( measurementPtr->thetadots );
////         _result["bodiesrelpos"] = vecArrayToNumpy( measurementPtr->bodiesRelativePosition );
////         _result["bodieslinvel"] = vecArrayVec3ToNumpy( measurementPtr->bodiesLinVelocities );
////         _result["bodieslinacc"] = vecArrayVec3ToNumpy( measurementPtr->bodiesLinAccelerations );

        return _result;
    }

    // MeasurementDict PySensor::_packExtrinsicsTerrain( tysoc::TSectionsTerrainSensorMeasurement* measurementPtr )
    // {
    //     MeasurementDict _result;
    //      
    //     _result["profile"] = vecArrayToNumpy( measurementPtr->profile );
    //      
    //     return _result;
    // }

    tysoc::TISensor* PySensor::ptr()
    {
        return m_sensorPtr;
    }

    PySensorIntrinsics::PySensorIntrinsics( const std::string& name,
                                            PyCoreAgent* pyCoreAgentPtr )
        : PySensor( name )
    {
////         if ( pyCoreAgentPtr->ptr() )
////         {
////             m_sensorPtr = new tysoc::TAgentIntrinsicsSensor( name,
////                                                                      pyCoreAgentPtr->ptr() );
////         }
////         else
////         {
////             std::cout << "WARNING> Sensor wrapper for agent-intrinsic sensor" 
////                       << "has been given an agent wrapper with no kintree-agent object" << std::endl;
////         }
    }

}