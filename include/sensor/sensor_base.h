
#pragma once

#include <terrain/terrain.h>
#include <agent/agent.h>

namespace tysocsensor
{

    struct TSensorMeasurement
    {
        std::string type;
    };


    class TSensor
    {

        protected :
        
        std::string m_name;

        public :

        TSensor( const std::string& name );
        ~TSensor();

        std::string name();

        /**
        * Should update measurements here, given terraingen and agent
        */
        virtual void update() = 0;

        /**
        * Collects the polymorphic sensor measurement
        */
        virtual TSensorMeasurement* getSensorMeasurement() = 0;
    };

    
}