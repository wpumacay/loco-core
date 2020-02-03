
#include <agent/formats/kintree_format_rlsim.h>

namespace tysoc {

    rlsim::RlsimModel* constructAgentFromModel( TAgent* agentPtr,
                                                rlsim::RlsimModel* modelDataPtr )
    {
        TRlsimParsingContext _context;
        _context.agentPtr = agentPtr;
        _context.modelDataPtr = modelDataPtr; // @TODO: Fix issue with deepcopy

        /**********************************************************************/
        /*                       KINTREE CONSTRUCTION                         */
        /**********************************************************************/

        // Sanity check: there should be a root for the model
        if ( !_context.modelDataPtr->rootJoint )
        {
            std::cout << "ERROR> no root link found in rlsim model "
                      << "for agent with name: " << agentPtr->name() << std::endl;
        }

        // Start recursive processing
        auto _rootBodyPtr = _processNode( _context, _context.modelDataPtr->rootJoint, NULL );

        if ( !_rootBodyPtr )
        {
            std::cout << "ERROR> something went wrong while parsing agent: "
                      << agentPtr->name() << ". Processed root body is NULL" << std::endl;
        }

        // make sure we set the root for this agent we are constructing
        _context.agentPtr->setRootBody( _rootBodyPtr );

        _constructDefaultActuators( _context );

        // Finally, initialize the agent
        _context.agentPtr->initialize();

        /**********************************************************************/

        return _context.modelDataPtr;
    }

    TKinTreeBody* _processNode( TRlsimParsingContext& context, 
                                rlsim::RlsimJoint* rlsimJointPtr, 
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
        auto _rlsimBodyPtr = rlsimJointPtr->childBodies.front();

        // Create a body out of some of the joint info
        auto _kinBody = new TKinTreeBody();
        _kinBody->name = rlsim::computeName( "body", _rlsimBodyPtr->name, context.agentPtr->name() );
        _kinBody->parentBodyPtr = parentKinBodyPtr;

        // store it into the appropiate containers
        context.agentPtr->bodies.push_back( _kinBody );
        context.agentPtr->bodiesMap[ _kinBody->name ] = _kinBody;

        // grab the transform from the attachPos and attachTheta from the ...
        // joint element, as this is the one that defines the reference frame ...
        // of the body itself
        _kinBody->localTransformZero.setPosition( rlsimJointPtr->localPos );
        _kinBody->localTransformZero.setRotation( rlsimJointPtr->localEuler );

        // link the visuals of the "joint" to this current body
        for ( auto _rlsimVisual : rlsimJointPtr->childVisuals )
        {
            auto _kinVisual = _processVisualFromRlsim( context, _rlsimVisual );
            _kinVisual->parentBodyPtr = _kinBody;
            _kinBody->visuals.push_back( _kinVisual );
        }

        // create a single collision using the body info (it encodes the collision)
        auto _kinCollision = _processCollisionFromRlsim( context, _rlsimBodyPtr );
        _kinCollision->parentBodyPtr = _kinBody;
        _kinBody->collisions.push_back( _kinCollision );

        // create the joint using the given joint information
        auto _kinJoint = _processJointFromRlsim( context, rlsimJointPtr );
        _kinJoint->parentBodyPtr = _kinBody;
        _kinBody->joints.push_back( _kinJoint );

        // @HACK: Compute density required to give required mass instead
        // of using the given mass and computing the inertia matrix ourselves
        if ( _rlsimBodyPtr->mass > 0.0f )
        {
            // Compute required density
            TScalar _targetMass = _rlsimBodyPtr->mass;
            TScalar _volume = computeVolumeFromShape( _kinCollision->data );
            _kinCollision->data.density = _targetMass / _volume;
        }

        // process the next nodes via the child joints
        for ( auto _rlsimJoint : rlsimJointPtr->childJoints )
        {
            auto _childKinBody = _processNode( context, _rlsimJoint, _kinBody );
            _kinBody->children.push_back( _childKinBody );

            // exclude contact between this body and its children
            context.agentPtr->exclusionContacts.push_back( std::make_pair( _kinBody->name, _childKinBody->name ) );
        }

        return _kinBody;
    }

    TKinTreeJoint* _processJointFromRlsim( TRlsimParsingContext& context, 
                                           rlsim::RlsimJoint* rlsimJointPtr )
    {
        auto _kinJointType = toEnumJoint( rlsimJointPtr->type );
        auto _kinJoint = new TKinTreeJoint( _kinJointType );

        // modify the name accordingly
        _kinJoint->name = rlsim::computeName( "joint", rlsimJointPtr->name, context.agentPtr->name() );
        // local-transform is identity, as the body frame coincides with the joint frame for this format
        _kinJoint->data.localTransform.setIdentity();
        // grab remaining information
        _kinJoint->data.axis = rlsimJointPtr->axis;
        _kinJoint->data.limits = rlsimJointPtr->limitsPerDof[0]; // @todo: for ball joints should have per-dof limits
        _kinJoint->data.stiffness = 0.0f;
        _kinJoint->data.armature = 0.0f;
        _kinJoint->data.damping = 0.0f;

        context.agentPtr->joints.push_back( _kinJoint );
        context.agentPtr->jointsMap[ _kinJoint->name ] = _kinJoint;

        // save the rlsim node ptr for later usage (actuators)
        context.jointNameToJointNodeMap[_kinJoint->name] = rlsimJointPtr;

        return _kinJoint;
    }

