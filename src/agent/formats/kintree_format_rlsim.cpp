
#include <agent/formats/kintree_format_rlsim.h>

namespace tysoc {
namespace agent {

    void constructAgentFromModel( TAgent* agentPtr,
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
        auto _kinTreeBodyPtr = new TKinTreeBody();
        _kinTreeBodyPtr->name = rlsim::computeName( "body", 
                                                    _rlsimBodyPtr->name, 
                                                    context.agentPtr->name() );
        _kinTreeBodyPtr->parentBodyPtr = parentKinBodyPtr;

        // store it into the appropiate containers
        context.agentPtr->bodies.push_back( _kinTreeBodyPtr );
        context.agentPtr->bodiesMap[ _kinTreeBodyPtr->name ] = _kinTreeBodyPtr;

        // grab the transform from the attachPos and attachTheta from the ...
        // joint element, as this is the one that defines the reference frame ...
        // of the body itself
        _kinTreeBodyPtr->relTransform.setPosition( rlsimJointPtr->localPos );
        _kinTreeBodyPtr->relTransform.setRotation( rlsimJointPtr->localEuler );

        // link the visuals of the "joint" to this current body
        auto _rlsimVisuals = rlsimJointPtr->childVisuals;
        for ( size_t q = 0; q < _rlsimVisuals.size(); q++ )
        {
            auto _kinTreeVisualPtr = _processVisualFromRlsim( context, _rlsimVisuals[q] );
            _kinTreeVisualPtr->parentBodyPtr = _kinTreeBodyPtr;
            _kinTreeBodyPtr->childVisuals.push_back( _kinTreeVisualPtr );
        }

        // create a single collision using the body info (it encodes the collision)
        auto _kinTreeCollisionPtr = _processCollisionFromRlsim( context, _rlsimBodyPtr );
        _kinTreeCollisionPtr->parentBodyPtr = _kinTreeBodyPtr;
        _kinTreeBodyPtr->childCollisions.push_back( _kinTreeCollisionPtr );

        // create a single joint as dof using the current rlsimjoint (it encodes the local dof)
        // @HACK: If there is a spherical joint, treat is as 3 separate revolute joints
    #if HACK_SPLIT_SPHERICAL_JOINTS
        if ( rlsimJointPtr->type == "spherical" )
    #else
        if ( false )
    #endif
        {
            std::vector< rlsim::RlsimJoint* > _joints;
            _joints.push_back( new rlsim::RlsimJoint() );
            _joints.push_back( new rlsim::RlsimJoint() );
            _joints.push_back( new rlsim::RlsimJoint() );

            // configure each joint appropriately
            for ( size_t q = 0; q < _joints.size(); q++ )
            {
                _joints[q]->name = rlsimJointPtr->name + "_" + std::to_string( (q + 1) );
                _joints[q]->type = "revolute";
                _joints[q]->localPos = rlsimJointPtr->localPos;
                _joints[q]->localEuler = rlsimJointPtr->localEuler;
                _joints[q]->parentJointId = rlsimJointPtr->parentJointId;
                _joints[q]->torqueLimit = rlsimJointPtr->torqueLimit;
                _joints[q]->limitsPerDof.push_back( rlsimJointPtr->limitsPerDof[q] );

                if ( q == 0 )
                    _joints[q]->axis = { 1., 0., 0. };
                else if ( q == 1 )
                    _joints[q]->axis = { 0., 1., 0. };
                else
                    _joints[q]->axis = { 0., 0., 1. };
            }

            // construct all joints for this spherical
            for ( size_t q = 0; q < _joints.size(); q++ )
            {
                auto _kinTreeJointPtr = _processJointFromRlsim( context, _joints[q] );
                _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;
                _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
            }

            // save these dummies for ourselves (later usage)
            for ( size_t q = 0; q < _joints.size(); q++ )
                context.hackSphericalJoints.push_back( _joints[q] );

            _joints.clear();
        }
        else
        {
        #if ENABLE_PLANAR_JOINTS
            if ( rlsimJointPtr->type == "planar" )
            {
                auto _rlsimJointSlideX = new rlsim::RlsimJoint();
                auto _rlsimJointSlideZ = new rlsim::RlsimJoint();
                auto _rlsimJointHingeY = new rlsim::RlsimJoint();

                _rlsimJointSlideX->name = rlsimJointPtr->name + "_planar_fix_slidex";
                _rlsimJointSlideZ->name = rlsimJointPtr->name + "_planar_fix_slidez";
                _rlsimJointHingeY->name = rlsimJointPtr->name + "_planar_fix_hingey";

                _rlsimJointSlideX->type = "slide";
                _rlsimJointSlideZ->type = "slide";
                _rlsimJointHingeY->type = "hinge";

                // _rlsimJointSlideX->axis = { 1.0f, 0.0f, 0.0f };
                // _rlsimJointSlideZ->axis = { 0.0f, 0.0f, 1.0f };
                // _rlsimJointHingeY->axis = { 0.0f, 1.0f, 0.0f };

                _rlsimJointSlideX->axis = { 0.0f, 1.0f, 0.0f };
                _rlsimJointSlideZ->axis = { 0.0f, 0.0f, 1.0f };
                _rlsimJointHingeY->axis = { 1.0f, 0.0f, 0.0f };

                _rlsimJointSlideX->limitsPerDof = { { 1.0f, -1.0f } };
                _rlsimJointSlideZ->limitsPerDof = { { 1.0f, -1.0f } };
                _rlsimJointHingeY->limitsPerDof = { { 1.0f, -1.0f } };

                std::vector< rlsim::RlsimJoint* > _rlsimJointsPlanar = { _rlsimJointSlideX,
                                                                         _rlsimJointSlideZ,
                                                                         _rlsimJointHingeY };

                for ( size_t q = 0; q < _rlsimJointsPlanar.size(); q++ )
                {
                    auto _kinTreeJointPtr = _processJointFromRlsim( context, _rlsimJointsPlanar[q] );
                    _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;
                    _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
                }
            }
            else
            {
                auto _kinTreeJointPtr = _processJointFromRlsim( context, rlsimJointPtr );
                _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;
                _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
            }
        #else
            if ( rlsimJointPtr->type == "planar" )
                rlsimJointPtr->type = "none";

            auto _kinTreeJointPtr = _processJointFromRlsim( context, rlsimJointPtr );
            _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;
            _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
        #endif
        }

        // @HACK: Compute density required to give required mass instead
        // of using the given mass and computing the inertia matrix ourselves
        if ( _rlsimBodyPtr->mass > 0. )
        {
            // Compute required density
            TScalar _targetMass = _rlsimBodyPtr->mass;
            TScalar _volume = computeVolumeFromShape( _kinTreeCollisionPtr->geometry.type,
                                                      _kinTreeCollisionPtr->geometry.size );
            _kinTreeCollisionPtr->density = _targetMass / _volume;
        }

        // process the next nodes via the child joints
        auto _rlsimJoints = rlsimJointPtr->childJoints;
        for ( size_t q = 0; q < _rlsimJoints.size(); q++ )
        {
            auto _childKinTreeBodyPtr = _processNode( context, 
                                                      _rlsimJoints[q],
                                                      _kinTreeBodyPtr );
            _kinTreeBodyPtr->childBodies.push_back( _childKinTreeBodyPtr );

            // exclude contact between this body and this child body
            context.agentPtr->exclusionContacts.push_back( std::make_pair( 
                                                                _kinTreeBodyPtr->name,
                                                                _childKinTreeBodyPtr->name ) );
        }

        return _kinTreeBodyPtr;
    }

