
#include <agent/types/agent_kintree_rlsim.h>


namespace tysoc {
namespace agent {

    TAgentKinTreeRlsim::TAgentKinTreeRlsim( const std::string& name,
                                            const TVec3& position,
                                            rlsim::RlsimModel* rlsimModelPtr )
        : TAgentKinTree( name, position )
    {
        // save the reference to the rlsimModelPtr for easier usage (should not modify it, read only)
        m_rlsimModelPtr = rlsimModelPtr;
        // and set the model template type as being created from a rlsim file
        m_modelTemplateType = MODEL_TEMPLATE_TYPE_RLSIM;

        _initializeKinTree();
    }

    TAgentKinTreeRlsim::~TAgentKinTreeRlsim()
    {
        // @CHECK: calling base virtual destructor?
        m_rlsimModelPtr = NULL;
    }

    void TAgentKinTreeRlsim::_constructKinTree()
    {
        m_rootBodyPtr = _processNode( m_rlsimModelPtr->rootJoint, NULL );

        // @TESTING
        _constructDefaultActuators();
    }

    TKinTreeBody* TAgentKinTreeRlsim::_processNode( rlsim::RlsimJoint* rlsimJointPtr, 
                                                    TKinTreeBody* parentKinBodyPtr )
    {
        // Joints in rlsim representation kind of map to bodies in this representation ...
        // so we could just create a body for each joint(node) and "bodies" in rlsim ...
        // representation would map to collisions in our representations.

        // Just a sanity check, as joints should have just one linked body
        if ( rlsimJointPtr->childBodies.size() != 1 )
            std::cout << "WARNING> joint: " << rlsimJointPtr->name 
                      << " should have just one child body."
                      << " It has: " << rlsimJointPtr->childBodies.size() << std::endl;

        // Grab the body information linked to this joint
        auto _rlsimBodyPtr = rlsimJointPtr->childBodies[0];

        // Create a body out of some of the joint info
        auto _kinTreeBodyPtr = new TKinTreeBody();
        _kinTreeBodyPtr->name = rlsim::computeName( "body", 
                                                    _rlsimBodyPtr->name, 
                                                    m_name );
        _kinTreeBodyPtr->parentBodyPtr = parentKinBodyPtr;

        // store it into the appropiate containers
        m_kinTreeBodies.push_back( _kinTreeBodyPtr );
        m_mapKinTreeBodies[ _kinTreeBodyPtr->name ] = _kinTreeBodyPtr;

        // grab the transform from the attachPos and attachTheta from the ...
        // joint element, as this is the one that defines the reference frame ...
        // of the body itself
        _kinTreeBodyPtr->relTransform.setPosition( rlsimJointPtr->localPos );
        _kinTreeBodyPtr->relTransform.setRotation( rlsimJointPtr->localEuler );

        // link the visuals of the "joint" to this current body
        auto _rlsimVisuals = rlsimJointPtr->childVisuals;
        for ( size_t q = 0; q < _rlsimVisuals.size(); q++ )
        {
            auto _kinTreeVisualPtr = _processVisualFromRlsim( _rlsimVisuals[q] );
            _kinTreeVisualPtr->parentBodyPtr = _kinTreeBodyPtr;
            _kinTreeBodyPtr->childVisuals.push_back( _kinTreeVisualPtr );
        }

        // create a single collision using the body info (it encodes the collision)
        auto _kinTreeCollisionPtr = _processCollisionFromRlsim( _rlsimBodyPtr );
        _kinTreeCollisionPtr->parentBodyPtr = _kinTreeBodyPtr;
        _kinTreeBodyPtr->childCollisions.push_back( _kinTreeCollisionPtr );

        // create a single joint as dof using the current rlsimjoint (it encodes the local dof)
        auto _kinTreeJointPtr = _processJointFromRlsim( rlsimJointPtr );
        _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;
        _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );

        // // create the inertia also from the body info
        // _kinTreeBodyPtr->inertiaPtr = _processInertialFromRlsim( _rlsimBodyPtr );

        // process the next nodes via the child joints
        auto _rlsimJoints = rlsimJointPtr->childJoints;
        for ( size_t q = 0; q < _rlsimJoints.size(); q++ )
        {
            _kinTreeBodyPtr->childBodies.push_back( _processNode( _rlsimJoints[q],
                                                                  _kinTreeBodyPtr ) );
        }

        return _kinTreeBodyPtr;
    }

