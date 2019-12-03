#pragma once

#include <tysoc_common.h>
#include <components/data.h>
#include <components/body.h>
#include <components/joint.h>

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