
#pragma once

#include <agent/agent.h>
#include <terrain/terrain.h>

namespace tysoc {
namespace sensor {

    const std::string SENSOR_TYPE_INTRINSICS          = "intrinsics";
    const std::string SENSOR_TYPE_EXTRINSICS_TERRAIN  = "extrinsics_terrain";


    /**
    * Base sensor measurement
    */
    struct TISensorMeasurement
    {
        std::string type;
    };


    class TISensor
    {

        protected :
        
        std::string m_name;
        std::string m_type;

        public :

        TISensor( const std::string& name );
        ~TISensor();

        std::string name();
        std::string type();

        /**
        * Should update measurements here, given specific resources
        */
        virtual void update() = 0;

        /**
        * Dump sensor information for debugging purposes
        */
        virtual void print() = 0;

        /**
        * Collects the polymorphic sensor measurement
        */
        virtual TISensorMeasurement* getSensorMeasurement() = 0;
    };

    
}}