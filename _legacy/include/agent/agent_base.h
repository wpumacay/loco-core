#pragma once

#include <agent/agent_components.h>
#include <utils/parsers/mjcf/mjcf.h>
#include <utils/parsers/urdf/urdf.h>
#include <utils/parsers/rlsim/rlsim.h>

#include <agent/formats/kintree_format_mjcf.h>
#include <agent/formats/kintree_format_urdf.h>
#include <agent/formats/kintree_format_rlsim.h>

namespace tysoc {

    enum class eModelFormat
    {
        NONE = 0,
        MJCF,   // mujoco file format, in the form of XML files (e.g. humanoid.xml)
        URDF,   // urdf format (as in ROS and others), in the form of XML files (e.g. laikago.urdf)
        RLSIM   // format used in TerrainRLSim (see https://github.com/UBCMOCCA/TerrainRLSim) (e.g. raptor3d.json)
    };

    std::string toString( const eModelFormat& format );

    enum class eAgentType
    {
        BASE = 0, // @todo: define remaining types of agents (when we make them :/)
    };

    std::string toString( const eAgentType& type );

    /**
    *   Agent definition
    *   ----------------
    *                               
    *   This class defines the base agent for our framework, which consists ...
    *   of a kinematic tree of body-nodes connected appropriately, which ...
    *   each body node containing resources that complete the description ...
    *   of the agent itself, like colliders, renderables, etc.. The structure ...
    *   of this kinematic tree is shown below:
    *   
    *                              (b0)->[j[0:nj],v[0:nv],c[0:nc]]                     
    *                _______________/\_____________________                                                   
    *                |                                    |
    *               (b1)->[j[]0:nj,v[0:nv],c[0:nc]]      (b2)->[j[]0:nj,v[0:nv],c[0:nc]] 
    *                /\
    *     __________/  \__________________________                                
    *     |                                      |
    *    (b3)->[j[]0:nj,v[0:nv],c[0:nc]]        (b4)->[j[]0:nj,v[0:nv],c[0:nc]]                                   
    *     |                                      |    
    *    ...                                    ...
    */
    class TAgent
    {

    public :

        /* Resources of the kinematic tree for array-like access */
        std::vector< TKinTreeBody* >        bodies;        // kinTree bodies
        std::vector< TKinTreeJoint* >       joints;        // kinTree joints
        std::vector< TKinTreeVisual* >      visuals;       // kinTree visuals
        std::vector< TKinTreeCollision* >   collisions;    // kinTree collisions
        std::vector< TKinTreeActuator* >    actuators;     // kinTree actuators
        std::vector< TKinTreeSensor* >      sensors;       // kinTree actuators

        /* Resources of the kinematic tree, hashed with their names for dict-like access */
        std::map< std::string, TKinTreeBody* >      bodiesMap;     // map of bodies
        std::map< std::string, TKinTreeJoint* >     jointsMap;     // map of joints
        std::map< std::string, TKinTreeVisual* >    visualsMap;    // map of visuals
        std::map< std::string, TKinTreeCollision* > collisionsMap; // map of collisions
        std::map< std::string, TKinTreeActuator* >  actuatorsMap;  // map of actuators
        std::map< std::string, TKinTreeSensor* >    sensorsMap;    // map of sensors

        /* Storage for contacts pairs to be excluded */
        std::vector< std::pair< std::string, std::string > > exclusionContacts;

        /* Constructs a high-level bare-bones agent, which can be constructed programatically */
        TAgent( const std::string& name,
                const TVec3& position,
                const TVec3& rotation );

        /* Constructs a high-level agent from the given mjcf model*/
        TAgent( mjcf::GenericElement* modelDataPtr,
                const std::string& name,
                const TVec3& position,
                const TVec3& rotation  );

        /* Constructs a high-level agent from the given urdf model */
        TAgent( urdf::UrdfModel* modelDataPtr,
                const std::string& name,
                const TVec3& position,
                const TVec3& rotation  );

        /* Constructs a high-level agent from the given rlsim model */
        TAgent( rlsim::RlsimModel* modelDataPtr,
                const std::string& name,
                const TVec3& position,
                const TVec3& rotation  );

        /* Releases the resources of this agent (it owns bodies, joints, ...) */
        ~TAgent();

        /* Initializes the agent to a starting configuration */
        void initialize();

        /* Updates the state of the internal components of the agent */
        void update();

        /* Resets this agent to its starting configuration */
        void reset();

        /* Sets the compensation matrix to fix position|rotation mismatches */
        void setZeroCompensationMatrix( const TMat4& mat ) { m_zeroCompensation = mat; }

        /* Sets the action vector */
        void setActions( const std::vector< TScalar >& actions );

        /* Returns the limits of a given actuator */
        TVec2 getActuatorLimits( int actuatorIndx );

