
#include <agent/formats/kintree_format_mjcf.h>

namespace tysoc {

    void constructAgentFromModel( TAgent* agentPtr,
                                  mjcf::GenericElement* modelDataPtr )
    {
        // Create and initialize the context
        TMjcfParsingContext _context;
        _context.agentPtr = agentPtr;
        _context.modelDataPtr = new mjcf::GenericElement();
        _context.filepath = modelDataPtr->filepath;
        _context.folderpath = modelDataPtr->folderpath;

        // create a copy of the model data with the names modified appropriately
        mjcf::deepCopy( _context.modelDataPtr, modelDataPtr, nullptr, agentPtr->name() );

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
        auto _rootBodyPtr = _processBodyFromMjcf( _context, _rootBodyElmPtr, nullptr );

        if ( !_rootBodyPtr )
        {
            std::cout << "ERROR> something went wrong while parsing agent: "
                      << agentPtr->name() << ". Processed root body is nullptr" << std::endl;
        }

        // make sure we set the root for this agent we are constructing
        _context.agentPtr->setRootBody( _rootBodyPtr );

        // grab the actuators. Sensors are just copied, and the joint sensors are created as needed
        auto _actuatorsElmPtr = mjcf::findFirstChildByType( _context.modelDataPtr, 
                                                            "actuator" );

        // create the actual actuator
        if ( _actuatorsElmPtr )
            for ( auto _actuatorElm : _actuatorsElmPtr->children )
                _processActuator( _context, _actuatorElm );

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

        for ( auto _defaultElm : _defaultsElmPtr->children )
        {
            if ( _defaultElm->etype == "default" )
                _collectDefaultsFromClass( context, _defaultElm );
            else
                _collectDefaultsNoClass( context, _defaultElm );
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

        for ( auto _child : defClassElmPtr->children )
        {
            auto _possibleAttribsDict = mjcf::MJCF_SCHEMA->getPossibleAttribs( _child->etype );

            for ( auto _pair : _possibleAttribsDict )
            {
                if ( _pair.second == mjcf::TYPE_ARRAY_FLOAT &&
                     _child->hasAttributeArrayFloat( _pair.first ) )
                {
                    context.defaultsPerClass[_classId][_child->etype].set( _pair.first,
                                                                  _child->getAttributeArrayFloat( _pair.first ) );
                }
                else if ( _pair.second == mjcf::TYPE_ARRAY_INT &&
                          _child->hasAttributeArrayInt( _pair.first ) )
                {
                    context.defaultsPerClass[_classId][_child->etype].set( _pair.first,
                                                                  _child->getAttributeArrayInt( _pair.first ) );
                }
                else if ( _pair.second == mjcf::TYPE_FLOAT &&
                          _child->hasAttributeFloat( _pair.first ) )
                {
                    context.defaultsPerClass[_classId][_child->etype].set( _pair.first,
                                                                  _child->getAttributeFloat( _pair.first ) );
                }
                else if ( _pair.second == mjcf::TYPE_INT &&
                          _child->hasAttributeInt( _pair.first ) )
                {
                    context.defaultsPerClass[_classId][_child->etype].set( _pair.first,
                                                                  _child->getAttributeInt( _pair.first ) );
                }
                else if ( _pair.second == mjcf::TYPE_STRING &&
                          _child->hasAttributeString( _pair.first ) )
                {
                    context.defaultsPerClass[_classId][_child->etype].set( _pair.first,
                                                                  _child->getAttributeString( _pair.first ) );
                }
            }
        }
    }

    void _collectDefaultsNoClass( TMjcfParsingContext& context, 
                                  mjcf::GenericElement* defElmPtr )
    {
        auto _tagId = defElmPtr->etype;
        auto _possibleAttribsDict = mjcf::MJCF_SCHEMA->getPossibleAttribs( _tagId );

        for ( auto _pair : _possibleAttribsDict )
        {
            if ( _pair.second == mjcf::TYPE_ARRAY_FLOAT &&
                 defElmPtr->hasAttributeArrayFloat( _pair.first ) )
            {
                context.defaultsNoClass[_tagId].set( _pair.first,
                                                   defElmPtr->getAttributeArrayFloat( _pair.first ) );
            }
            else if ( _pair.second == mjcf::TYPE_ARRAY_INT &&
                      defElmPtr->hasAttributeArrayInt( _pair.first ) )
            {
                context.defaultsNoClass[_tagId].set( _pair.first,
                                                   defElmPtr->getAttributeArrayInt( _pair.first ) );
            }
            else if ( _pair.second == mjcf::TYPE_FLOAT &&
                      defElmPtr->hasAttributeFloat( _pair.first ) )
            {
                context.defaultsNoClass[_tagId].set( _pair.first,
                                                   defElmPtr->getAttributeFloat( _pair.first ) );
            }
            else if ( _pair.second == mjcf::TYPE_INT &&
                      defElmPtr->hasAttributeInt( _pair.first ) )
            {
                context.defaultsNoClass[_tagId].set( _pair.first,
                                                   defElmPtr->getAttributeInt( _pair.first ) );
            }
            else if ( _pair.second == mjcf::TYPE_STRING &&
                      defElmPtr->hasAttributeString( _pair.first ) )
            {
                context.defaultsNoClass[_tagId].set( _pair.first,
                                                   defElmPtr->getAttributeString( _pair.first ) );
            }
        }
    }

    void _collectAssets( TMjcfParsingContext& context )
    {
        auto _assetsElmPtr = mjcf::findFirstChildByType( context.modelDataPtr, "asset" );
        if ( !_assetsElmPtr )
            return;// Nothing to do if no assets element found

        for ( auto _asset : _assetsElmPtr->children )
        {
            if ( _asset->etype == "mesh" )
            {
                // Grab the mesh asset into a dict for later usage *************
                auto _meshAsset = TGenericParams();
                if ( _asset->hasAttributeString( "file" ) )
                    _meshAsset.set( "file", _asset->getAttributeString( "file", "" ) );
                else
                    std::cout << "WARNING> mesh asset doesn't have a valid linked file" << std::endl;

                if ( _asset->hasAttributeString( "name" ) )
                    _meshAsset.set( "name", _asset->getAttributeString( "name", "" ) );
                else
                    _meshAsset.set( "name", tysoc::getFilenameNoExtensionFromFilePath( _meshAsset.getString( "file", "" ) ) );

                _meshAsset.set( "scale", _asset->getAttributeVec3( "scale", { 1.0f, 1.0f, 1.0f } ) );

                context.assetsMeshes[_meshAsset.getString( "name", "" )] = _meshAsset;
                // *************************************************************
            }
            else if ( _asset->etype == "material" && _asset->hasAttributeString( "name" ) )
            {
                auto _materialAsset = TGenericParams();
                if ( _asset->hasAttributeVec4( "rgba" ) )
                    _materialAsset.set( "rgba", _asset->getAttributeVec4( "rgba", TYSOC_DEFAULT_RGBA_COLOR ) );
                if ( _asset->hasAttributeFloat( "shininess" ) )
                    _materialAsset.set( "shininess", _asset->getAttributeFloat( "shininess", TYSOC_DEFAULT_SHININESS / 128.0f ) * 128.0f );
                if ( _asset->hasAttributeFloat( "specular" ) )
                    _materialAsset.set( "specular", _asset->getAttributeFloat( "specular", 1.0f ) );

                context.assetsMaterials[_asset->getAttributeString( "name", "" )] = _materialAsset;
            }
            else
            {
                // Other asset types are not supported yet
                std::cout << "WARNING> asset of type (" << _asset->etype 
                          << ") isn't supported yet" << std::endl;
            }
        }
    }

    TKinTreeBody* _processBodyFromMjcf( TMjcfParsingContext& context, 
                                        mjcf::GenericElement* bodyElementPtr,
                                        TKinTreeBody* parentKinBody )
    {
        // grab body information
        auto _kinBody = new TKinTreeBody();
        // grab the name
        _kinBody->name = bodyElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body (zero-configuration | rest-configuration)
        _extractTransform( context, bodyElementPtr, _kinBody->localTransformZero );
        // and the parent bodyptr as well
        _kinBody->parentBodyPtr = parentKinBody;
        // and store it in the bodies buffer
        context.agentPtr->bodies.push_back( _kinBody );
        // and to the bodies map
        context.agentPtr->bodiesMap[_kinBody->name] = _kinBody;

        // grab the child-class (if given)
        auto _childClass = bodyElementPtr->getAttributeString( "childclass", "" );

        // grab all geom elements and process them as visual and collisions
        auto _geomElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "geom" );
        for ( auto _geomElement : _geomElementPtrs )
        {
            // set child-class if geom element does not have any
            if ( _childClass != "" && !_geomElement->hasAttributeString( "class" ) )
                _geomElement->setAttributeString( "class", _childClass );

            auto _kinVisual     = _processVisualFromMjcf( context, _geomElement );
            _kinVisual->parentBodyPtr    = _kinBody;
            _kinBody->visuals.push_back( _kinVisual );

            auto _kinCollision  = _processCollisionFromMjcf( context, _geomElement );
            _kinCollision->parentBodyPtr = _kinBody;
            _kinBody->collisions.push_back( _kinCollision );
        }

        // grab all joint elements and process them
        auto _jointElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "joint" );
        for ( auto _jointElement : _jointElementPtrs )
        {
            // set child-class if joint element does not have any
            if ( _childClass != "" && !_jointElement->hasAttributeString( "class" ) )
                _jointElement->setAttributeString( "class", _childClass );

            auto _kinTreeJointPtr =  _processJointFromMjcf( context, _jointElement );
            _kinTreeJointPtr->parentBodyPtr = _kinBody;
            _kinBody->joints.push_back( _kinTreeJointPtr );
        }

