
#pragma once


#include <scenario.h>
#include <utils/primitives_spawner.h>

namespace tysoc
{

    const std::string API_TYPE_BASE     = "BASE_API";
    const std::string API_TYPE_MUJOCO   = "MUJOCO_API";
    const std::string API_TYPE_BULLET   = "BULLET_API";

    // This is the base adapter from which other ...
    // concrete apis will inherit from (mujoco, bullet, etc.)
    class TTysocCommonApi
    {

        protected :

        std::string                 m_apiType;
        TScenario*                  m_scenarioPtr;
        utils::TPrimitivesSpawner*  m_primitivesSpawnerPtr;

        virtual void _preStep() = 0; // collect info "for" specific backend
        virtual void _updateStep() = 0; // simulate with specific backend
        virtual void _postStep() = 0; // collect infor "from" specific backend

        public :

        TTysocCommonApi();
        ~TTysocCommonApi();

        void setScenario( TScenario* scenarioPtr );
        TScenario* getScenario();
        std::string getApiType();

        void initialize();
        void step();

        // @TODO|@CHECK: should add functionality to set agent actions. So far ...
        // the functionality resides in the agents themselves, but it should be ...
        // exposed through this adapter to the user.

        // @TODO|@CHECK: I have removed the previous functionality to check a better ...
        // design and expose the necessary functionality to the user. I will add this in the next commits

        utils::TPrimitivesSpawner* getPrimitivesSpawner();
    };

}