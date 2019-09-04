#pragma once

#include <tysoc_common.h>
#include <components/data.h>
#include <components/bodies.h>
#include <components/joints.h>

namespace tysoc 
{


    class TSensor
    {
        public :

        TSensor( const std::string& name );

        std::string name() { return m_name; }

        protected :

        std::string m_name;
    };

}