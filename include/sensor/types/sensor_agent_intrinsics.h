
#pragma once

#include <sensor/sensor_base.h>

namespace tysoc {
namespace sensor {

    struct TAgentIntrinsicsSensorMeasurement : public TISensorMeasurement
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

    class TAgentIntrinsicsSensor : public TISensor
    {

        private :

        TAgentIntrinsicsSensorMeasurement* m_sensorMeasurement;

        agent::TIAgent* m_agentPtr;

        public :

        TAgentIntrinsicsSensor( const std::string& name,
                                agent::TIAgent* agentPtr );
        ~TAgentIntrinsicsSensor();

        void update() override;
        TISensorMeasurement* getSensorMeasurement() override;

    };

}}