    TKinTreeCollision* TAgentKinTreeRlsim::_processCollisionFromRlsim( rlsim::RlsimBody* rlSimBodyPtr )
    {
        // create a collision out of the body info
        auto _kinTreeCollisionPtr = new TKinTreeCollision();
        _kinTreeCollisionPtr->name = rlsim::computeName( "collision", 
                                                         rlSimBodyPtr->name, 
                                                         m_name );

        // grab the transform from the attachPos and attachTheta from the body ...
        // element, as this is the one that defines the reference frame of the ...
        // collision itself
        _kinTreeCollisionPtr->relTransform.setPosition( rlSimBodyPtr->localPos );
        _kinTreeCollisionPtr->relTransform.setRotation( rlSimBodyPtr->localEuler );
        // and the collision/geom shape
        _kinTreeCollisionPtr->geometry.type = rlSimBodyPtr->shape;
        // and the size
        _extractStandardSize( rlSimBodyPtr->shape,
                              rlSimBodyPtr->size,
                              _kinTreeCollisionPtr->geometry.size );
        _kinTreeCollisionPtr->geometry.usesFromto = false;

        // and the contype collision bitmask (@GENERIC)
        _kinTreeCollisionPtr->contype = -1;
        // and the conaffinity collision bitmask (@GENERIC)
        _kinTreeCollisionPtr->conaffinity = -1;
        // and the condim contact dimensionality (@GENERIC)
        _kinTreeCollisionPtr->condim = -1;
        // and the group the object belongs (for internal compiler calcs.) (@GENERIC)
        _kinTreeCollisionPtr->group = -1;
        // and store it in the collisions buffer
        m_kinTreeCollisions.push_back( _kinTreeCollisionPtr );
        // and to the collisions map
        m_mapKinTreeCollisions[ _kinTreeCollisionPtr->name ] = _kinTreeCollisionPtr;

        return _kinTreeCollisionPtr;
    }

    TKinTreeJoint* TAgentKinTreeRlsim::_processJointFromRlsim( rlsim::RlsimJoint* rlsimJointPtr )
    {
        // create a joint out of the joint info
        auto _kinTreeJointPtr = new TKinTreeJoint();
        _kinTreeJointPtr->name = rlsim::computeName( "joint",
                                                     rlsimJointPtr->name,
                                                     m_name );

        // the relative transform is identity, as the body frame is the ...
        // same as the joint frame for this format

        // grab the type of joint
        if ( rlsimJointPtr->type == "none" )
            _kinTreeJointPtr->type = "free";
        else
            _kinTreeJointPtr->type = rlsimJointPtr->type;

        // the joint axis is "z", as the joint frame is aligned to the body frame
        _kinTreeJointPtr->axis = TVec3( 1.0f, 0.0f, 0.0f );

        // grab the joint limits (@TODO: for ball joints should have per-dof ...
        // limits, for now just pick first one)
        _kinTreeJointPtr->lowerLimit = rad2degrees( rlsimJointPtr->limitsPerDof[0].x );
        _kinTreeJointPtr->upperLimit = rad2degrees( rlsimJointPtr->limitsPerDof[0].y );

        // all models in this format have limited constraints
        auto _lowerLimit = _kinTreeJointPtr->lowerLimit;
        auto _upperLimit = _kinTreeJointPtr->upperLimit;
        _kinTreeJointPtr->limited = ( _lowerLimit < _upperLimit ) ? true : false;
        // and the joint stiffness (@GENERIC)
        _kinTreeJointPtr->stiffness = 0.5;
        // and the joint armature (@GENERIC)
        _kinTreeJointPtr->armature = 0.0;
        // and the joint damping (@GENERIC)
        _kinTreeJointPtr->damping = 0.3;
        // child body should be set to NULL
        _kinTreeJointPtr->childBodyPtr = NULL;
        // and store it in the joints buffer
        m_kinTreeJoints.push_back( _kinTreeJointPtr );
        // and to the joints map
        m_mapKinTreeJoints[ _kinTreeJointPtr->name ] = _kinTreeJointPtr;

        return _kinTreeJointPtr;
    }

    TKinTreeVisual* TAgentKinTreeRlsim::_processVisualFromRlsim( rlsim::RlsimVisual* rlsimVisualPtr )
    {
        // create a visual out of the visual info
        auto _kinTreeVisualPtr = new TKinTreeVisual();
        _kinTreeVisualPtr->name = rlsim::computeName( "visual",
                                                      rlsimVisualPtr->name,
                                                      m_name );

        // grab the transform from the attach pos and theta from the visual element
        _kinTreeVisualPtr->relTransform.setPosition( rlsimVisualPtr->localPos );
        _kinTreeVisualPtr->relTransform.setRotation( rlsimVisualPtr->localEuler );

        // and the type of visual/geom
        _kinTreeVisualPtr->geometry.type = rlsimVisualPtr->shape;

        // and the visual size
        _extractStandardSize( rlsimVisualPtr->shape, 
                              rlsimVisualPtr->size,
                              _kinTreeVisualPtr->geometry.size );
        _kinTreeVisualPtr->geometry.usesFromto = false;

        // and the material (colors)
        auto _rgba = rlsimVisualPtr->rgba;
        _kinTreeVisualPtr->material.diffuse     = { _rgba.x, _rgba.y, _rgba.z };
        _kinTreeVisualPtr->material.specular    = { _rgba.x, _rgba.y, _rgba.z };

        // and the contype collision bitmask (@GENERIC)
        _kinTreeVisualPtr->contype = 1;
        // and the conaffinity collision bitmask (@GENERIC)
        _kinTreeVisualPtr->conaffinity = 0;
        // and the condim contact dimensionality (@GENERIC)
        _kinTreeVisualPtr->condim = -1;
        // and the group the object belongs (for internal compiler calcs.) (@GENERIC)
        _kinTreeVisualPtr->group = -1;
        // and the material name (@GENERIC)
        _kinTreeVisualPtr->materialName = "";
        // and store it in the visuals buffer
        m_kinTreeVisuals.push_back( _kinTreeVisualPtr );
        // and to the visuals map
        m_mapKinTreeVisuals[ _kinTreeVisualPtr->name ] = _kinTreeVisualPtr;

        return _kinTreeVisualPtr;
    }

