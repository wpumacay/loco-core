
#include <agent/agent.h>

namespace tysoc {
namespace agent {

    TAgent::TAgent( const std::string& name )
    {
        m_name = name;
    }

    TAgent::~TAgent()
    {
        for ( const auto& jointPair : m_joints )
        {
            delete jointPair.second;
        }
        m_joints.clear();
        m_jointsBuffer.clear();

        for ( const auto& bodyPair : m_bodies )
        {
            delete bodyPair.second;
        }
        m_bodies.clear();
        m_bodies.clear();

        for ( const auto& geomPair : m_geometries )
        {
            delete geomPair.second;
        }
        m_geometries.clear();
        m_geometriesBuffer.clear();

        for ( const auto& actuatorPair : m_actuators )
        {
            delete actuatorPair.second;
        }
        m_actuators.clear();
        m_actuatorsBuffer.clear();
    }

    void TAgent::addJoint( const std::string& name,
                           const std::string& type )
    {
        auto _joint = new TAgentJoint();
        _joint->name = name;
        _joint->type = type;

        m_joints[ name ] = _joint;
        m_jointsBuffer.push_back( _joint );
    }
                       
    void TAgent::addBody( const std::string& name )
    {
        auto _body = new TAgentBody();
        _body->name = name;

        m_bodies[ name ] = _body;
        m_bodiesBuffer.push_back( _body );
    }

    void TAgent::addGeom( const std::string& name,
                          const std::string& type,
                          float size[3] )
    {
        auto _geom = new TAgentGeom();
        _geom->name = name;
        _geom->type = type;
        _geom->size = { size[0], size[1], size[2] };

        m_geometries[ name ] = _geom;
        m_geometriesBuffer.push_back( _geom );
    }

    void TAgent::addActuator( const std::string& name,
                              const std::string& type,
                              const std::string& linkedJointName )
    {
        auto _actuator = new TAgentActuator();
        _actuator->name = name;
        _actuator->type = type;
        _actuator->linkedJointName = linkedJointName;
        _actuator->ctrlValue = 0.0f;

        m_actuators[ name ] = _actuator;
        m_actuatorsBuffer.push_back( _actuator );
    }

    void TAgent::updateJoint( const std::string& name,
                              float theta, 
                              float thetadot )
    {
        if ( m_joints.find( name ) != m_joints.end() )
        {
            auto _joint = m_joints[ name ];
            _joint->theta = theta;
            _joint->thetadot = thetadot;
        }
    }

    void TAgent::updateBody( const std::string& name,
                             float pos[3],
                             float vel[3] )
    {
        if ( m_bodies.find( name ) != m_bodies.end() )
        {
            auto _body = m_bodies[ name ];
            _body->pos = { pos[0], pos[1], pos[2] };
            _body->vel = { vel[0], vel[1], vel[2] };
        }
    }

    void TAgent::setCtrl( const std::string& name, float ctrlValue )
    {
        if ( m_actuators.find( name ) != m_actuators.end() )
        {
            m_actuators[ name ]->ctrlValue = ctrlValue;
        }
    }

    float TAgent::getCtrl( const std::string& name )
    {
        if ( m_actuators.find( name ) != m_actuators.end() )
        {
            return m_actuators[ name ]->ctrlValue;
        }

        return 0.0f;
    }

    std::map< std::string, TAgentJoint* > TAgent::joints()
    {
        return m_joints;
    }

    std::map< std::string, TAgentBody* > TAgent::bodies()
    {
        return m_bodies;
    }

    std::map< std::string, TAgentGeom* > TAgent::geometries()
    {
        return m_geometries;
    }

    std::map< std::string, TAgentActuator* > TAgent::actuators()
    {
        return m_actuators;
    }

    std::vector< TAgentJoint* > TAgent::jointsBuffer()
    {
        return m_jointsBuffer;
    }

    std::vector< TAgentBody* > TAgent::bodiesBuffer()
    {
        return m_bodiesBuffer;
    }

    std::vector< TAgentActuator* > TAgent::actuatorsBuffer()
    {
        return m_actuatorsBuffer;
    }

    std::vector< TAgentGeom* > TAgent::geometriesBuffer()
    {
        return m_geometriesBuffer;
    }

    std::string TAgent::name()
    {
        return m_name;
    }

    size_t TAgent::actionDim()
    {
        return m_actionDim;
    }

    TAgentJoint* TAgent::getJoint( const std::string& name )
    {
        if ( m_joints.find( name ) != m_joints.end() )
        {
            return m_joints[ name ];
        }

        std::cout << "WARNING> joint with name: " << name << " not in agent" << std::endl;
        return NULL;        
    }

    TAgentBody* TAgent::getBody( const std::string& name )
    {
        if ( m_bodies.find( name ) != m_bodies.end() )
        {
            return m_bodies[ name ];
        }

        std::cout << "WARNING> body with name: " << name << " not in agent" << std::endl;
        return NULL;
    }

    TAgentGeom* TAgent::getGeom( const std::string& name )
    {
        if ( m_geometries.find( name ) != m_geometries.end() )
        {
            return m_geometries[ name ];
        }

        std::cout << "WARNING> geom with name: " << name << " not in agent" << std::endl;
        return NULL;
    }
    
}}