        // grab the inertial properties (if given, as by default is computed from the geometries)
        auto _inertialElmPtr = mjcf::findFirstChildByType( bodyElementPtr, "inertial" );
        if ( _inertialElmPtr )
            _kinBody->inertialData = _processInertialFromMjcf( context, _inertialElmPtr );

        // @todo: Add support for sites, as some models may need them
        // ...

        // grab child bodies (if any) and repeat process
        auto _childrenElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "body" );
        for ( auto _childBody : _childrenElementPtrs )
        {
            // set child-class if joint element does not have any
            if ( _childClass != "" && !_childBody->hasAttributeString( "childclass" ) )
                _childBody->setAttributeString( "childclass", _childClass );

            _kinBody->children.push_back( _processBodyFromMjcf( context, _childBody, _kinBody ) );
        }

        return _kinBody;
    }

    TKinTreeJoint* _processJointFromMjcf( TMjcfParsingContext& context, 
                                          mjcf::GenericElement* jointElementPtr )
    {
        auto _kinTreeJointType = toEnumJoint( _grabString( context, jointElementPtr, "type", "hinge" ) );
        auto _kinTreeJointPtr = new TKinTreeJoint( _kinTreeJointType );

        // grab the name
        _kinTreeJointPtr->name = jointElementPtr->getAttributeString( "name" );
        // and the relative transform to the owner body
        _extractTransform( context, jointElementPtr, _kinTreeJointPtr->data.localTransform );
        // and the joint axis
        _kinTreeJointPtr->data.axis = _grabVec3( context, jointElementPtr, "axis", { 0., 1., 0. } );
        // and the joint value clamping flag
        bool _limited = ( _grabString( context, jointElementPtr, "limited", "false" ) == "true" );
        _limited |= jointElementPtr->hasAttributeVec2( "range" );
        if ( _limited )
        {
            if ( context.useDegrees )
            {
                auto _limits = _grabVec2( context, jointElementPtr, "range", { -180., 180. } );
                _kinTreeJointPtr->data.limits = { degrees2rad( _limits.x ), degrees2rad( _limits.y ) };
            }
            else
            {
                auto _limits = _grabVec2( context, jointElementPtr, "range", { -TYSOC_PI, TYSOC_PI } );
                _kinTreeJointPtr->data.limits = { _limits.x, _limits.y };
            }
        }
        else
        {
            // lower > higher means not limited
            _kinTreeJointPtr->data.limits = { 1.0f, -1.0f };
        }
        // and the joint stiffness
        _kinTreeJointPtr->data.stiffness = _grabFloat( context, jointElementPtr, "stiffness", 0.0 );
        // and the joint armature
        _kinTreeJointPtr->data.armature = _grabFloat( context, jointElementPtr, "armature", 0.0 );
        // and the joint damping
        _kinTreeJointPtr->data.damping = _grabFloat( context, jointElementPtr, "damping", 0.0 );
        // and the joint ref
        _kinTreeJointPtr->data.ref = _grabFloat( context, jointElementPtr, "ref", 0.0 );
        // and store it in the joints buffer
        context.agentPtr->joints.push_back( _kinTreeJointPtr );
        // and to the joints map
        context.agentPtr->jointsMap[ _kinTreeJointPtr->name ] = _kinTreeJointPtr;

        return _kinTreeJointPtr;
    }

    TKinTreeVisual* _processVisualFromMjcf( TMjcfParsingContext& context, 
                                            mjcf::GenericElement* geomElementPtr )
    {
        auto _kinVisual = new TKinTreeVisual();
        // grab the name
        _kinVisual->name = geomElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( context, geomElementPtr, _kinVisual->data.localTransform );
        // and the type of visual/geom
        _kinVisual->data.type = toEnumShape( _grabString( context, geomElementPtr, "type", "" ) );
        // and the mesh filename in case there is any
        _kinVisual->data.filename = _extractFilename( context, _grabString( context, geomElementPtr, "mesh", "" ) );
        // and the visual/geom size (and check if uses fromto, so we can extract the relative transform)
        TVec3 _posFromFromto; TMat3 _rotFromFromto;
        bool _usesFromto = _extractStandardSize( context, 
                                                 geomElementPtr, 
                                                 _kinVisual->data.size,
                                                 _posFromFromto,
                                                 _rotFromFromto );
        if ( _usesFromto )
        {
            _kinVisual->data.localTransform.setPosition( _posFromFromto );
            _kinVisual->data.localTransform.setRotation( _rotFromFromto );
        }

        auto _rgba = TVec4( TYSOC_DEFAULT_RGBA_COLOR );
        auto _specular = TVec3( TYSOC_DEFAULT_SPECULAR_COLOR );
        float _shininess = TYSOC_DEFAULT_SHININESS;
        auto _materialId = _grabString( context, geomElementPtr, "material", "" );
        if ( !geomElementPtr->hasAttributeVec4( "rgba" ) && ( _materialId != "" ) &&
             ( context.assetsMaterials.find( _materialId ) != context.assetsMaterials.end() ) )
        {
            if ( context.assetsMaterials[_materialId].hasParam( "rgba" ) )
                _rgba = context.assetsMaterials[_materialId].getVec4( "rgba" );

            if ( context.assetsMaterials[_materialId].hasParam( "shininess" ) )
                _shininess = context.assetsMaterials[_materialId].getFloat( "shininess" );

            if ( context.assetsMaterials[_materialId].hasParam( "specular" ) )
            _specular = { context.assetsMaterials[_materialId].getFloat( "specular" ),
                          context.assetsMaterials[_materialId].getFloat( "specular" ),
                          context.assetsMaterials[_materialId].getFloat( "specular" ) };
        }
        else
        {
            _rgba = _grabVec4( context, geomElementPtr, "rgba", TYSOC_DEFAULT_RGBA_COLOR );
            _specular = { _rgba.x, _rgba.y, _rgba.z };
            
        }
        _kinVisual->data.ambient = { _rgba.x, _rgba.y, _rgba.z };
        _kinVisual->data.diffuse = { _rgba.x, _rgba.y, _rgba.z };
        _kinVisual->data.specular = _specular;
        _kinVisual->data.shininess = _shininess;
        _kinVisual->data.usesMaterialFromMesh = false; // xml file provides color information

        // and store it in the visuals buffer
        context.agentPtr->visuals.push_back( _kinVisual );
        // and to the visuals map
        context.agentPtr->visualsMap[_kinVisual->name] = _kinVisual;

        return _kinVisual;
    }

    TKinTreeCollision* _processCollisionFromMjcf( TMjcfParsingContext& context, 
                                                  mjcf::GenericElement* geomElementPtr )
    {
        auto _kinCollision = new TKinTreeCollision();
        // grab the name
        _kinCollision->name = geomElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( context, geomElementPtr, _kinCollision->data.localTransform );
        // and the collision/geom
        _kinCollision->data.type = toEnumShape( _grabString( context, geomElementPtr, "type", "" ) );
        // and the mesh filename in case there is any (this one is tricky)
        _kinCollision->data.filename = _extractFilename( context, _grabString( context, geomElementPtr, "mesh", "" ) );
        // and the collision/geom size (and check if uses fromto, so we can extract the relative transform)
        TVec3 _posFromFromto;
        TMat3 _rotFromFromto;
        bool _usesFromto = _extractStandardSize( context, 
                                                 geomElementPtr, 
                                                 _kinCollision->data.size,
                                                 _posFromFromto,
                                                 _rotFromFromto );
        if ( _usesFromto )
        {
            _kinCollision->data.localTransform.setPosition( _posFromFromto );
            _kinCollision->data.localTransform.setRotation( _rotFromFromto );
        }

        // and the contype collision bitmask
        _kinCollision->data.collisionGroup = _grabInt( context, geomElementPtr, "contype", 1 );
        // and the conaffinity collision bitmask
        _kinCollision->data.collisionMask = _grabInt( context, geomElementPtr, "conaffinity", 1 );
        // and the friction coefficients
        _kinCollision->data.friction = _grabVec3( context, geomElementPtr, "friction", { 1., 0.005, 0.0001 } );
        // and the density of the geometries (for default mass calculation)
        _kinCollision->data.density = _grabFloat( context, geomElementPtr, "density", TYSOC_DEFAULT_DENSITY );
        // and store it in the collisions buffer
        context.agentPtr->collisions.push_back( _kinCollision );
        // and to the collisions map
        context.agentPtr->collisionsMap[_kinCollision->name] = _kinCollision;

        return _kinCollision;
    }

    TInertialData _processInertialFromMjcf( TMjcfParsingContext& context, 
                                            mjcf::GenericElement* inertialElmPtr )
    {
        // For this element, check this documentation from mujoco
        // url: http://mujoco.org/book/XMLreference.html#inertial
        auto _kinInertial = TInertialData();
        // Grab the transform of the inertial frame
        // from mujoco docs: origin in body's CM and axes along Principal Axes of inertia
        // no transformation is required if axes are not aligned, it's going to be done by ...
        // the compiler when the body is processed by computing the eigenvalues and (perhaps) doing ...
        // a similar trick to the one done in the bullet examples (in some urdf parsing they do this trick)
        _extractTransform( context, inertialElmPtr, _kinInertial.localTransform );
        // grab the mass as well (required)
        _kinInertial.mass = inertialElmPtr->getAttributeFloat( "mass", 1.0 );
        // and also the inertia matrix
        if ( inertialElmPtr->hasAttributeVec3( "diaginertia" ) )
        {
            // we have been given a diagonal inertia matrix
            auto _inertiaVec3 = inertialElmPtr->getAttributeVec3( "diaginertia", { 0.1, 0.1, 0.1 } );
            // and assign the inertia matrix
            _kinInertial.ixx = _inertiaVec3.x;
            _kinInertial.iyy = _inertiaVec3.y;
            _kinInertial.izz = _inertiaVec3.z;
            _kinInertial.ixy = 0.0;
            _kinInertial.ixz = 0.0;
            _kinInertial.iyz = 0.0;
        }
        else if ( inertialElmPtr->hasAttributeArrayFloat( "fullinertia" ) )
        {
            // we have been given a full positive-definite symmetric inertia matrix
            auto _inertiaArray6 = inertialElmPtr->getAttributeArrayFloat( "fullinertia" );
            // and assign the inertia matrix
            _kinInertial.ixx = _inertiaArray6.buff[0];
            _kinInertial.iyy = _inertiaArray6.buff[1];
            _kinInertial.izz = _inertiaArray6.buff[2];
            _kinInertial.ixy = _inertiaArray6.buff[3];
            _kinInertial.ixz = _inertiaArray6.buff[4];
            _kinInertial.iyz = _inertiaArray6.buff[5];
        }
        else
        {
            // Must have one inertia matrix definition given by one of the previous elements
            std::cout << "ERROR> no inertia-matrix given in mjcf model "
                      << "for agent with name: " << context.agentPtr->name() << std::endl;
        }

        return _kinInertial;
    }

    TKinTreeActuator* _processActuator( TMjcfParsingContext& context, 
                                        mjcf::GenericElement* actuatorElementPtr )
    {
        auto _kinActuator = new TKinTreeActuator();
        // grab the name
        _kinActuator->name = actuatorElementPtr->getAttributeString( "name" );
        // check in case the user forgot to set the name of the actuator (use joint name as default)
        if ( _kinActuator->name == "" )
            _kinActuator->name = actuatorElementPtr->getAttributeString( "joint" );
        // and the type of actuator
        _kinActuator->data.type = toEnumActuator( actuatorElementPtr->etype );
        // and a reference to the joint it controls (@todo: add support for other 3 types of transmissions)
        auto _jointName = actuatorElementPtr->getAttributeString( "joint" );
        if ( context.agentPtr->jointsMap.find( _jointName ) != context.agentPtr->jointsMap.end() )
        {
            _kinActuator->jointPtr = context.agentPtr->jointsMap[_jointName];
        }
        else
        {
            std::cout << "WARNING> actuator with name: " << _kinActuator->name << " "
                      << "using a non existent joint: " << _jointName << std::endl;

            _kinActuator->jointPtr = nullptr;
        }
        // and the ctrl limits (must have limited-flag set, and have a valid range). Otherwise, use defaults
        if ( _grabString( context, actuatorElementPtr, "ctrllimited", "false" ) == "true" )
            if ( actuatorElementPtr->hasAttributeVec2( "ctrlrange" ) )
                _kinActuator->data.limits = _grabVec2( context, actuatorElementPtr, "ctrlrange", { -1.0f, 1.0f } );

        // and the position feedback gain
        _kinActuator->data.kp = _grabFloat( context, actuatorElementPtr, "kp", 1.0 );
        // and the velocity feedback gain
        _kinActuator->data.kv = _grabFloat( context, actuatorElementPtr, "kv", 1.0 );
        // and the gear scaling
        _kinActuator->data.gear = _grabArrayFloat( context, actuatorElementPtr, "gear", { 6, { 1., 0., 0., 0., 0., 0. } } );

        // and store it in the actuators buffer
        context.agentPtr->actuators.push_back( _kinActuator );
        // and to the actuators map
        context.agentPtr->actuatorsMap[_kinActuator->name] = _kinActuator;

        return _kinActuator;
    }

    std::string _extractFilename( TMjcfParsingContext& context, const std::string& meshStr )
    {
        // in case no string found (no mesh resource), just return empty
        if ( meshStr == "" )
            return "";

        // if meshStr is found in the assets-map (collected during asset collection), grab the resource from the map
        if ( context.assetsMeshes.find( meshStr ) != context.assetsMeshes.end() )
            return context.folderpath + context.assetsMeshes[meshStr].getString( "file", "" );

        // in case not found, then this is the actual filepath
        return context.folderpath + meshStr;
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
                // This is weird, but just in case make some default dimensions
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
                _width = 2.0f * _gsize.buff[0];
                _depth = 2.0f * _gsize.buff[1];
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

            targetSize.x = 2.0f * _hwidth;
            targetSize.y = 2.0f * _hdepth;
            targetSize.z = 2.0f * _hheight;
        }
        else if ( _gtype == "mesh" )
        {
            auto _gmeshId = _grabString( context, geomElm, "mesh", "" );
            if ( _gmeshId != "" && ( context.assetsMeshes.find( _gmeshId ) != context.assetsMeshes.end() ) )
                targetSize = context.assetsMeshes[_gmeshId].getVec3( "scale", { 1.0f, 1.0f, 1.0f } );
            else
                targetSize = { 1.0f, 1.0f, 1.0f };
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

    void _convertGlobalToLocalCoordinates( TKinTreeBody* kinBody, 
                                           const TMat4& parentWorldTransform )
    {
        /* convert all transforms, keeping in mind that the local transforms act as world-transforms */

        auto _bodyWorldTransform = kinBody->localTransformZero;
        kinBody->localTransformZero = _convertGlobalToLocalTransform( parentWorldTransform,
                                                                      _bodyWorldTransform );

        for ( auto _joint : kinBody->joints )
            _joint->data.localTransform = _convertGlobalToLocalTransform( _bodyWorldTransform, _joint->data.localTransform );

        for ( auto _visual : kinBody->visuals )
            _visual->data.localTransform = _convertGlobalToLocalTransform( _bodyWorldTransform, _visual->data.localTransform );

        for ( auto _collision : kinBody->collisions )
            _collision->data.localTransform = _convertGlobalToLocalTransform( _bodyWorldTransform, _collision->data.localTransform );

        for ( auto _childBody : kinBody->children )
            _convertGlobalToLocalCoordinates( _childBody, _bodyWorldTransform );
    }

    TMat4 _convertGlobalToLocalTransform( const TMat4& parentWorldTransform,
                                          const TMat4& childWorldTransform )
    {
        // Compute the relative transform between two frames, given their world transforms
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
}