    TKinTreeInertia* TAgentKinTreeRlsim::_processInertialFromRlsim( rlsim::RlsimBody* rlSimBodyPtr )
    {
        // This element has only mass, and the components are primitives ...
        // so the inertia matrix calculation is done by the backend (if it ...
        // has this feature). If not, could just do some computations for ...
        // the actual inertia matrix.
        auto _kinTreeInertia = new TKinTreeInertia();
        // grab the mass
        _kinTreeInertia->mass = rlSimBodyPtr->mass;
        // and set the inertia matrix to zeros
        _kinTreeInertia->ixx = 0.0f;
        _kinTreeInertia->iyy = 0.0f;
        _kinTreeInertia->izz = 0.0f;
        _kinTreeInertia->ixy = 0.0f;
        _kinTreeInertia->ixz = 0.0f;
        _kinTreeInertia->iyz = 0.0f;
        // localtransform-> identity, as the body frame is the inertial frame ...
        // so leave the relTransform with the default value

        return _kinTreeInertia;
    }

    void TAgentKinTreeRlsim::_constructDefaultActuators()
    {
        for ( size_t q = 0; q < m_kinTreeJoints.size(); q++ )
        {
            if ( m_kinTreeJoints[q]->type == "free" ||
                 m_kinTreeJoints[q]->type == "fixed" )
            {
                continue;
            }

            auto _kinTreeActuatorPtr = new TKinTreeActuator();
            _kinTreeActuatorPtr->name = rlsim::computeName( "actuator",
                                                            m_kinTreeJoints[q]->name,
                                                            m_name );
            // set a default "motor" type
            _kinTreeActuatorPtr->type = "motor";
            // set a reference to the joint it handles
            _kinTreeActuatorPtr->jointPtr = m_kinTreeJoints[q];
            // set some default control props
            _kinTreeActuatorPtr->minCtrl = -1;
            _kinTreeActuatorPtr->maxCtrl = 1;
            _kinTreeActuatorPtr->clampCtrl = true;
            _kinTreeActuatorPtr->kp = 0.0f;
            _kinTreeActuatorPtr->kv = 0.0f;
            _kinTreeActuatorPtr->gear = { 1, { 2.0f } };

            m_kinTreeActuators.push_back( _kinTreeActuatorPtr );
            m_mapKinTreeActuators[ _kinTreeActuatorPtr->name ] = _kinTreeActuatorPtr;
        }
    }

    void TAgentKinTreeRlsim::_extractStandardSize( const std::string& shapeType,
                                                   const TVec3& srcSize,
                                                   TVec3& dstSize )
    {
        if ( shapeType == "box" )
        {
            // boxes are given in the standard width,depth,height format
            dstSize.x = srcSize.x;
            dstSize.y = srcSize.y;
            dstSize.z = srcSize.z;
        }
        else if ( shapeType == "capsule" ||
                  shapeType == "cylinder" )
        {
            // capsule and cylinder are given in the standard height, radius format
            dstSize.x = srcSize.x;// radius
            dstSize.y = srcSize.y;// height
            // @CHECK: in case ellipsoid top-bot are needed
            dstSize.z = srcSize.z;// radius again
        }
        else if ( shapeType == "sphere" )
        {
            // @CHECK: in case ellipsoids are needed
            dstSize.x = srcSize.x;// radius
            dstSize.y = srcSize.y;// radius again
            dstSize.z = srcSize.z;// radius and again
        }
        else
        {
            std::cout << "WARNING> unsupported shape given: " << shapeType << std::endl;
        }
    }

    rlsim::RlsimModel* TAgentKinTreeRlsim::getRlsimModelDataPtr()
    {
        return m_rlsimModelPtr;
    }

    TAgentKinTree* createKinTreeAgent( const std::string& name,
                                       const TVec3& position,
                                       rlsim::RlsimModel* modelDataPtr )
    {
        auto _kinTreeAgent = new TAgentKinTreeRlsim( name, position, modelDataPtr );
        return _kinTreeAgent;
    }

}}