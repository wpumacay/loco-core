#pragma once

#include <agent/agent.h>
#include <terrain/terrain.h>
#include <components/data.h>

namespace tysoc {

    /**
    * Base sensor measurement
    */
    struct TISensorMeasurement
    {
        eSensorType type;
    };

    class TISensor
    {

    public :

        TISensor( const std::string& name );
        
        virtual ~TISensor();

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

        std::string name() const { return m_name; }
        eSensorType type() const { return m_type; }

    protected :
        
        std::string m_name;
        eSensorType m_type;
    };

    
}