    TKinTreeJoint* _processJointFromRlsim( TRlsimParsingContext& context, 
                                           rlsim::RlsimJoint* rlsimJointPtr )
    {
        // create a joint out of the joint info
        auto _kinTreeJointPtr = new TKinTreeJoint();
        _kinTreeJointPtr->name = rlsim::computeName( "joint",
                                                     rlsimJointPtr->name,
                                                     context.agentPtr->name() );

        // the relative transform is identity, as the body frame is the ...
        // same as the joint frame for this format

        // check if this joint is from a planar joint
        bool _fromPlanarJoint = false;
        if ( rlsimJointPtr->name.find( "_planar_fix_" ) != std::string::npos )
            _fromPlanarJoint = true;

        // grab the type of joint
        if ( rlsimJointPtr->type == "none" )
            _kinTreeJointPtr->type = "free";
        else
            _kinTreeJointPtr->type = rlsimJointPtr->type;

        _kinTreeJointPtr->axis = rlsimJointPtr->axis;

        // grab the joint limits (@TODO: for ball joints should have per-dof ...
        // limits, for now just pick first one)
        _kinTreeJointPtr->lowerLimit = rlsimJointPtr->limitsPerDof[0].x;
        _kinTreeJointPtr->upperLimit = rlsimJointPtr->limitsPerDof[0].y;

        // all models in this format have limited constraints
        auto _lowerLimit = _kinTreeJointPtr->lowerLimit;
        auto _upperLimit = _kinTreeJointPtr->upperLimit;
        _kinTreeJointPtr->limited = ( _lowerLimit < _upperLimit ) ? true : false;
        // and the joint stiffness
        _kinTreeJointPtr->stiffness = _fromPlanarJoint ? 0.0 : 0.5;
        // and the joint armature
        _kinTreeJointPtr->armature = 0.0;
        // and the joint damping
        _kinTreeJointPtr->damping = _fromPlanarJoint ? 0.0 : 0.3;
        // and store it in the joints buffer
        context.agentPtr->joints.push_back( _kinTreeJointPtr );
        // and to the joints map
        context.agentPtr->jointsMap[ _kinTreeJointPtr->name ] = _kinTreeJointPtr;

        // save the rlsim node ptr for later usage (actuators)
        context.jointNameToJointNodeMap[ _kinTreeJointPtr->name ] = rlsimJointPtr;

        return _kinTreeJointPtr;
    }

