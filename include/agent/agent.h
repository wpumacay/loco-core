
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>


namespace tysoc {
namespace agent {

    struct TAgentBody
    {
        std::string                                 name;
        struct { float x; float y; float z; }       pos;
        struct { float vx; float vy; float vz; }    vel;
    };

    struct TAgentJoint
    {
        std::string name;
        std::string type;
        float theta;
        float thetadot;
    };

    struct TAgentGeom
    {
        std::string                             name;
        std::string                             type;
        struct { float r; float g; float b; }   color;
        struct { float x; float y; float z; }   size;
        struct { float x; float y; float z; }   pos;// absolute position (from mjv)
        float                                   rotmat[9];// absolute orientation (from mjv)
    };

    struct TAgentActuator
    {
        std::string name;
        std::string type;
        std::string linkedJointName;
        float ctrlValue;
    };

    class TAgent
    {
        protected :

        std::string                                 m_name;
        std::map< std::string, TAgentJoint* >       m_joints;
        std::map< std::string, TAgentBody* >        m_bodies;
        std::map< std::string, TAgentGeom* >        m_geometries;
        std::map< std::string, TAgentActuator* >    m_actuators;

        // @CHECK: I'm adding this to check whether or not I'll need ...
        // to access the elements of the agent by key or just use them ...
        // as a vector ( to iterate over them )

        std::vector< TAgentJoint* >       m_jointsBuffer;
        std::vector< TAgentBody* >        m_bodiesBuffer;
        std::vector< TAgentGeom* >        m_geometriesBuffer;
        std::vector< TAgentActuator* >    m_actuatorsBuffer;

        size_t m_actionDim;

        public :

        TAgent( const std::string& name );
        ~TAgent();

        void addBody( const std::string& name );
        void addJoint( const std::string& name,
                       const std::string& type );
        void addGeom( const std::string& name,
                      const std::string& type,
                      float size[3] );
        void addActuator( const std::string& name,
                          const std::string& type,
                          const std::string& linkedJointName );

        void updateJoint( const std::string& name,
                          float theta, 
                          float thetadot );
        void updateBody( const std::string& name,
                         float pos[3],
                         float vel[3] );
    
        void setCtrl( const std::string& name, float ctrlValue );
        float getCtrl( const std::string& name );

        std::map< std::string, TAgentJoint* > joints();
        std::map< std::string, TAgentBody* > bodies();
        std::map< std::string, TAgentActuator* > actuators();
        std::map< std::string, TAgentGeom* > geometries();

        std::vector< TAgentJoint* > jointsBuffer();
        std::vector< TAgentBody* > bodiesBuffer();
        std::vector< TAgentActuator* > actuatorsBuffer();
        std::vector< TAgentGeom* > geometriesBuffer();

        std::string name();
        size_t actionDim();
        
        TAgentJoint* getJoint( const std::string& name );
        TAgentBody* getBody( const std::string& name );
        TAgentGeom* getGeom( const std::string& name );

    };
    
}}