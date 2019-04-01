
#pragma once

#include <tysoc_common.h>

namespace tysoc{
namespace agent{

    const std::string AGENT_TYPE_SIMPLE     = "simple";
    const std::string AGENT_TYPE_KINTREE    = "kintree";


    /**
    * This is the base interface for the agents in the library.
    */
    class TIAgent
    {

        protected : 

        std::vector< TGeometry >    m_geometries;
        std::string                 m_name;
        std::string                 m_type;
        TVec3                       m_position;
        TVec3                       m_startPosition;
        TVec3                       m_rotation;
        TVec3                       m_startRotation;

        // agent specific functionality
        virtual void _updateAgentInternal( float dt ) = 0;
        virtual void _resetAgentInternal() = 0;

        public :

        TIAgent( const std::string& name,
                 const TVec3& position,
                 const TVec3& rotation = TVec3() );
        virtual ~TIAgent();

        void setPosition( const TVec3& position );
        void setStartPosition( const TVec3& position );
        void setRotation( const TVec3& rotation );
        void setStartRotation( const TVec3& rotation );

        TVec3 getPosition();
        TVec3 getStartPosition();
        TVec3 getRotation();
        TVec3 getStartRotation();

        void update( float dt );
        void reset();

        std::string name();
        std::string type();
        const std::vector< TGeometry >& getGeometries() const;
    };



}}