
#pragma once

#include <sensor/sensor_base.h>

namespace tysocsensor
{

    struct TPathTerrainSensorMeasurement : public TSensorMeasurement
    {
        std::vector< float > profile;
    };

    class TPathTerrainSensor : public TSensor
    {
        private :

        TPathTerrainSensorMeasurement* m_sensorMeasurement;

        public :

        TPathTerrainSensor( tysocterrain::TPathTerrainGenerator* terrainGenPtr,
                            tysocagent::TAgent* agentPtr );
        ~TPathTerrainSensor();

        void update() override;
        TSensorMeasurement* getSensorMeasurement() override;
    };


}