
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_agent_core.h>

#include <sensor/sensor.h>


namespace pytysoc
{

    class PySensor
    {

        protected :

        tysoc::sensor::TISensor* m_sensorPtr;

        public :

        /**
        *   Creates a python wrapper for a generic sensor.  The wrapped sensor ...
        *   object is created internally from a given sensor type.
        *   
        *   @param name     Name for the wrapped sensor
        *   @exposed        Not exposed through python API
        */
        PySensor( const std::string& name );

        /**
        *   Destructor for this wrapper. It only NULLs the references wrapped.
        *   The actual deletion of the wrapped resources should be made by ...
        *   the scenario, which has control over all kintrees, terraingens and sensors.
        */
        ~PySensor();

        /**
        *   Returns the name of the sensor
        *   
        *   @exposed    Exposed through python API
        */
        std::string name();

        /**
        *   Returns the type of the sensor: intrinsics, extrinsics_terrain
        *   
        *   @exposed    Exposed through python API
        */
        std::string type();

        /**
        *   Returns a measurement dictionary of array measurements in the ...
        *   following form :
        *
        *   { 'key' : nparray }
        *
        *   E.g. agent_intrinsics measurements
        *   
        *   { 'jointpos' : np.array(...), 'jointvel' : np.array(...), 
        *     'bodyrelpos' : np.arary(...), 'bodylinvel' : np.arary(...), 'bodylinacc' : (...) }
        *
        *   E.g. extrinsics_terrain measurements
        *
        *   { 'heightmap' : np.array(...) }
        *
        *   E.g. extrinsics_camera measurements
        *
        *   { 'rgb' : np.array(...), 'depth' : np.array(...) }
        *
        *   @exposed    Exposed through python API
        */
        MeasurementDict getMeasurement();

        /**
        *   Returns a measurenet dictionary containing the information stored ...
        *   by the agent-intrinsics type of sensor
        *
        *   @exposed    Not exposed through python API
        */
        MeasurementDict _packIntrinsics( tysoc::sensor::TAgentIntrinsicsSensorMeasurement* measurementPtr );

        /**
        *   Returns a measurenet dictionary containing the information stored ...
        *   by the terrain-extrinsics type of sensor
        *
        *   @exposed    Not exposed through python API
        */
        // MeasurementDict _packExtrinsicsTerrain( tysoc::sensor::TSectionsTerrainSensorMeasurement* measurementPtr );

        /**
        *   Returns a pointer to the wrapped sensor object
        *
        *   @exposed    Exposed through python API
        */
        tysoc::sensor::TISensor* ptr();
    };

    class PySensorIntrinsics : public PySensor
    {

        public :

        /**
        *   Creates a python wrapper for a generic sensor.  The wrapped sensor ...
        *   object is created internally from a given sensor type.
        *   
        *   @param name                 Name for the wrapped sensor
        *   @param pyCoreAgentPtr       Agent from whom to take the measurements
        *   @exposed                    Exposed through python API
        */
        PySensorIntrinsics( const std::string& name,
                            PyCoreAgent* pyCoreAgentPtr );


    };

}


#define PYTYSOC_SENSOR_BINDINGS(m) \
    py::class_<pytysoc::PySensor>( m, "PySensor" ) \
        .def( py::init<const std::string& >() ) \
        .def( "name", &pytysoc::PySensor::name ) \
        .def( "type", &pytysoc::PySensor::type ) \
        .def( "getMeasurement", &pytysoc::PySensor::getMeasurement ); \
    py::class_<pytysoc::PySensorIntrinsics, pytysoc::PySensor>( m, "PySensorIntrinsics" ) \
        .def( py::init<const std::string&, pytysoc::PyCoreAgent*>() );

