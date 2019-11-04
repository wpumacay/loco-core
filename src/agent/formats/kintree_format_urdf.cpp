
#include <agent/formats/kintree_format_urdf.h>

namespace tysoc {

    urdf::UrdfModel* constructAgentFromModel( TAgent* agentPtr,
                                              urdf::UrdfModel* modelDataPtr )
    {
        TUrdfParsingContext _context;
        _context.agentPtr = agentPtr;
        _context.modelDataPtr = new urdf::UrdfModel();
        _context.filepath = modelDataPtr->filepath;
        _context.folderpath = modelDataPtr->folderpath;

        // create a copy of the model data with the names modified appropriately
        urdf::deepCopy( _context.modelDataPtr, modelDataPtr, agentPtr->name() );

        /**********************************************************************/
        /*                       KINTREE CONSTRUCTION                         */
        /**********************************************************************/

        // Some sanity check. The parser already handled the warning/error message
        if ( _context.modelDataPtr->rootLinks.size() < 1 )
        {
            std::cout << "ERROR> no root links found in urdf model "
                      << "for agent with name: " << agentPtr->name() << std::endl;
        }

        // more sanity check. Just remind the user in case there are more root links
        if ( _context.modelDataPtr->rootLinks.size() > 1 )
        {
            std::cout << "WARNING> It seems that agent (" << agentPtr->name() << ") "
                      << "has more root links that it should(1). Using first one"
                      << std::endl;
        }


        // grab the first root link
        auto _rootLink = _context.modelDataPtr->rootLinks[0];
        // and start the recursive process
        auto _rootBodyPtr = _processBodyFromUrdf( _context, _rootLink, nullptr );

        if ( !_rootBodyPtr )
        {
            std::cout << "ERROR> something went wrong while parsing agent: "
                      << agentPtr->name() << ". Processed root body is nullptr" << std::endl;
        }

        // make sure we set the root for this agent we are constructing
        _context.agentPtr->setRootBody( _rootBodyPtr );

        // construct some default actuators (in case none given)
        _constructDefaultActuators( _context );

        // Set the compensation matrix (fixes orientation mismatch with format)
        _context.agentPtr->setZeroCompensationMatrix( modelDataPtr->zeroCompensation );

        // Grab all extra exclusion contacts from the urdf file
        auto _exclusionPairs = _context.modelDataPtr->exclusionPairs;
        for ( auto _exclusionPair : _exclusionPairs )
            _context.agentPtr->exclusionContacts.push_back( _exclusionPair );

        // Finally, initialize the agent
        _context.agentPtr->initialize();

        /**********************************************************************/

        return _context.modelDataPtr;
    }