    TKinTreeVisual* _processVisualFromRlsim( TRlsimParsingContext& context, 
                                             const rlsim::RlsimVisual& rlsimVisual )
    {
        // create a visual out of the visual info
        auto _kinVisual = new TKinTreeVisual();
        _kinVisual->name = rlsim::computeName( "visual", rlsimVisual.name, context.agentPtr->name() );
        _kinVisual->data.localTransform.setPosition( rlsimVisual.localPos );
        _kinVisual->data.localTransform.setRotation( rlsimVisual.localEuler );
        _kinVisual->data.type = toEnumShape( rlsimVisual.shape );
        _extractStandardSize( rlsimVisual.shape, rlsimVisual.size, _kinVisual->data.size );
        // and the material (colors)
        auto _rgba = rlsimVisual.rgba;
        _kinVisual->data.ambient  = { _rgba.x, _rgba.y, _rgba.z };
        _kinVisual->data.diffuse  = { _rgba.x, _rgba.y, _rgba.z };
        _kinVisual->data.specular = { _rgba.x, _rgba.y, _rgba.z };
        _kinVisual->data.filename = "";
        _kinVisual->data.texture  = "img_grid"; // use this texture for all rlsim-model

        context.agentPtr->visuals.push_back( _kinVisual );
        context.agentPtr->visualsMap[ _kinVisual->name ] = _kinVisual;

        return _kinVisual;
    }

    TKinTreeCollision* _processCollisionFromRlsim( TRlsimParsingContext& context, 
                                                   rlsim::RlsimBody* rlsimBodyPtr )
    {
        // create a collision out of the body info
        auto _kinCollision = new TKinTreeCollision();
        _kinCollision->name = rlsim::computeName( "collision", rlsimBodyPtr->name, context.agentPtr->name() );
        // grab the transform from the attachPos and attachTheta from the body element, as this is 
        // the one that defines the reference frame of the collision itself
        _kinCollision->data.localTransform.setPosition( rlsimBodyPtr->localPos );
        _kinCollision->data.localTransform.setRotation( rlsimBodyPtr->localEuler );
        // grab remaining information
        _kinCollision->data.type = toEnumShape( rlsimBodyPtr->shape );
        _extractStandardSize( rlsimBodyPtr->shape, rlsimBodyPtr->size, _kinCollision->data.size );
        _kinCollision->data.collisionGroup = 1;
        _kinCollision->data.collisionMask = 1;
        _kinCollision->data.friction = { 1.0f, 0.005f, 0.0001f };
        _kinCollision->data.density = DEFAULT_DENSITY;

        context.agentPtr->collisions.push_back( _kinCollision );
        context.agentPtr->collisionsMap[ _kinCollision->name ] = _kinCollision;

        return _kinCollision;
    }

    void _constructDefaultActuators( TRlsimParsingContext& context )
    {
        for ( auto _kinJoint : context.agentPtr->joints )
        {
            if ( _kinJoint->data.type == eJointType::FREE || 
                 _kinJoint->data.type == eJointType::FIXED ||
                 _kinJoint->data.type == eJointType::PLANAR )
                continue;

            // grab the rlsim joint reference
            auto _rlsimJointPtr = context.jointNameToJointNodeMap[_kinJoint->name];

            auto _kinTreeActuatorPtr = new TKinTreeActuator();
            _kinTreeActuatorPtr->name = rlsim::computeName( "actuator", _kinJoint->name, context.agentPtr->name() );
            _kinTreeActuatorPtr->jointPtr = _kinJoint; // set a reference to the joint it handles
            _kinTreeActuatorPtr->data.type = eActuatorType::PD_CONTROLLER;
            _kinTreeActuatorPtr->data.limits = { -1.0f, 1.0f };
            _kinTreeActuatorPtr->data.kp = 50.0f;
            _kinTreeActuatorPtr->data.kv = 1.0f;

            if ( _rlsimJointPtr->torqueLimit > 0. )
            {
                _kinTreeActuatorPtr->data.gear = { 1, { _rlsimJointPtr->torqueLimit } };
            }
            else
            {
                std::cout << "WARNING> joint: " << _kinJoint->name
                          << " has no torque limits" << std::endl;
                _kinTreeActuatorPtr->data.gear = { 1, { 10. } };
            }

            context.agentPtr->actuators.push_back( _kinTreeActuatorPtr );
            context.agentPtr->actuatorsMap[ _kinTreeActuatorPtr->name ] = _kinTreeActuatorPtr;
        }
    }

    void _extractStandardSize( const std::string& shapeType,
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
            dstSize.x = 0.5 * srcSize.x;// radius
            dstSize.y = srcSize.z;// height
            dstSize.z = 0.5 * srcSize.y;// radius again
        }
        else if ( shapeType == "sphere" )
        {
            // @CHECK: in case ellipsoids are needed
            dstSize.x = 0.5 * srcSize.x;// radius
            dstSize.y = 0.5 * srcSize.y;// radius again
            dstSize.z = 0.5 * srcSize.z;// radius and again
        }
        else
        {
            std::cout << "WARNING> unsupported shape given: " << shapeType << std::endl;
        }
    }

}