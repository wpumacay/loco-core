
#pragma once

#include <agent/agent_base.h>


namespace tysoc{
namespace agent{

    /**
    * This is a simple agent whose purpose is mainly ..
    * for debugging some functionality around. It consists ...
    * of a signle primitive that moves around by the user commands.
    */
    class TAgentSimple : public TIAgent
    {

        protected :

        // some speed variables to make the agent...
        // move around according to the user's requests
        TVec3 m_speed;

        void _updateAgentInternal( float dt ) override;

        public :


        TAgentSimple( const std::string& name,
                      const TGeometry& geometry );
        ~TAgentSimple() override;

        void setSpeed( const TVec3& speed );
        TVec3 getSpeed();

    };

}}