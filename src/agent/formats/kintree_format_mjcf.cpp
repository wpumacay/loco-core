
#include <agent/formats/kintree_format_mjcf.h>

namespace tysoc {
namespace agent {

    void constructAgentFromModel( TAgent* agentPtr,
                                  mjcf::GenericElement* modelDataPtr )
    {
        // Create and initialize the context
        TMjcfParsingContext _context;
        _context.agentPtr = agentPtr;
        _context.modelDataPtr = new mjcf::GenericElement();

        // create a copy of the model data with the names modified appropriately
        mjcf::deepCopy( _context.modelDataPtr, modelDataPtr, NULL, agentPtr->name() );

        // grab some required info before start constructing the kintree
        _extractMjcfModelSettings( _context );
        _collectDefaults( _context );
        _collectAssets( _context );

        /**********************************************************************/
        /*                       KINTREE CONSTRUCTION                         */
        /**********************************************************************/

        auto _worldBodyElmPtr = mjcf::findFirstChildByType( _context.modelDataPtr,
                                                            "worldbody" );

        if ( !_worldBodyElmPtr )
        {
            std::cout << "ERROR> something went wrong while parsing agent: "
                      << agentPtr->name() << ". Model data doesn't have [worldbody]" << std::endl;
        }

        auto _rootBodyElmPtr = mjcf::findFirstChildByType( _worldBodyElmPtr, 
                                                           "body" );

        if ( !_rootBodyElmPtr )
        {
            std::cout << "ERROR> something went wrong while parsing agent: "
                      << agentPtr->name() << ". Model data doesn't have a root [body]" << std::endl;
        }

        // start recursive processing of all kintree components, starting at root
        auto _rootBodyPtr = _processBodyFromMjcf( _context, _rootBodyElmPtr, NULL );

        if ( !_rootBodyPtr )
        {
            std::cout << "ERROR> something went wrong while parsing agent: "
                      << agentPtr->name() << ". Processed root body is NULL" << std::endl;
        }

        // make sure we set the root for this agent we are constructing
        _context.agentPtr->setRootBody( _rootBodyPtr );

        // grab the actuators. Sensors are just copied, ...
        // and the joint sensors are created as needed
        auto _actuatorsElmPtr = mjcf::findFirstChildByType( _context.modelDataPtr, 
                                                            "actuator" );
        if ( _actuatorsElmPtr )
        {
            for ( size_t i = 0; i < _actuatorsElmPtr->children.size(); i++ )
            {
                // create the actual actuator
                _processActuator( _context, _actuatorsElmPtr->children[i] );
            }
        }

        if ( !_context.useLocalCoordinates )
            _convertGlobalToLocalCoordinates( _rootBodyPtr, TMat4() );

        // Finally, initialize the agent
        _context.agentPtr->initialize();

        /**********************************************************************/
    }

    void _collectDefaults( TMjcfParsingContext& context )
    {
        auto _defaultsElmPtr = mjcf::findFirstChildByType( context.modelDataPtr, "default" );

        if ( !_defaultsElmPtr )
            return;

        for ( size_t q = 0; q < _defaultsElmPtr->children.size(); q++ )
        {
            auto _child = _defaultsElmPtr->children[q];
            if ( _child->etype == "default" )
                _collectDefaultsFromClass( context, _child );
            else
                _collectDefaultsNoClass( context, _child );
        }
    }