    TKinTreeBody* _processBodyFromUrdf( TUrdfParsingContext& context, 
                                        urdf::UrdfLink* urdfLinkPtr, 
                                        TKinTreeBody* parentKinBodyPtr )
    {
        // grab body information
        auto _kinBody = new TKinTreeBody();
        // grab the name
        _kinBody->name = urdfLinkPtr->name;
        // and the relative transform to the parent body
        if ( urdfLinkPtr->parentJoint )
            _kinBody->localTransformZero = urdfLinkPtr->parentJoint->parentLinkToJointTransform;
        else
            _kinBody->localTransformZero.setIdentity();
        // and the parent bodyptr as well
        _kinBody->parentBodyPtr = parentKinBodyPtr;
        // and store it in the bodies buffer
        context.agentPtr->bodies.push_back( _kinBody );
        // and to the bodies map
        context.agentPtr->bodiesMap[ _kinBody->name ] = _kinBody;

        // grab all urdfvisuals and store them in the body node
        for ( auto _urdfVisual : urdfLinkPtr->visuals )
        {
            // process the element to create a TKinTreeVisual
            auto _kinVisual = _processVisualFromUrdf( context, _urdfVisual );
            // link this visual to its parent (current body being processed)
            _kinVisual->parentBodyPtr = _kinBody;
            // and add it to the children of the current body being processed
            _kinBody->visuals.push_back( _kinVisual );
        }

        // grab all urdfcollisions and store them in the body node
        for ( auto _urdfCollision : urdfLinkPtr->collisions )
        {
            // process the element to create a TKinTreeCollision
            auto _kinCollision = _processCollisionFromUrdf( context, _urdfCollision );
            // link this collision to its parent (current body being processed)
            _kinCollision->parentBodyPtr = _kinBody;
            // and add it to the children of the current body being processed
            _kinBody->collisions.push_back( _kinCollision );
        }

        // create a single joint as dof using the link's parentjoint
        auto _urdfJointPtr = urdfLinkPtr->parentJoint;
        if ( _urdfJointPtr )
        {
            auto _kinJoint = _processJointFromUrdf( context, _urdfJointPtr );
            _kinJoint->parentBodyPtr = _kinBody;
            _kinBody->joints.push_back( _kinJoint );
        }

        // grab the inertial properties (if given in file. Otherwise, will be computed by engine)
        _kinBody->inertialData = _processInertialFromUrdf( context, urdfLinkPtr->inertia );

        // @todo: Add support for sites, as some models may need them (sites ...
        // here consists of more abstract objects that can be used to trigger ...
        // special functionality in more complicated environments, like opening ...
        // something if a button is touched, or getting a reward (+/-) if this ...
        // abstract site touches an object)

        // grab child bodies (if any) and repeat process
        for ( auto _urdfChildLink : urdfLinkPtr->children )
        {
            auto _childKinTreeBodyPtr = _processBodyFromUrdf( context, _urdfChildLink, _kinBody );
            _kinBody->children.push_back( _childKinTreeBodyPtr );

            // exclude contact between this body and this child body
            context.agentPtr->exclusionContacts.push_back( std::make_pair( 
                                                                _kinBody->name,
                                                                _childKinTreeBodyPtr->name ) );
        }

        return _kinBody;
    }

    TKinTreeJoint* _processJointFromUrdf( TUrdfParsingContext& context, 
                                          urdf::UrdfJoint* urdfJointPtr )
    {
        auto _kinJointType = toEnumJoint( urdfJointPtr->type );
        auto _kinJoint = new TKinTreeJoint( _kinJointType );

        _kinJoint->name = urdfJointPtr->name;
        // and the relative transform to the parent body (urdf states that both frames coincide)
        _kinJoint->data.localTransform.setIdentity();
        _kinJoint->data.axis = urdfJointPtr->localJointAxis;
        _kinJoint->data.limits = { urdfJointPtr->lowerLimit, urdfJointPtr->upperLimit };
        _kinJoint->data.stiffness = 0.0;
        _kinJoint->data.armature = 0.0;
        _kinJoint->data.damping = 0.0;
        _kinJoint->data.ref = 0.0;

        context.agentPtr->joints.push_back( _kinJoint );
        context.agentPtr->jointsMap[ _kinJoint->name ] = _kinJoint;

        return _kinJoint;
    }

    TKinTreeVisual* _processVisualFromUrdf( TUrdfParsingContext& context, 
                                            const urdf::UrdfVisual& urdfVisual )
    {
        auto _kinVisual = new TKinTreeVisual();

        _kinVisual->name = urdfVisual.name;
        _kinVisual->data.localTransform = urdfVisual.localTransform;
        _kinVisual->data.type = toEnumShape( urdfVisual.type );
        // grab the size (transform it from urdf standard to our standard)
        _extractStandardSize( urdfVisual, _kinVisual->data.size );
        // material information
        auto _rgba = urdfVisual.material.color;
        _kinVisual->data.ambient = { _rgba.x, _rgba.y, _rgba.z };
        _kinVisual->data.diffuse = { _rgba.x, _rgba.y, _rgba.z };
        _kinVisual->data.specular = { _rgba.x, _rgba.y, _rgba.z };
        if ( urdfVisual.type == "mesh" )
        {
            _kinVisual->data.filename = context.folderpath + urdfVisual.filename;
            _kinVisual->data.usesMaterialFromMesh = urdfVisual.material.usesMaterialFromMesh;
        }

        context.agentPtr->visuals.push_back( _kinVisual );
        context.agentPtr->visualsMap[_kinVisual->name] = _kinVisual;

        return _kinVisual;
    }

