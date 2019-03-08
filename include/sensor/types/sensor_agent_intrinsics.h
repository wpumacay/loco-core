
#pragma once

#include <sensor/sensor_base.h>

namespace tysoc {
namespace sensor {

    struct TAgentIntrinsicsSensorMeasurement : public TISensorMeasurement
    {
        // Bodies measurements **************************************
        struct { TScalar x; TScalar y; TScalar z; } rootPosition;
        std::vector< TScalar > bodiesRelativePosition;
        std::vector< TVec3 > bodiesLinVelocities;
        std::vector< TVec3 > bodiesLinAccelerations;
        // external forces of COM
        std::vector< TVec3 > comForces;
        std::vector< TVec3 > comTorques;
        // **********************************************************

        // Joints measurements **************************************
        std::vector< TScalar > thetas;
        std::vector< TScalar > thetadots;
        // **********************************************************
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
        void print() override;
        TISensorMeasurement* getSensorMeasurement() override;

    };

}}