
#pragma once

// just in case
#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <iostream>
#include <vector>


namespace tysocterrain
{

    struct TScenarioContext
    {
        // some time information
        float time;
        float deltatime;
        // position to use for scenario calculations
        struct { float x; float y; float z ;} trackingpoint;
        // camera frustum to cull things
        float camfrustum[9];
    };


    class TScenario
    {

        public :

        TScenario() {}
        virtual ~TScenario(){}

        virtual void initialize( TScenarioContext* scenarioContext ) = 0;
        virtual void update( TScenarioContext* scenarioContext ) = 0;

    };

}