    TKinTreeCollision* _processCollisionFromUrdf( TUrdfParsingContext& context, 
                                                  const urdf::UrdfCollision& urdfCollision )
    {
        auto _kinCollision = new TKinTreeCollision();

        _kinCollision->name = urdfCollision.name;
        _kinCollision->data.localTransform = urdfCollision.localTransform;
        _kinCollision->data.type = toEnumShape( urdfCollision.type );
        // grab the size (transform it from urdf standard to our standard)
        _extractStandardSize( urdfCollision, _kinCollision->data.size );
        // collision flags, and friction
        _kinCollision->data.collisionGroup = 1;
        _kinCollision->data.collisionMask = 1;
        _kinCollision->data.friction = { 1.0f, 0.005f, 0.0001f };
        _kinCollision->data.density = TYSOC_DEFAULT_DENSITY;
        if ( urdfCollision.type == "mesh" )
            _kinCollision->data.filename = context.folderpath + urdfCollision.filename;

        context.agentPtr->collisions.push_back( _kinCollision );
        context.agentPtr->collisionsMap[_kinCollision->name] = _kinCollision;

        return _kinCollision;
    }

    TInertialData _processInertialFromUrdf( TUrdfParsingContext& context, 
                                            const urdf::UrdfInertia& urdfInertia )
    {
        // For this element, check this documentation from ros-urdf: http://wiki.ros.org/urdf/XML/link
        TInertialData _kinInertial;
        // grab the transform of the inertial frame
        _kinInertial.localTransform = urdfInertia.localTransform;
        // grab the mass and inertia-matrix properties
        _kinInertial.mass = urdfInertia.mass;
        _kinInertial.ixx = urdfInertia.ixx;
        _kinInertial.iyy = urdfInertia.iyy;
        _kinInertial.izz = urdfInertia.izz;
        _kinInertial.ixy = urdfInertia.ixy;
        _kinInertial.ixz = urdfInertia.ixz;
        _kinInertial.iyz = urdfInertia.iyz;

        return _kinInertial;
    }

    void _constructDefaultActuators( TUrdfParsingContext& context )
    {
        for ( auto _joint : context.agentPtr->joints )
        {
            if ( _joint->data.type == eJointType::FREE || 
                 _joint->data.type == eJointType::FIXED )
                continue;

            auto _kinTreeActuatorPtr = new TKinTreeActuator();
            _kinTreeActuatorPtr->name = urdf::computeUrdfName( "actuator", _joint->name, context.agentPtr->name() );
            // set a default "motor" type
            _kinTreeActuatorPtr->data.type = eActuatorType::TORQUE;
            // set some default control props
            _kinTreeActuatorPtr->data.limits = { -1.0f, 1.0f };
            _kinTreeActuatorPtr->data.kp = 0.0f;
            _kinTreeActuatorPtr->data.kv = 0.0f;
            _kinTreeActuatorPtr->data.gear = { 1, { 2.0f } };
            // set a reference to the joint it handles
            _kinTreeActuatorPtr->jointPtr = _joint;

            context.agentPtr->actuators.push_back( _kinTreeActuatorPtr );
            context.agentPtr->actuatorsMap[ _kinTreeActuatorPtr->name ] = _kinTreeActuatorPtr;
        }
    }

    void _extractStandardSize( const urdf::UrdfShape& urdfShape,
                               TVec3& targetSize )
    {
        /* The sizes are given in standard form for urdf files */
        targetSize = urdfShape.size;
    }

}