    TKinTreeVisual* _processVisualFromRlsim( TRlsimParsingContext& context, 
                                             rlsim::RlsimVisual* rlsimVisualPtr )
    {
        // create a visual out of the visual info
        auto _kinTreeVisualPtr = new TKinTreeVisual();
        _kinTreeVisualPtr->name = rlsim::computeName( "visual",
                                                      rlsimVisualPtr->name,
                                                      context.agentPtr->name() );

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

        // and the material name
        _kinTreeVisualPtr->material.name = "";
        // and store it in the visuals buffer
        context.agentPtr->visuals.push_back( _kinTreeVisualPtr );
        // and to the visuals map
        context.agentPtr->visualsMap[ _kinTreeVisualPtr->name ] = _kinTreeVisualPtr;

        return _kinTreeVisualPtr;
    }

    TKinTreeCollision* _processCollisionFromRlsim( TRlsimParsingContext& context, 
                                                   rlsim::RlsimBody* rlsimBodyPtr )
    {
        // create a collision out of the body info
        auto _kinTreeCollisionPtr = new TKinTreeCollision();
        _kinTreeCollisionPtr->name = rlsim::computeName( "collision", 
                                                         rlsimBodyPtr->name, 
                                                         context.agentPtr->name() );

        // grab the transform from the attachPos and attachTheta from the body ...
        // element, as this is the one that defines the reference frame of the ...
        // collision itself
        _kinTreeCollisionPtr->relTransform.setPosition( rlsimBodyPtr->localPos );
        _kinTreeCollisionPtr->relTransform.setRotation( rlsimBodyPtr->localEuler );
        // and the collision/geom shape
        _kinTreeCollisionPtr->geometry.type = rlsimBodyPtr->shape;
        // and the size
        _extractStandardSize( rlsimBodyPtr->shape,
                              rlsimBodyPtr->size,
                              _kinTreeCollisionPtr->geometry.size );
        _kinTreeCollisionPtr->geometry.usesFromto = false;

        // and the contype collision bitmask
        _kinTreeCollisionPtr->contype = 1;
        // and the conaffinity collision bitmask
        _kinTreeCollisionPtr->conaffinity = 1;
        // and the condim contact dimensionality
        _kinTreeCollisionPtr->condim = 3;
        // and the group the object belongs (for internal compiler calcs.)
        // and store it in the collisions buffer
        context.agentPtr->collisions.push_back( _kinTreeCollisionPtr );
        // and to the collisions map
        context.agentPtr->collisionsMap[ _kinTreeCollisionPtr->name ] = _kinTreeCollisionPtr;

        return _kinTreeCollisionPtr;
    }

    void _constructDefaultActuators( TRlsimParsingContext& context )
    {
        for ( size_t q = 0; q < context.agentPtr->joints.size(); q++ )
        {
            if ( context.agentPtr->joints[q]->name.find( "_planar_fix_" ) !=
                 std::string::npos )
            {
                continue;
            }

            if ( context.agentPtr->joints[q]->type == "free" ||
                 context.agentPtr->joints[q]->type == "fixed" )
            {
                continue;
            }

            // grab the rlsim joint reference
            auto _rlsimJointPtr = context.jointNameToJointNodeMap[context.agentPtr->joints[q]->name];

            auto _kinTreeActuatorPtr = new TKinTreeActuator();
            _kinTreeActuatorPtr->name = rlsim::computeName( "actuator",
                                                            context.agentPtr->joints[q]->name,
                                                            context.agentPtr->name() );
            // set a default "motor" type
            _kinTreeActuatorPtr->type = "motor";
            // set a reference to the joint it handles
            _kinTreeActuatorPtr->jointPtr = context.agentPtr->joints[q];
            // set some default control props
            _kinTreeActuatorPtr->minCtrl = -1;
            _kinTreeActuatorPtr->maxCtrl = 1;
            _kinTreeActuatorPtr->clampCtrl = true;
            _kinTreeActuatorPtr->kp = 0.0f;
            _kinTreeActuatorPtr->kv = 0.0f;

            if ( _rlsimJointPtr->torqueLimit > 0. )
            {
                _kinTreeActuatorPtr->gear = { 1, { _rlsimJointPtr->torqueLimit } };
            }
            else
            {
                std::cout << "WARNING> joint: " << context.agentPtr->joints[q]->name
                          << " has no torque limits" << std::endl;
                _kinTreeActuatorPtr->gear = { 1, { 10. } };
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

}}