        /* Returns the number of dimensions of the action vector */
        int getActionDim() const { return actuators.size(); }

        /* Sets the root pointer (recall, external parsers assemble the agent) */
        void setRootBody( TKinTreeBody* rootPtr ) { m_rootBodyPtr = rootPtr; }

        /* Sets the starting position of the agent (its root body) */
        void setStartPosition( const TVec3& position ) { m_startPosition = position; }

        /* Sets the starting rotation of the agent (its root body) */
        void setStartRotation( const TVec3& rotation ) { m_startRotation = rotation; }

        /* Returns the current position of the agent (its root body) */
        TVec3 getPosition() const { return m_position; }

        /* Returns the starting position of the agent (its root body) */
        TVec3 getStartPosition() const { return m_startPosition; }

        /* Returns the current rotation of the agent (its root body) */
        TVec3 getRotation() const { return m_rotation; }

        /* Returns the starting rotation of the agent (its root body) */
        TVec3 getStartRotation() const { return m_startRotation; }

        /* Returns the root body of the kinematic tree */
        TKinTreeBody* getRootBody() const { return m_rootBodyPtr; }

        /* Returns the stored reference to the mjcf data used for this model */
        mjcf::GenericElement* getModelDataMjcf() const { return m_modelDataMjcfPtr; }

        /* Returns the stored reference to the mjcf data used for this model */
        urdf::UrdfModel* getModelDataUrdf() const { return m_modelDataUrdfPtr; }

        /* Returns the stored reference to the mjcf data used for this model */
        rlsim::RlsimModel* getModelDataRlsim() const { return m_modelDataRlsimPtr; }

        /* Returns the unique name of this agent */
        std::string name() const { return m_name; }

        /* Returns the type of agent */
        eAgentType type() const { return m_type; }

        /* Returns the model format used to create this agent */
        eModelFormat format() const { return m_modelFormat; }

        /* Returns a summary written by internals of this agent */
        TGenericParams& summary() { return m_summary; }

        /* Returns a string representation of this agent */
        std::string toString();

    protected :

        /* Initializes extra internal of more specific (derived) agents */
        virtual void _initializeAgentInternal() {}
        /* Updates extra internals of more specific (derived) agents */
        virtual void _updateAgentInternal() {}
        /* Resets extra internals of more specific (derived) agents */
        virtual void _resetAgentInternal() {}

        /* Updates the state of the internal components of the given body */
        void _updateBodyComponents( TKinTreeBody* kinTreeBodyPtr );
        /* Updates the state of the given joint */
        void _updateJoint( TKinTreeJoint* kinTreeJointPtr );
        /* Updates the state of the given actuator */
        void _updateActuator( TKinTreeActuator* kinTreeActuatorPtr );
        /* Updates the state of the given sensor */
        void _updateSensor( TKinTreeSensor* kinTreeSensorPtr );
        /* Updates the state of the given visual */
        void _updateVisual( TKinTreeVisual* kinTreeVisualPtr );
        /* Updates the state of the given collider */
        void _updateCollision( TKinTreeCollision* kinTreeCollisionPtr );

        /* Constructs some defaults sensors, if user did not give any */
        void _constructDefaultSensors();
        /* Initializes all world transforms for all bodies to the starting configuration */
        void _initializeWorldTransforms();
        /* Initializes a given body, and continues recursively with its children */
        void _initializeBody( TKinTreeBody* kinTreeBodyPtr );

        /* Recursively traverse nodes in the tree to construct the string representation up to this node */
        std::string _strTraverse( TKinTreeBody* kinTreeBodyPtr, size_t depth );

    protected :

        /* Unique name identifier for this agent */
        std::string m_name;
        /* Type variant of this agent (if extending through inheritance) */
        eAgentType m_type;
        /* Current position of the agent (of the root body) */
        TVec3 m_position;
        /* Starting position of the agent (of the root body) */
        TVec3 m_startPosition;
        /* Current rotation of the agent, in euler angles zyx (of the root body) */
        TVec3 m_rotation;
        /* Starting rotation of the agent, in euler angles zyx (of the root body) */
        TVec3 m_startRotation;
        /* Compensation for the zero configuration */
        TMat4 m_zeroCompensation;

        /* Root body of the kinematic tree */
        TKinTreeBody* m_rootBodyPtr;

        /* Model format used to create this agent */
        eModelFormat m_modelFormat;

        /* References to parsed data from all formats */
        mjcf::GenericElement*   m_modelDataMjcfPtr;
        urdf::UrdfModel*        m_modelDataUrdfPtr;
        rlsim::RlsimModel*      m_modelDataRlsimPtr;

        /* Storage with all summary information written by the agent internals */
        TGenericParams m_summary;

    };

}