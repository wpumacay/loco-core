
#pragma once

#include <sensor/sensor_base.h>

namespace tysocsensor
{

    struct TAgentIntrinsicsSensorMeasurement : public TSensorMeasurement
    {
        // bodies measurements
        struct { float x; float y; float z; } rootPosition;
        std::vector< float > bodiesRelativePosition;// @CHECK: still using geoms here

        //@CHECK: should add this once start using bodies instead of geoms
        // std::vector< float > bodiesVelocities;

        // joints measurements
        std::vector< float > thetas;
        std::vector< float > thetadots;
    };

    class TAgentIntrinsicsSensor : public TSensor
    {

        private :

        TAgentIntrinsicsSensorMeasurement* m_sensorMeasurement;

        tysocagent::TAgent* m_agentPtr;

        public :

        TAgentIntrinsicsSensor( const std::string& name,
                                tysocagent::TAgent* agentPtr );
        ~TAgentIntrinsicsSensor();

        void update() override;
        TSensorMeasurement* getSensorMeasurement() override;

    };


}