    void _collectDefaultsFromClass( TMjcfParsingContext& context, 
                                    mjcf::GenericElement* defClassElmPtr )
    {
        auto _classId = defClassElmPtr->getAttributeString( "class" );
        if ( _classId == "" )
            return;

        if ( context.defaultsPerClass.find( _classId ) == context.defaultsPerClass.end() )
            context.defaultsPerClass[_classId] = std::map< std::string, TGenericParams >();

        auto _children = defClassElmPtr->children;
        for ( size_t q = 0; q < _children.size(); q++ )
        {
            auto _child = _children[q];
            auto _etype = _child->etype;
            auto _possibleAttribsDict = mjcf::MJCF_SCHEMA->getPossibleAttribs( _etype );

            for ( auto _it = _possibleAttribsDict.begin();
                       _it != _possibleAttribsDict.end();
                       _it++ )
            {
                if ( _it->second == mjcf::TYPE_ARRAY_FLOAT &&
                     _child->hasAttributeArrayFloat( _it->first ) )
                {
                    context.defaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeArrayFloat( _it->first ) );
                }
                else if ( _it->second == mjcf::TYPE_ARRAY_INT &&
                          _child->hasAttributeArrayInt( _it->first ) )
                {
                    context.defaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeArrayInt( _it->first ) );
                }
                else if ( _it->second == mjcf::TYPE_FLOAT &&
                          _child->hasAttributeFloat( _it->first ) )
                {
                    context.defaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeFloat( _it->first ) );
                }
                else if ( _it->second == mjcf::TYPE_INT &&
                          _child->hasAttributeInt( _it->first ) )
                {
                    context.defaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeInt( _it->first ) );
                }
                else if ( _it->second == mjcf::TYPE_STRING &&
                          _child->hasAttributeString( _it->first ) )
                {
                    context.defaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeString( _it->first ) );
                }
            }
        }
    }

    void _collectDefaultsNoClass( TMjcfParsingContext& context, 
                                  mjcf::GenericElement* defElmPtr )
    {
        auto _tagId = defElmPtr->etype;
        auto _possibleAttribsDict = mjcf::MJCF_SCHEMA->getPossibleAttribs( _tagId );

        for ( auto _it = _possibleAttribsDict.begin();
                   _it != _possibleAttribsDict.end();
                   _it++ )
        {
            if ( _it->second == mjcf::TYPE_ARRAY_FLOAT &&
                 defElmPtr->hasAttributeArrayFloat( _it->first ) )
            {
                context.defaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeArrayFloat( _it->first ) );
            }
            else if ( _it->second == mjcf::TYPE_ARRAY_INT &&
                      defElmPtr->hasAttributeArrayInt( _it->first ) )
            {
                context.defaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeArrayInt( _it->first ) );
            }
            else if ( _it->second == mjcf::TYPE_FLOAT &&
                      defElmPtr->hasAttributeFloat( _it->first ) )
            {
                context.defaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeFloat( _it->first ) );
            }
            else if ( _it->second == mjcf::TYPE_INT &&
                      defElmPtr->hasAttributeInt( _it->first ) )
            {
                context.defaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeInt( _it->first ) );
            }
            else if ( _it->second == mjcf::TYPE_STRING &&
                      defElmPtr->hasAttributeString( _it->first ) )
            {
                context.defaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeString( _it->first ) );
            }
        }
    }

    void _collectAssets( TMjcfParsingContext& context )
    {
        auto _assetsElmPtr = mjcf::findFirstChildByType( context.modelDataPtr, "asset" );
        if ( !_assetsElmPtr )
            return;// Nothing to do if no assets found

        auto _assets = _assetsElmPtr->children;
        for ( size_t i = 0; i < _assets.size(); i++ )
        {
            auto _asset = _assets[i];
            if ( _asset->etype == "mesh" )
            {
                // Grab the mesh asset into a dict for later usage *************
                auto _meshAsset = new TKinTreeMeshAsset();

                // grab the name of the mesh
                _meshAsset->name = _asset->getAttributeString( "name" );
                // and the file resource
                _meshAsset->file = _asset->getAttributeString( "file" );

                // as well as the scale (in case there is)
                _meshAsset->scale = _asset->getAttributeVec3( "scale", 
                                                              { 1.0f, 1.0f, 1.0f } );

                if ( context.agentPtr->meshAssetsMap.find( _meshAsset->name ) ==
                     context.agentPtr->meshAssetsMap.end() )
                {
                    context.agentPtr->meshAssets.push_back( _meshAsset );
                    context.agentPtr->meshAssetsMap[ _meshAsset->name ] = _meshAsset;
                }
                else
                {
                    delete _meshAsset;
                    _meshAsset = NULL;
                }
                // *************************************************************
            }
            else
            {
                // Other asset types are not supported yet
                std::cout << "WARNING> asset of type (" << _asset->etype 
                          << " isn't supported yet" << std::endl;
            }
        }
    }

    TKinTreeBody* _processBodyFromMjcf( TMjcfParsingContext& context, 
                                        mjcf::GenericElement* bodyElementPtr,
                                        TKinTreeBody* parentKinBodyPtr )
    {
        // grab body information
        auto _kinTreeBodyPtr = new TKinTreeBody();
        // grab the name
        _kinTreeBodyPtr->name = bodyElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( context, bodyElementPtr, _kinTreeBodyPtr->relTransform );
        // and the parent bodyptr as well
        _kinTreeBodyPtr->parentBodyPtr = parentKinBodyPtr;
        // and store it in the bodies buffer
        context.agentPtr->bodies.push_back( _kinTreeBodyPtr );
        // and to the bodies map
        context.agentPtr->bodiesMap[ _kinTreeBodyPtr->name ] = _kinTreeBodyPtr;

        // grab the child-class (if given)
        auto _childClass = bodyElementPtr->getAttributeString( "childclass", "" );

        // grab all geom elements and process them as visual and collisions
        auto _geomElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "geom" );
        for ( size_t i = 0; i < _geomElementPtrs.size(); i++ )
        {
            // set child-class if geom element does not have any
            if ( _childClass != "" && !_geomElementPtrs[i]->hasAttributeString( "class" ) )
                _geomElementPtrs[i]->setAttributeString( "class", _childClass );

            auto _kinTreeVisualPtr      = _processVisualFromMjcf( context, _geomElementPtrs[i] );
            auto _kinTreeCollisionPtr   = _processCollisionFromMjcf( context, _geomElementPtrs[i] );

            _kinTreeVisualPtr->parentBodyPtr    = _kinTreeBodyPtr;
            _kinTreeCollisionPtr->parentBodyPtr = _kinTreeBodyPtr;

            _kinTreeBodyPtr->childVisuals.push_back( _kinTreeVisualPtr );
            _kinTreeBodyPtr->childCollisions.push_back( _kinTreeCollisionPtr );
        }

        // grab all joint elements and process them
        auto _jointElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "joint" );
        for ( size_t i = 0; i < _jointElementPtrs.size(); i++ )
        {
            // set child-class if joint element does not have any
            if ( _childClass != "" && !_jointElementPtrs[i]->hasAttributeString( "class" ) )
                _jointElementPtrs[i]->setAttributeString( "class", _childClass );

            auto _kinTreeJointPtr =  _processJointFromMjcf( context, _jointElementPtrs[i] );

            _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;

            _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
        }

        // grab the inertial properties (if given, as by default is computed from the geometries)
        auto _inertialElmPtr = mjcf::findFirstChildByType( bodyElementPtr, "inertial" );
        if ( _inertialElmPtr )
        {
            _kinTreeBodyPtr->inertiaPtr = _processInertialFromMjcf( context, _inertialElmPtr );
        }

        // @TODO: Add support for sites, as some models may need them

        // grab child bodies (if any) and repeat process
        auto _childBodiesElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "body" );
        for ( size_t i = 0; i < _childBodiesElementPtrs.size(); i++ )
        {
            // set child-class if joint element does not have any
            if ( _childClass != "" && !_childBodiesElementPtrs[i]->hasAttributeString( "childclass" ) )
                _childBodiesElementPtrs[i]->setAttributeString( "childclass", _childClass );

            _kinTreeBodyPtr->childBodies.push_back( _processBodyFromMjcf( context,
                                                                          _childBodiesElementPtrs[i], 
                                                                          _kinTreeBodyPtr ) );
        }

        return _kinTreeBodyPtr;
    }

    TKinTreeJoint* _processJointFromMjcf( TMjcfParsingContext& context, 
                                          mjcf::GenericElement* jointElementPtr )
    {
        auto _kinTreeJointPtr = new TKinTreeJoint();
        // grab the name
        _kinTreeJointPtr->name = jointElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( context, jointElementPtr, _kinTreeJointPtr->relTransform );
        // and the type of joint
        _kinTreeJointPtr->type = _grabString( context, jointElementPtr, "type", "hinge" );
        // and the joint axis
        auto _axis = _grabVec3( context, jointElementPtr, "axis", { 0., 1., 0. } );
        _kinTreeJointPtr->axis = { _axis.x, _axis.y, _axis.z };
        // and the joint value clamping flag
        _kinTreeJointPtr->limited = ( _grabString( context, jointElementPtr, "limited", "false" ) == "true" );
        _kinTreeJointPtr->limited |= jointElementPtr->hasAttributeVec2( "range" );
        // in case not limited, assume low > high
        if ( !_kinTreeJointPtr->limited )
        {
            _kinTreeJointPtr->lowerLimit = 1.0f;
            _kinTreeJointPtr->upperLimit = -1.0f;
        }
        else
        {
            // and the range limits
            if ( context.useDegrees )
            {
                auto _limits = _grabVec2( context, jointElementPtr, "range", { -180., 180. } );
                _kinTreeJointPtr->lowerLimit = degrees2rad( _limits.x );
                _kinTreeJointPtr->upperLimit = degrees2rad( _limits.y );
            }
            else
            {
                auto _limits = _grabVec2( context, jointElementPtr, "range", { -TYSOC_PI, TYSOC_PI } );
                _kinTreeJointPtr->lowerLimit = _limits.x;
                _kinTreeJointPtr->upperLimit = _limits.y;
            }
        }
        // and the joint stiffness
        _kinTreeJointPtr->stiffness = _grabFloat( context, jointElementPtr, "stiffness", 0.0 );
        // and the joint armature
        _kinTreeJointPtr->armature = _grabFloat( context, jointElementPtr, "armature", 0.0 );
        // and the joint damping
        _kinTreeJointPtr->damping = _grabFloat( context, jointElementPtr, "damping", 0.0 );
        // and the joint ref
        _kinTreeJointPtr->ref = _grabFloat( context, jointElementPtr, "ref", 0.0 );
        // and store it in the joints buffer
        context.agentPtr->joints.push_back( _kinTreeJointPtr );
        // and to the joints map
        context.agentPtr->jointsMap[ _kinTreeJointPtr->name ] = _kinTreeJointPtr;

        // let the joint configure its internal props
        _kinTreeJointPtr->configure();

        return _kinTreeJointPtr;
    }

    TKinTreeVisual* _processVisualFromMjcf( TMjcfParsingContext& context, 
                                            mjcf::GenericElement* geomElementPtr )
    {
        auto _kinTreeVisualPtr = new TKinTreeVisual();
        // grab the name
        _kinTreeVisualPtr->name = geomElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( context, geomElementPtr, _kinTreeVisualPtr->relTransform );
        // and the type of visual/geom
        _kinTreeVisualPtr->geometry.type = _grabString( context, geomElementPtr, "type", "" );
        if ( _kinTreeVisualPtr->geometry.type == "" )
        {
            _kinTreeVisualPtr->geometry.type = "sphere";
        }
        // and the mesh filename in case there is any (this one is tricky)
        auto _meshId = _grabString( context, geomElementPtr, "mesh", "" );
        if ( context.agentPtr->meshAssetsMap.find( _meshId ) != 
             context.agentPtr->meshAssetsMap.end() )
        {
            // if the mesh is is a "pure link" to a filename, then use the stored asset in map
            _kinTreeVisualPtr->geometry.meshId   = context.agentPtr->meshAssetsMap[ _meshId ]->name;
            _kinTreeVisualPtr->geometry.filename = context.agentPtr->meshAssetsMap[ _meshId ]->file;
        }
        else
        {
            // if not, then is a pure path to the mesh file
            _kinTreeVisualPtr->geometry.meshId      = "";
            _kinTreeVisualPtr->geometry.filename    = _meshId;
        }

        // and the visual/geom size (and check if uses fromto, so we can extract the relative transform)
        TVec3 _posFromFromto;
        TMat3 _rotFromFromto;
        bool _usesFromto = _extractStandardSize( context, 
                                                 geomElementPtr, 
                                                 _kinTreeVisualPtr->geometry.size,
                                                 _posFromFromto,
                                                 _rotFromFromto );
        _kinTreeVisualPtr->geometry.usesFromto = _usesFromto;
        if ( _usesFromto )
        {
            _kinTreeVisualPtr->relTransform.setPosition( _posFromFromto );
            _kinTreeVisualPtr->relTransform.setRotation( _rotFromFromto );
        }

        auto _rgba = _grabVec4( context, geomElementPtr, "rgba", TYSOC_DEFAULT_RGBA_COLOR );

        _kinTreeVisualPtr->material.diffuse     = { _rgba.x, _rgba.y, _rgba.z };
        _kinTreeVisualPtr->material.specular    = { _rgba.x, _rgba.y, _rgba.z };

        // and the material name
        _kinTreeVisualPtr->material.name = _grabString( context, geomElementPtr, "material", "" );
        // and store it in the visuals buffer
        context.agentPtr->visuals.push_back( _kinTreeVisualPtr );
        // and to the visuals map
        context.agentPtr->visualsMap[ _kinTreeVisualPtr->name ] = _kinTreeVisualPtr;

        return _kinTreeVisualPtr;
    }

    TKinTreeCollision* _processCollisionFromMjcf( TMjcfParsingContext& context, 
                                                  mjcf::GenericElement* geomElementPtr )
    {
        auto _kinTreeCollisionPtr = new TKinTreeCollision();
        // grab the name
        _kinTreeCollisionPtr->name = geomElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( context, geomElementPtr, _kinTreeCollisionPtr->relTransform );
        // and the collision/geom
        _kinTreeCollisionPtr->geometry.type = _grabString( context, geomElementPtr, "type", "" );
        if ( _kinTreeCollisionPtr->geometry.type == "" )
        {
            _kinTreeCollisionPtr->geometry.type = "sphere";
        }
        // and the mesh filename in case there is any (this one is tricky)
        auto _meshId = _grabString( context, geomElementPtr, "mesh", "" );
        if ( context.agentPtr->meshAssetsMap.find( _meshId ) != context.agentPtr->meshAssetsMap.end() )
        {
            // if the mesh is is a "pure link" to a filename, then use the stored asset in map
            _kinTreeCollisionPtr->geometry.meshId      = context.agentPtr->meshAssetsMap[ _meshId ]->name;
            _kinTreeCollisionPtr->geometry.filename    = context.agentPtr->meshAssetsMap[ _meshId ]->file;
        }
        else
        {
            // if not, then is a pure path to the mesh file
            _kinTreeCollisionPtr->geometry.meshId      = "";
            _kinTreeCollisionPtr->geometry.filename    = _meshId;
        }
        // and the collision/geom size (and check if uses fromto, so we can extract the relative transform)
        TVec3 _posFromFromto;
        TMat3 _rotFromFromto;
        bool _usesFromto = _extractStandardSize( context, 
                                                 geomElementPtr, 
                                                 _kinTreeCollisionPtr->geometry.size,
                                                 _posFromFromto,
                                                 _rotFromFromto );
        _kinTreeCollisionPtr->geometry.usesFromto = _usesFromto;
        if ( _usesFromto )
        {
            _kinTreeCollisionPtr->relTransform.setPosition( _posFromFromto );
            _kinTreeCollisionPtr->relTransform.setRotation( _rotFromFromto );
        }

        // and the contype collision bitmask
        _kinTreeCollisionPtr->contype = _grabInt( context, geomElementPtr, "contype", 1 );
        // and the conaffinity collision bitmask
        _kinTreeCollisionPtr->conaffinity = _grabInt( context, geomElementPtr, "conaffinity", 1 );
        // and the condim contact dimensionality
        _kinTreeCollisionPtr->condim = _grabInt( context, geomElementPtr, "condim", 3 );
        // and the friction coefficients
        _kinTreeCollisionPtr->friction = _grabArrayFloat( context, geomElementPtr, "friction", { 3, { 1., 0.005, 0.0001 } } );
        // and the density of the geometries (for default mass calculation)
        _kinTreeCollisionPtr->density = _grabFloat( context, geomElementPtr, "density", TYSOC_DEFAULT_DENSITY );
        // and store it in the collisions buffer
        context.agentPtr->collisions.push_back( _kinTreeCollisionPtr );
        // and to the collisions map
        context.agentPtr->collisionsMap[ _kinTreeCollisionPtr->name ] = _kinTreeCollisionPtr;

        return _kinTreeCollisionPtr;
    }

    TKinTreeInertia* _processInertialFromMjcf( TMjcfParsingContext& context, 
                                               mjcf::GenericElement* inertialElmPtr )
    {
        // For this element, check this documentation from mujoco
        // url: http://mujoco.org/book/XMLreference.html#inertial
        auto _kinTreeInertia = new TKinTreeInertia();
        // grab the transform of the inertial frame:
        // from mujoco docs: origin in body's CM and axes along Principal Axes of inertia
        // no transformation is required if axes are not aligned, it's going to be done by ...
        // the compiler when the body is processed by computing the eigenvalues and (perhaps) doing ...
        // a similar trick to the one done in the bullet examples (in some urdf parsing they do this trick)
        _extractTransform( context, inertialElmPtr, _kinTreeInertia->relTransform );
        // grab the mass as well (required)
        _kinTreeInertia->mass = inertialElmPtr->getAttributeFloat( "mass", 1.0 );
        // and also the inertia matrix
        if ( inertialElmPtr->hasAttributeVec3( "diaginertia" ) )
        {
            // we have been given a diagonal inertia matrix
            auto _inertiaVec3 = inertialElmPtr->getAttributeVec3( "diaginertia", { 0.1, 0.1, 0.1 } );
            // and assign the inertia matrix
            _kinTreeInertia->ixx = _inertiaVec3.x;
            _kinTreeInertia->iyy = _inertiaVec3.y;
            _kinTreeInertia->izz = _inertiaVec3.z;
            _kinTreeInertia->ixy = 0.0;
            _kinTreeInertia->ixz = 0.0;
            _kinTreeInertia->iyz = 0.0;
        }
        else if ( inertialElmPtr->hasAttributeArrayFloat( "fullinertia" ) )
        {
            // we have been given a full positive-definite symmetric inertia matrix
            auto _inertiaArray6 = inertialElmPtr->getAttributeArrayFloat( "fullinertia" );
            // and assign the inertia matrix
            _kinTreeInertia->ixx = _inertiaArray6.buff[0];
            _kinTreeInertia->iyy = _inertiaArray6.buff[1];
            _kinTreeInertia->izz = _inertiaArray6.buff[2];
            _kinTreeInertia->ixy = _inertiaArray6.buff[3];
            _kinTreeInertia->ixz = _inertiaArray6.buff[4];
            _kinTreeInertia->iyz = _inertiaArray6.buff[5];
        }
        else
        {
            // Must have one inertia matrix definition given by one of the previous elements
            std::cout << "ERROR> no inertia-matrix given in mjcf model "
                      << "for agent with name: " << context.agentPtr->name() << std::endl;
            return NULL;
        }

        return _kinTreeInertia;
    }

    TKinTreeActuator* _processActuator( TMjcfParsingContext& context, 
                                        mjcf::GenericElement* actuatorElementPtr )
    {
        auto _kinTreeActuatorPtr = new TKinTreeActuator();
        // grab the name
        _kinTreeActuatorPtr->name = actuatorElementPtr->getAttributeString( "name" );
        // check in case the user forgot to set the name of the actuator
        if ( _kinTreeActuatorPtr->name == "" )
            _kinTreeActuatorPtr->name = actuatorElementPtr->getAttributeString( "joint" );
        // and the type of actuator
        _kinTreeActuatorPtr->type = actuatorElementPtr->etype;
        // and a reference to the joint it controls
        auto _jointName = actuatorElementPtr->getAttributeString( "joint" );
        if ( context.agentPtr->jointsMap.find( _jointName ) != context.agentPtr->jointsMap.end() )
        {
            _kinTreeActuatorPtr->jointPtr = context.agentPtr->jointsMap[ _jointName ];
        }
        else
        {
            std::cout << "WARNING> actuator with name: "
                      << _kinTreeActuatorPtr->name << " "
                      << "using a non existent joint: "
                      << _jointName << std::endl;

            _kinTreeActuatorPtr->jointPtr = NULL;
        }
        // and the ctrl limits
        auto _ctrlLimits = _grabVec2( context, actuatorElementPtr, "ctrlrange", { -1, 1 } );
        _kinTreeActuatorPtr->minCtrl = _ctrlLimits.x;
        _kinTreeActuatorPtr->maxCtrl = _ctrlLimits.y;

        // @CHECK|@WIP : Should use variant here for extra parameters for ...
        // each backend type used, as some would support more/less features

        // and the ctrl clamping flag
        _kinTreeActuatorPtr->clampCtrl = ( _grabString( context, actuatorElementPtr, "ctrllimited", "true" ) == "true" );
        // and the position feedback gain
        _kinTreeActuatorPtr->kp = _grabFloat( context, actuatorElementPtr, "kp", 1.0 );
        // and the velocity feedback gain
        _kinTreeActuatorPtr->kv = _grabFloat( context, actuatorElementPtr, "kv", 1.0 );
        // and the gear scaling
        _kinTreeActuatorPtr->gear = _grabArrayFloat( context, actuatorElementPtr, "gear", { 6, { 1., 0., 0., 0., 0., 0. } } );

        // @TODO|CHECK: for some special joints (ball and free) "joint" is not the field we are looking for

        // and store it in the actuators buffer
        context.agentPtr->actuators.push_back( _kinTreeActuatorPtr );
        // and to the actuators map
        context.agentPtr->actuatorsMap[ _kinTreeActuatorPtr->name ] = _kinTreeActuatorPtr;

        return _kinTreeActuatorPtr;
    }

    void _extractTransform( TMjcfParsingContext& context, 
                            mjcf::GenericElement* elementPtr,
                            TMat4& targetTransform )
    {
        // grab local position from element
        auto _relPosition   = _grabVec3( context, elementPtr, "pos", { 0.0, 0.0, 0.0 } );
        // and make the vector to be used
        TVec3 _rPosition    = { _relPosition.x, _relPosition.y, _relPosition.z };
        // and set it to the target transform
        targetTransform.setPosition( _rPosition );

        // check if we have euler
        if ( elementPtr->hasAttributeVec3( "euler" ) ||
             _hasDefaultAttrib( context, elementPtr, "euler" ) )
        {
            // extract rotation using euler
            auto _relEuler = _grabVec3( context, elementPtr, "euler", { 0.0, 0.0, 0.0 } );

            // and convert it to radiangs in case using degrees
            if ( context.useDegrees )
            {
                _relEuler.x = degrees2rad( _relEuler.x );
                _relEuler.y = degrees2rad( _relEuler.y );
                _relEuler.z = degrees2rad( _relEuler.z );
            }

            // and convert to matrix type
            TMat3 _rRotation = TMat3::fromEuler( _relEuler );
            // and set it to the target transform
            targetTransform.setRotation( _rRotation );
        }
        else if ( elementPtr->hasAttributeVec4( "quat" ) ||
                  _hasDefaultAttrib( context, elementPtr, "quat" ) )
        {
            // extract rotation using quaternions
            auto _relQuaternion = _grabVec4( context, elementPtr, "quat", { 1.0, 0.0, 0.0, 0.0 } );
            // and extract rotation (quaternion convention in mjcf is "wxyz", our is "xyzw", so compensate for this)
            TVec4 _rQuaternion  = { _relQuaternion.y, _relQuaternion.z, _relQuaternion.w, _relQuaternion.x };
            // and convert to matrix type
            TMat3 _rRotation = TMat3::fromQuaternion( _rQuaternion );
            // and set it to the target transform
            targetTransform.setRotation( _rRotation );
        }
        else if ( elementPtr->hasAttributeVec3( "zaxis" ) ||
                  _hasDefaultAttrib( context, elementPtr, "zaxis" ) )
        {
            // extract rotation using shortest quaternion to get to the given axis
            // grab the local zaxis
            auto _zAxisLocal = _grabVec3( context, elementPtr, "zaxis", { 0.0, 0.0, 1.0 } );
            // define target zaxis
            auto _zAxisWorld = TVec3( 0, 0, 1 );
            // compute minimum rotation quat
            TVec4 _rQuaternion = shortestArcQuat( _zAxisWorld, _zAxisLocal );
            // transform it to a matrix type
            TMat3 _rRotation = TMat3::fromQuaternion( _rQuaternion );
            // and set it to the target transform
            targetTransform.setRotation( _rRotation );
        }

        // @TODO: add support for axisangle, and other remaining frame orientation representations

        // by default, the rotation matrix is set to identity, so no extra info needed
    }

    bool _extractStandardSize( TMjcfParsingContext& context, 
                               mjcf::GenericElement* geomElm,
                               TVec3& targetSize,
                               TVec3& posFromFromto,
                               TMat3& rotFromFromto )
    {
        auto _gname     = geomElm->getAttributeString( "name" );
        auto _gtype     = _grabString( context, geomElm, "type", "sphere" );
        auto _gsize     = _grabArrayFloat( context, geomElm, "size", { 1, { 0.01 } } );
        auto _gfromto   = _grabArrayFloat( context, geomElm, "fromto", { 0, { 0. } } );

        bool _usesFromto = false;

        if ( _gtype == "plane" )
        {
            // should normalize sizes accordingly
            float _width, _depth;
            if ( _gsize.ndim == 0 )
            {
                // This is weird, but just in case make some default dimenions
                std::cout << "WARNING> the plane " <<  _gname << " has no size :/" << std::endl;
                _width = 3.0f;
                _depth = 3.0f;
            }
            else if ( _gsize.ndim == 1 )
            {
                // the dimensions should be repeated
                _width = _depth = _gsize.buff[0];
            }
            else if ( _gsize.ndim >= 2 )
            {
                // each dimensiones has a separate field
                _width = _gsize.buff[0];
                _depth = _gsize.buff[1];
                // third is spacing
            }

            targetSize.x = _width;
            targetSize.y = _depth;
        }
        else if ( _gtype == "sphere" )
        {
            float _radius;

            if ( _gsize.ndim == 0 )
            {
                // This is weird, but just in case make some default dimenions
                std::cout << "WARNING> the sphere " << _gname  << " has no size :/" << std::endl;
                _radius = 0.1f;
            }
            else if ( _gsize.ndim == 1 )
            {
                _radius = _gsize.buff[0];
            }
            else
            {
                // just in case, if someone passed more parameters than needed
                // it's like ... "thanks, but no thanks xD", so just use the first two
                std::cout << "WARNING> the sphere "<< _gname << " has more parameters than needed" << std::endl;
                _radius = _gsize.buff[0];
            }

            targetSize.x = _radius;
        }
        else if ( _gtype == "capsule" ||
                  _gtype == "cylinder" )
        {
            float _radius, _length;

            // This one is a bit trick, because it might have a fromto
            if ( _gfromto.ndim == 6 ) // use fromto
            {
                /*   _____________________
                *   |                     |
                *  |  s                 e  |
                *   |_____________________|
                */
                // first 3 are start point (s)
                float _x1 = _gfromto.buff[0];
                float _y1 = _gfromto.buff[1];
                float _z1 = _gfromto.buff[2];
                // second 3 are the end point (e)
                float _x2 = _gfromto.buff[3];
                float _y2 = _gfromto.buff[4];
                float _z2 = _gfromto.buff[5];
                // get the length of the capsule (the space orientation is ...
                // computed from the scene, so we just use that one)
                float _dx = ( _x2 - _x1 );
                float _dy = ( _y2 - _y1 );
                float _dz = ( _z2 - _z1 );
                _length = sqrtf( _dx * _dx + _dy * _dy + _dz * _dz );

                // get the radius from the size param
                if ( _gsize.ndim >= 1 )
                {
                    _radius = _gsize.buff[0];
                }
                else
                {
                    std::cout << "WARNING> the capsule/cylinder " << _gname << " has wrong size dim for fromto" << std::endl;
                    _radius = 0.25f * _length;
                }

                _usesFromto = true;

                // compute local position
                posFromFromto.x = 0.5f * ( _x1 + _x2 );
                posFromFromto.y = 0.5f * ( _y1 + _y2 );
                posFromFromto.z = 0.5f * ( _z1 + _z2 );

                // compute local rotation
                TVec3 _zAxisWorld   = TVec3( 0, 0, 1 );
                TVec3 _zAxisLocal   = TVec3( _dx / _length, _dy / _length, _dz / _length );
                TVec4 _quat         = shortestArcQuat( _zAxisWorld, _zAxisLocal );
                rotFromFromto       = TMat3::fromQuaternion( _quat );
            }
            else
            {
                if ( _gsize.ndim == 2 )
                {
                    _radius = _gsize.buff[0];
                    _length = 2.0f * _gsize.buff[1];
                }
                else
                {
                    // default, just in case passed less than (radius,length)
                    std::cout << "WARNING> the capsule/cylinder " << _gname << " has wrong size dim" << std::endl;
                    _radius = 0.05f;
                    _length = 0.1f;
                }
            }

            targetSize.x = _radius;
            targetSize.y = _length;
        }
        else if ( _gtype == "box" )
        {
            float _hwidth, _hdepth, _hheight;

            if ( _gsize.ndim == 3 )
            {
                _hwidth  = _gsize.buff[0];
                _hdepth  = _gsize.buff[1];
                _hheight = _gsize.buff[2];
            }
            else
            {
                std::cout << "WARNING> the box " << _gname << " has wrong dims for creation" << std::endl;
                _hwidth = _hdepth = _hheight = 0.1f;
            }

            targetSize.x = 2 * _hwidth;
            targetSize.y = 2 * _hdepth;
            targetSize.z = 2 * _hheight;
        }

        return _usesFromto;
    }

    void _extractMjcfModelSettings( TMjcfParsingContext& context )
    {
        auto _settingsElm = mjcf::findFirstChildByType( context.modelDataPtr, "compiler" );

        if ( _settingsElm )
        {
            context.useLocalCoordinates = ( _settingsElm->getAttributeString( "coordinate", "local" ) == "local" );
            context.useDegrees = ( _settingsElm->getAttributeString( "angle", "degree" ) == "degree" );
        }
        else
        {
            context.useLocalCoordinates = true;
            context.useDegrees = true;
        }
    }

    void _convertGlobalToLocalCoordinates( TKinTreeBody* kinTreeBodyPtr, 
                                           const TMat4& parentWorldTransform )
    {
        auto _bodyWorldTransform = kinTreeBodyPtr->relTransform;
        kinTreeBodyPtr->relTransform = _convertGlobalToLocalTransform( parentWorldTransform,
                                                                       _bodyWorldTransform );

        auto _joints = kinTreeBodyPtr->childJoints;
        for ( size_t q = 0; q < _joints.size(); q++ )
        {
            _joints[q]->relTransform = _convertGlobalToLocalTransform( _bodyWorldTransform,
                                                                       _joints[q]->relTransform );
        }

        auto _visuals = kinTreeBodyPtr->childVisuals;
        for ( size_t q = 0; q < _visuals.size(); q++ )
        {
            _visuals[q]->relTransform = _convertGlobalToLocalTransform( _bodyWorldTransform,
                                                                        _visuals[q]->relTransform );
        }

        auto _collisions = kinTreeBodyPtr->childCollisions;
        for ( size_t q = 0; q < _collisions.size(); q++ )
        {
            _collisions[q]->relTransform = _convertGlobalToLocalTransform( _bodyWorldTransform,
                                                                           _collisions[q]->relTransform );
        }

        auto _bodies = kinTreeBodyPtr->childBodies;
        for ( size_t q = 0; q < _bodies.size(); q++ )
        {
            _convertGlobalToLocalCoordinates( _bodies[q], _bodyWorldTransform );
        }
    }

    TMat4 _convertGlobalToLocalTransform( const TMat4& parentWorldTransform,
                                          const TMat4& childWorldTransform )
    {
        // convert coordinates from body->relTransform to local coordinates
        // Use the following relation:
        //
        //  b1 : parent
        //  b2 : child
        //
        //     w       b1        w         b1       w -1     w   
        //      T   *    T    =   T    ->   T   =    T    *   T  
        //       b1       b2       b2        b2       b1       b2
        
        auto _b1ToWorld = parentWorldTransform;
        auto _b2ToWorld = childWorldTransform;
        auto _b2Tob1 = _b1ToWorld.inverse() * _b2ToWorld;

        return _b2Tob1;
    }

    std::string _grabString( TMjcfParsingContext& context,
                             mjcf::GenericElement* elementPtr,
                             const std::string& attribId,
                             const std::string& defString )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeString( attribId ) )
            return elementPtr->getAttributeString( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    if ( context.defaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return context.defaultsPerClass[_classId][_elType].getString( attribId, defString );
                }
            }
        }

        if ( context.defaultsNoClass.find( _elType ) !=
             context.defaultsNoClass.end() )
        {
            if ( context.defaultsNoClass[_elType].hasParam( attribId ) )
                return context.defaultsNoClass[_elType].getString( attribId, defString );
        }

        return defString;
    }

    float _grabFloat( TMjcfParsingContext& context,
                      mjcf::GenericElement* elementPtr,
                      const std::string& attribId,
                      const float& defFloat )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeFloat( attribId ) )
            return elementPtr->getAttributeFloat( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    if ( context.defaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return context.defaultsPerClass[_classId][_elType].getFloat( attribId, defFloat );
                }
            }
        }

        if ( context.defaultsNoClass.find( _elType ) !=
             context.defaultsNoClass.end() )
        {
            if ( context.defaultsNoClass[_elType].hasParam( attribId ) )
                return context.defaultsNoClass[_elType].getFloat( attribId, defFloat );
        }

        return defFloat;
    }

    int _grabInt( TMjcfParsingContext& context,
                  mjcf::GenericElement* elementPtr,
                  const std::string& attribId,
                  const int& defInt )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeInt( attribId ) )
            return elementPtr->getAttributeInt( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    if ( context.defaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return context.defaultsPerClass[_classId][_elType].getInt( attribId, defInt );
                }
            }
        }

        if ( context.defaultsNoClass.find( _elType ) !=
             context.defaultsNoClass.end() )
        {
            if ( context.defaultsNoClass[_elType].hasParam( attribId ) )
                return context.defaultsNoClass[_elType].getInt( attribId, defInt );
        }

        return defInt;
    }


    TVec2 _grabVec2( TMjcfParsingContext& context,
                     mjcf::GenericElement* elementPtr,
                     const std::string& attribId,
                     const TVec2& defVec2 )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeVec2( attribId ) )
            return elementPtr->getAttributeVec2( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    if ( context.defaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return context.defaultsPerClass[_classId][_elType].getVec2( attribId, defVec2 );
                }
            }
        }

        if ( context.defaultsNoClass.find( _elType ) !=
             context.defaultsNoClass.end() )
        {
            if ( context.defaultsNoClass[_elType].hasParam( attribId ) )
                return context.defaultsNoClass[_elType].getVec2( attribId, defVec2 );
        }

        return defVec2;
    }

    TVec3 _grabVec3( TMjcfParsingContext& context,
                     mjcf::GenericElement* elementPtr,
                     const std::string& attribId,
                     const TVec3& defVec3 )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeVec3( attribId ) )
            return elementPtr->getAttributeVec3( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    if ( context.defaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return context.defaultsPerClass[_classId][_elType].getVec3( attribId, defVec3 );
                }
            }
        }
        
        if ( context.defaultsNoClass.find( _elType ) !=
             context.defaultsNoClass.end() )
        {
            if ( context.defaultsNoClass[_elType].hasParam( attribId ) )
                return context.defaultsNoClass[_elType].getVec3( attribId, defVec3 );
        }

        return defVec3;
    }

    TVec4 _grabVec4( TMjcfParsingContext& context,
                     mjcf::GenericElement* elementPtr,
                     const std::string& attribId,
                     const TVec4& defVec4 )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeVec4( attribId ) )
            return elementPtr->getAttributeVec4( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    if ( context.defaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return context.defaultsPerClass[_classId][_elType].getVec4( attribId, defVec4 );
                }
            }
        }

        if ( context.defaultsNoClass.find( _elType ) !=
             context.defaultsNoClass.end() )
        {
            if ( context.defaultsNoClass[_elType].hasParam( attribId ) )
                return context.defaultsNoClass[_elType].getVec4( attribId, defVec4 );
        }

        return defVec4;
    }

    TSizef _grabArrayFloat( TMjcfParsingContext& context,
                            mjcf::GenericElement* elementPtr,
                            const std::string& attribId,
                            const TSizef& defSizef )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeArrayFloat( attribId ) )
            return elementPtr->getAttributeArrayFloat( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    if ( context.defaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return context.defaultsPerClass[_classId][_elType].getSizef( attribId, defSizef );
                }
            }
        }

        if ( context.defaultsNoClass.find( _elType ) !=
             context.defaultsNoClass.end() )
        {
            if ( context.defaultsNoClass[_elType].hasParam( attribId ) )
                return context.defaultsNoClass[_elType].getSizef( attribId, defSizef );
        }

        return defSizef;
    }

    TSizei _grabArrayInt( TMjcfParsingContext& context,
                          mjcf::GenericElement* elementPtr,
                          const std::string& attribId,
                          const TSizei& defSizei )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeArrayInt( attribId ) )
            return elementPtr->getAttributeArrayInt( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    if ( context.defaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return context.defaultsPerClass[_classId][_elType].getSizei( attribId, defSizei );
                }
            }
        }
        
        if ( context.defaultsNoClass.find( _elType ) !=
             context.defaultsNoClass.end() )
        {
            if ( context.defaultsNoClass[_elType].hasParam( attribId ) )
                return context.defaultsNoClass[_elType].getSizei( attribId, defSizei );
        }

        return defSizei;
    }

    bool _hasDefaultAttrib( TMjcfParsingContext& context,
                            mjcf::GenericElement* elementPtr,
                            const std::string& attribId )
    {
        bool _hasDefault = false;
        auto _elmType = elementPtr->etype;

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( context.defaultsPerClass.find( _classId ) !=
                 context.defaultsPerClass.end() )
            {
                if ( context.defaultsPerClass[_classId].find( _elmType ) !=
                     context.defaultsPerClass[_classId].end() )
                {
                    _hasDefault = context.defaultsPerClass[_classId][_elmType].hasParam( attribId );
                }
            }
        }
        else if ( context.defaultsNoClass.find( _elmType ) !=
                  context.defaultsNoClass.end() )
        {
            _hasDefault = context.defaultsNoClass[_elmType].hasParam( attribId );
        }

        return _hasDefault;
    }
}}