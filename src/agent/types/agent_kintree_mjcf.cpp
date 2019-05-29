
#include <agent/types/agent_kintree_mjcf.h>

namespace tysoc {
namespace agent {

    // @TODO: Add a more comprehensive explanation of the flow of the resources ...
    // to create a kintree from various formats. I still kind of have to check ...
    // again the implementation to see the flow of the code

    /*
    *   Backend-specific stuff: the kintree tries to have as much data as necessary to ...
    *                           instantiate a simulation in all supported backends. Still ...
    *                           in some cases it's necessary to copy some specific resources ...
    *                           and make a kind of specific checks to create backend-specific ...
    *                           resources in the appropriate wrapper. For example, check ...
    *                           the method _createMjcResourcesFromKinTree in the ...
    *                           mujoco_agent_wrapper.cpp file. It grabs some very specific information ...
    *                           of the model being used.
    *
    *   Flow of resources: the basic flow of resources is the following :
    *                        > Grab all information from the parsed model (in any format) ...
    *                          into the kintree structures. This usually start in ...
    *                          "_constructKinTree" method. We also do some backend-specific ...
    *                          modifications towards the instantiation on a specific backend, ...
    *                          like the case of contacts in mjcf, which are not part of the core ...
    *                          structures.
    *                        > These resources are used by the appropriate wrapper in the ...
    *                          specific backend used, usually in a method called ...
    *                          "_construct(BackendName)ResourcesFromKinTree". There we ...
    *                          have to grab all information from the structures of the kintree ...
    *                          and use it to instantiate the appropriate resources in the ...
    *                          desired backend. We also do some specific checks from some ...
    *                          extra information that might not be stored in the core kintree, ...
    *                          which should be stored in the 
    */


    TAgentKinTreeMjcf::TAgentKinTreeMjcf( const std::string& name,
                                          mjcf::GenericElement* modelElementPtr,
                                          const TVec3& position,
                                          const TVec3& rotation )
        : TAgentKinTree( name, position, rotation )
    {
        // save a copy of the elementPtr for later usage (if needed to inject resources)
        m_modelElementPtr = new mjcf::GenericElement();
        mjcf::deepCopy( m_modelElementPtr, modelElementPtr, NULL, m_name );
        // and set the model template type as being created from a mjc file
        m_modelTemplateType = MODEL_TEMPLATE_TYPE_MJCF;

        _extractMjcfModelSettings();
        _collectDefaults();
        _collectAssets();
        _initializeKinTree();
    }

    TAgentKinTreeMjcf::~TAgentKinTreeMjcf()
    {
        // @CHECK: calling base virtual destructor?
        m_modelElementPtr = NULL;
    }

    void TAgentKinTreeMjcf::_constructKinTree()
    {
        // Initialize recursive processing for this kintree
        // The info stored in the worldbody should not be used
        // All following elements should be processed properly

        // grab the worldbody and then the root body
        auto _worldBodyElmPtr   = mjcf::findFirstChildByType( m_modelElementPtr, "worldbody" );
        auto _rootBodyElmPtr    = mjcf::findFirstChildByType( _worldBodyElmPtr, "body" );

        // start recursive processing
        m_rootBodyPtr = _processBodyFromMjcf( _rootBodyElmPtr, NULL );

        // grab the actuators (sensors are just copied, and the joint sensors are created as needed)
        auto _actuatorsElmPtr = mjcf::findFirstChildByType( m_modelElementPtr, "actuator" );
        if ( _actuatorsElmPtr )
        {
            for ( size_t i = 0; i < _actuatorsElmPtr->children.size(); i++ )
                _processActuator( _actuatorsElmPtr->children[i] );
        }

        // // grab the contacts and update the names appropriately
        // auto _contactsElmPtr = mjcf::findFirstChildByType( m_modelElementPtr, "contact" );
        // if ( _contactsElmPtr )
        // {
        //     for ( size_t i = 0; i < _contactsElmPtr->children.size(); i++ )
        //     {
        //         auto _contactElm = _contactsElmPtr->children[i];
        //         if ( !_contactElm || 
        //              !_contactElm->hasAttributeString( "body1" ) ||
        //              !_contactElm->hasAttributeString( "body2" ) )
        //         {
        //             std::cout << "WARNING> invalid contact, it should have both body1 and body2 tags" << std::endl;
        //             continue;
        //         }
        //         auto _body1name = _contactElm->getAttributeString( "body1" );
        //         auto _body2name = _contactElm->getAttributeString( "body2" );
        //         _body1name = mjcf::computeMjcfName( "body", _body1name, m_name );
        //         _body2name = mjcf::computeMjcfName( "body", _body2name, m_name );
        //         _contactElm->setAttributeString( "body1", _body1name );
        //         _contactElm->setAttributeString( "body2", _body2name );
        //     }
        // }

        if ( m_rootBodyPtr && !m_mjcfModelSettings.useLocalCoordinates )
        {
            std::cout << "INFO> converting to local coordinates: " << m_name << std::endl;
            _convertGlobalToLocalCoordinates( m_rootBodyPtr, TMat4() );
        }
    }

    void TAgentKinTreeMjcf::_collectDefaults()
    {
        auto _defaultsElmPtr = mjcf::findFirstChildByType( m_modelElementPtr, "default" );

        if ( !_defaultsElmPtr )
            return;

        for ( size_t q = 0; q < _defaultsElmPtr->children.size(); q++ )
        {
            auto _child = _defaultsElmPtr->children[q];
            if ( _child->etype == "default" )
                _collectDefaultsFromClass( _child );
            else
                _collectDefaultsNoClass( _child );
        }
    }

    void TAgentKinTreeMjcf::_collectDefaultsFromClass( mjcf::GenericElement* defClassElmPtr )
    {
        auto _classId = defClassElmPtr->getAttributeString( "class" );
        if ( _classId == "" )
            return;

        if ( m_mjcfDefaultsPerClass.find( _classId ) == m_mjcfDefaultsPerClass.end() )
            m_mjcfDefaultsPerClass[_classId] = std::map< std::string, TGenericParams >();

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
                    m_mjcfDefaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeArrayFloat( _it->first ) );
                }
                else if ( _it->second == mjcf::TYPE_ARRAY_INT &&
                          _child->hasAttributeArrayInt( _it->first ) )
                {
                    m_mjcfDefaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeArrayInt( _it->first ) );
                }
                else if ( _it->second == mjcf::TYPE_FLOAT &&
                          _child->hasAttributeFloat( _it->first ) )
                {
                    m_mjcfDefaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeFloat( _it->first ) );
                }
                else if ( _it->second == mjcf::TYPE_INT &&
                          _child->hasAttributeInt( _it->first ) )
                {
                    m_mjcfDefaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeInt( _it->first ) );
                }
                else if ( _it->second == mjcf::TYPE_STRING &&
                          _child->hasAttributeString( _it->first ) )
                {
                    m_mjcfDefaultsPerClass[_classId][_etype].set( _it->first,
                                                                  _child->getAttributeString( _it->first ) );
                }
            }
        }
    }

    void TAgentKinTreeMjcf::_collectDefaultsNoClass( mjcf::GenericElement* defElmPtr )
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
                m_mjcfDefaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeArrayFloat( _it->first ) );
            }
            else if ( _it->second == mjcf::TYPE_ARRAY_INT &&
                      defElmPtr->hasAttributeArrayInt( _it->first ) )
            {
                m_mjcfDefaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeArrayInt( _it->first ) );
            }
            else if ( _it->second == mjcf::TYPE_FLOAT &&
                      defElmPtr->hasAttributeFloat( _it->first ) )
            {
                m_mjcfDefaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeFloat( _it->first ) );
            }
            else if ( _it->second == mjcf::TYPE_INT &&
                      defElmPtr->hasAttributeInt( _it->first ) )
            {
                m_mjcfDefaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeInt( _it->first ) );
            }
            else if ( _it->second == mjcf::TYPE_STRING &&
                      defElmPtr->hasAttributeString( _it->first ) )
            {
                m_mjcfDefaultsNoClass[_tagId].set( _it->first,
                                                   defElmPtr->getAttributeString( _it->first ) );
            }
        }
    }

    void TAgentKinTreeMjcf::_collectAssets()
    {
        auto _assetsElmPtr = mjcf::findFirstChildByType( m_modelElementPtr, "asset" );
        if ( !_assetsElmPtr )
        {
            // Nothing to do here
            return;
        }

        auto _assets = _assetsElmPtr->children;
        for ( size_t i = 0; i < _assets.size(); i++ )
        {
            auto _asset = _assets[i];
            if ( _asset->etype == "mesh" );
            {
                // Grab the mesh asset into a dict for easier usage ************
                TMjcfMeshAsset _meshAsset;

                // grab the name of the mesh
                _meshAsset.name = _asset->getAttributeString( "name" );
                // and the file resource
                _meshAsset.file = _asset->getAttributeString( "file" );

                // as well as the scale (in case there is)
                auto _scale = _asset->getAttributeVec3( "scale", 
                                                        { 1.0f, 1.0f, 1.0f } );
                _meshAsset.scale.x = _scale.x;
                _meshAsset.scale.y = _scale.y;
                _meshAsset.scale.z = _scale.z;

                m_mjcfMeshAssets[ _meshAsset.name ] = _meshAsset;
                // *************************************************************
                // cache this asset if not already cached **********************
                if ( m_mapKinTreeMeshAssets.find( _meshAsset.name ) ==
                     m_mapKinTreeMeshAssets.end() )
                {
                    auto _kinTreeMeshAsset = new TKinTreeMeshAsset();
                    _kinTreeMeshAsset->name = _meshAsset.name;
                    _kinTreeMeshAsset->file = _meshAsset.file;

                    m_kinTreeMeshAssets.push_back( _kinTreeMeshAsset );
                    m_mapKinTreeMeshAssets[_meshAsset.name] = _kinTreeMeshAsset;
                }
                // *************************************************************
            }
            // else
            // {
            //     // the others are not supported yet
            // }
        }
    }

    TKinTreeBody* TAgentKinTreeMjcf::_processBodyFromMjcf( mjcf::GenericElement* bodyElementPtr,
                                                           TKinTreeBody* parentKinBodyPtr )
    {
        // mjcf format defines joints as constraints from that ...
        // body to the body just created before, like this :
        //
        //  <body name="mjcbody_###_tmjcroot" pos="0 0 1.4">
        //      <freejoint name="mjcjoint_###_root" /><!--armature="0"-->
        //      <geom condim="1" material="matgeom" name="mjcgeom_###_tmjcroot" type="capsule" fromto="0 -0.07 0 0 .07 0"  size="0.07"/>
        //      <geom condim="1" material="matgeom" name="mjcgeom_###_head" type="sphere" pos="0 0 .19" size=".09"/>
        //      <geom condim="1" material="matgeom" name="mjcgeom_###_uwaist" type="capsule" fromto="-0.01 -0.06 -0.12 -0.01 .06 -0.12" size="0.06"/>
        //      <body name="mjcbody_###_lwaist" pos="-0.01 0 -0.260" quat="1.000 0 -0.002 0" >
        //          <geom condim="1" material="matgeom" name="mjcgeom_###_lwaist" type="capsule" fromto="0 -0.06 0 0 .06 0"  size="0.06" />
        //          <joint name="mjcjoint_###_abdomen_z" type="hinge" pos="0 0 0.065" axis="0 0 1" range="-45 45" damping="5" stiffness="20" armature="0.02" limited="true" />
        //          <joint name="mjcjoint_###_abdomen_y" type="hinge" pos="0 0 0.065" axis="0 1 0" range="-75 30" damping="5" stiffness="10" armature="0.02" limited="true" />
        //          <body name="mjcbody_###_pelvis" pos="0 0 -0.165" quat="1.000 0 -0.002 0" >
        //              <joint name="mjcjoint_###_abdomen_x" type="hinge" pos="0 0 0.1" axis="1 0 0" range="-35 35" damping="5" stiffness="10" armature="0.02" limited="true" />
        //              <geom condim="1" material="matgeom" name="mjcgeom_###_butt" type="capsule" fromto="-0.02 -0.07 0 -0.02 .07 0"  size="0.09" />
        // 
        // mjcbody_###_lwaist is connected by ###_abdomen_z and ###_abdomen_y to mjcbody_###_tmjcroot (2dof)
        // mjcbody_###_pelvis is connected by ###_abdomen_x to  mjcbody_###_lwaist (1dof)

        // grab body information
        auto _kinTreeBodyPtr = new TKinTreeBody();
        // grab the name
        _kinTreeBodyPtr->name = bodyElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( bodyElementPtr, _kinTreeBodyPtr->relTransform );
        // and the parent bodyptr as well
        _kinTreeBodyPtr->parentBodyPtr = parentKinBodyPtr;
        // and store it in the bodies buffer
        m_kinTreeBodies.push_back( _kinTreeBodyPtr );
        // and to the bodies map
        m_mapKinTreeBodies[ _kinTreeBodyPtr->name ] = _kinTreeBodyPtr;

        // grab the child-class (if given)
        auto _childClass = bodyElementPtr->getAttributeString( "childclass", "" );

        // grab all geom elements and process them as visual and collisions
        auto _geomElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "geom" );
        for ( size_t i = 0; i < _geomElementPtrs.size(); i++ )
        {
            // set child-class if geom element does not have any
            if ( _childClass != "" && !_geomElementPtrs[i]->hasAttributeString( "class" ) )
                _geomElementPtrs[i]->setAttributeString( "class", _childClass );

            auto _kinTreeVisualPtr      = _processVisualFromMjcf( _geomElementPtrs[i] );
            auto _kinTreeCollisionPtr   = _processCollisionFromMjcf( _geomElementPtrs[i] );

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

            auto _kinTreeJointPtr =  _processJointFromMjcf( _jointElementPtrs[i] );

            _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;

            _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
        }

        // grab the inertial properties (if given, as by default is computed from the geometries)
        // (If no inertia is given, by default is set to NULL, and inertia properties should be ...
        //  extracted once the model creation has been completed, so an extra initialization stage ...
        //  should be added. This is needed to ensure that for other backends they can grab the appropiate ...
        //  inertia computation) (Perhaps this could be avoided if we are sure that for any backend ...
        //  if no inertia is given, then by default the inertia is computed from primitives, and by the ...
        //  backend itself)
        auto _inertialElmPtr = mjcf::findFirstChildByType( bodyElementPtr, "inertial" );
        if ( _inertialElmPtr )
        {
            _kinTreeBodyPtr->inertiaPtr = _processInertialFromMjcf( _inertialElmPtr );
        }

        // and sites? @CHECK
        // ...

        // grab child bodies (if any) and repeat process
        auto _childBodiesElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "body" );
        for ( size_t i = 0; i < _childBodiesElementPtrs.size(); i++ )
        {
            // set child-class if joint element does not have any
            if ( _childClass != "" && !_childBodiesElementPtrs[i]->hasAttributeString( "childclass" ) )
                _childBodiesElementPtrs[i]->setAttributeString( "childclass", _childClass );

            _kinTreeBodyPtr->childBodies.push_back( _processBodyFromMjcf( _childBodiesElementPtrs[i], 
                                                                          _kinTreeBodyPtr ) );
        }

        return _kinTreeBodyPtr;
    }

    TKinTreeJoint* TAgentKinTreeMjcf::_processJointFromMjcf( mjcf::GenericElement* jointElementPtr )
    {
        auto _kinTreeJointPtr = new TKinTreeJoint();
        // grab the name
        // _kinTreeJointPtr->name = mjcf::computeMjcfName( "joint", 
        //                                                 jointElementPtr->getAttributeString( "name" ),
        //                                                 m_name );
        _kinTreeJointPtr->name = jointElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( jointElementPtr, _kinTreeJointPtr->relTransform );
        // and the type of joint
        _kinTreeJointPtr->type = _grabString( jointElementPtr, "type", "hinge" );
        // and the joint axis
        auto _axis = _grabVec3( jointElementPtr, "axis", { 0., 1., 0. } );
        _kinTreeJointPtr->axis = { _axis.x, _axis.y, _axis.z };
        // and the joint value clamping flag
        _kinTreeJointPtr->limited = ( _grabString( jointElementPtr, "limited", "false" ) == "true" );
        // in case not limited, assume low > high
        if ( !_kinTreeJointPtr->limited )
        {
            _kinTreeJointPtr->lowerLimit = 1.0f;
            _kinTreeJointPtr->upperLimit = -1.0f;
        }
        else
        {
            // and the range limits
            auto _limits = _grabVec2( jointElementPtr, "range", { -TYSOC_PI, TYSOC_PI } );
            _kinTreeJointPtr->lowerLimit = _limits.x;
            _kinTreeJointPtr->upperLimit = _limits.y;
        }
        // and the joint stiffness (@GENERIC)
        _kinTreeJointPtr->stiffness = _grabFloat( jointElementPtr, "stiffness", 0.0 );
        // and the joint armature (@GENERIC)
        _kinTreeJointPtr->armature = _grabFloat( jointElementPtr, "armature", 0.0 );
        // and the joint damping (@GENERIC)
        _kinTreeJointPtr->damping = _grabFloat( jointElementPtr, "damping", 0.0 );
        // and the joint ref (@GENERIUC)
        _kinTreeJointPtr->ref = _grabFloat( jointElementPtr, "ref", 0.0 );
        // child body should be set to NULL (used only for urdf?->seems not->@CHANGE)
        _kinTreeJointPtr->childBodyPtr = NULL;
        // and store it in the joints buffer
        m_kinTreeJoints.push_back( _kinTreeJointPtr );
        // and to the joints map
        m_mapKinTreeJoints[ _kinTreeJointPtr->name ] = _kinTreeJointPtr;

        return _kinTreeJointPtr;
    }

    TKinTreeVisual* TAgentKinTreeMjcf::_processVisualFromMjcf( mjcf::GenericElement* geomElementPtr )
    {
        auto _kinTreeVisualPtr = new TKinTreeVisual();
        // grab the name
        _kinTreeVisualPtr->name = geomElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( geomElementPtr, _kinTreeVisualPtr->relTransform );
        // and the type of visual/geom
        _kinTreeVisualPtr->geometry.type = _grabString( geomElementPtr, "type", "" );
        if ( _kinTreeVisualPtr->geometry.type == "" )
        {
            _kinTreeVisualPtr->geometry.type = "sphere";
        }
        // and the mesh filename in case there is any (this one is tricky)
        auto _meshId = _grabString( geomElementPtr, "mesh", "" );
        if ( m_mjcfMeshAssets.find( _meshId ) != m_mjcfMeshAssets.end() )
        {
            // if the mesh is is a "pure link" to a filename, then use the stored asset in map
            _kinTreeVisualPtr->geometry.meshId      = m_mjcfMeshAssets[ _meshId ].name;
            _kinTreeVisualPtr->geometry.filename    = m_mjcfMeshAssets[ _meshId ].file;
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
        bool _usesFromto = _extractStandardSize( geomElementPtr, 
                                                 _kinTreeVisualPtr->geometry.size,
                                                 _posFromFromto,
                                                 _rotFromFromto );
        _kinTreeVisualPtr->geometry.usesFromto = _usesFromto;
        if ( _usesFromto )
        {
            _kinTreeVisualPtr->relTransform.setPosition( _posFromFromto );
            _kinTreeVisualPtr->relTransform.setRotation( _rotFromFromto );
        }

        auto _rgba = _grabVec4( geomElementPtr, "rgba", TYSOC_DEFAULT_RGBA_COLOR );

        _kinTreeVisualPtr->material.diffuse     = { _rgba.x, _rgba.y, _rgba.z };
        _kinTreeVisualPtr->material.specular    = { _rgba.x, _rgba.y, _rgba.z };

        // and the material name (@GENERIC)
        _kinTreeVisualPtr->materialName = _grabString( geomElementPtr, "material", "" );
        // and store it in the visuals buffer
        m_kinTreeVisuals.push_back( _kinTreeVisualPtr );
        // and to the visuals map
        m_mapKinTreeVisuals[ _kinTreeVisualPtr->name ] = _kinTreeVisualPtr;

        return _kinTreeVisualPtr;
    }

    TKinTreeCollision* TAgentKinTreeMjcf::_processCollisionFromMjcf( mjcf::GenericElement* geomElementPtr )
    {
        auto _kinTreeCollisionPtr = new TKinTreeCollision();
        // grab the name
        _kinTreeCollisionPtr->name = geomElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( geomElementPtr, _kinTreeCollisionPtr->relTransform );
        // and the collision/geom
        _kinTreeCollisionPtr->geometry.type = _grabString( geomElementPtr, "type", "" );
        if ( _kinTreeCollisionPtr->geometry.type == "" )
        {
            _kinTreeCollisionPtr->geometry.type = "sphere";
        }
        // and the mesh filename in case there is any (this one is tricky)
        auto _meshId = _grabString( geomElementPtr, "mesh", "" );
        if ( m_mjcfMeshAssets.find( _meshId ) != m_mjcfMeshAssets.end() )
        {
            // if the mesh is is a "pure link" to a filename, then use the stored asset in map
            _kinTreeCollisionPtr->geometry.meshId      = m_mjcfMeshAssets[ _meshId ].name;
            _kinTreeCollisionPtr->geometry.filename    = m_mjcfMeshAssets[ _meshId ].file;
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
        bool _usesFromto = _extractStandardSize( geomElementPtr, 
                                                 _kinTreeCollisionPtr->geometry.size,
                                                 _posFromFromto,
                                                 _rotFromFromto );
        _kinTreeCollisionPtr->geometry.usesFromto = _usesFromto;
        if ( _usesFromto )
        {
            _kinTreeCollisionPtr->relTransform.setPosition( _posFromFromto );
            _kinTreeCollisionPtr->relTransform.setRotation( _rotFromFromto );
        }

        auto _rgba = _grabVec4( geomElementPtr, "rgba", TYSOC_DEFAULT_RGBA_COLOR );

        _kinTreeCollisionPtr->material.diffuse     = { _rgba.x, _rgba.y, _rgba.z };
        _kinTreeCollisionPtr->material.specular    = { _rgba.x, _rgba.y, _rgba.z };

        // and the contype collision bitmask (@GENERIC)
        _kinTreeCollisionPtr->contype = _grabInt( geomElementPtr, "contype", -1 );
        // and the conaffinity collision bitmask (@GENERIC)
        _kinTreeCollisionPtr->conaffinity = _grabInt( geomElementPtr, "conaffinity", -1 );
        // and the condim contact dimensionality (@GENERIC)
        _kinTreeCollisionPtr->condim = _grabInt( geomElementPtr, "condim", -1 );
        // and the group the object belongs (for internal compiler calcs.) (@GENERIC)
        _kinTreeCollisionPtr->group = _grabInt( geomElementPtr, "group", -1 );
        // and the friction (@GENERIC)
        _kinTreeCollisionPtr->friction = _grabArrayFloat( geomElementPtr, "friction", { 3, { 1., 0.005, 0.0001 } } );
        // and the density (@GENERIC)
        _kinTreeCollisionPtr->density = _grabFloat( geomElementPtr, "density", 1000. );
        // and store it in the collisions buffer
        m_kinTreeCollisions.push_back( _kinTreeCollisionPtr );
        // and to the collisions map
        m_mapKinTreeCollisions[ _kinTreeCollisionPtr->name ] = _kinTreeCollisionPtr;

        return _kinTreeCollisionPtr;
    }

    TKinTreeInertia* TAgentKinTreeMjcf::_processInertialFromMjcf( mjcf::GenericElement* inertialElmPtr )
    {
        // For this element, check this documentation from mujoco
        // url: http://mujoco.org/book/XMLreference.html#inertial
        auto _kinTreeInertia = new TKinTreeInertia();
        // grab the transform of the inertial frame:
        // from mujoco docs: origin in body's CM and axes along Principal Axes of inertia
        // no transformation is required if axes are not aligned, it's going to be done by ...
        // the compiler when the body is processed by computing the eigenvalues and (perhaps) doing ...
        // a similar trick to the one done in the bullet examples (in some urdf parsing they do this trick)
        _extractTransform( inertialElmPtr, _kinTreeInertia->relTransform );
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
            std::cout << "ERROR> no inertia-matrix given for agent: " << m_name << std::endl;
        }

        return _kinTreeInertia;
    }

    void TAgentKinTreeMjcf::_processActuator( mjcf::GenericElement* actuatorElementPtr )
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
        if ( m_mapKinTreeJoints.find( _jointName ) != m_mapKinTreeJoints.end() )
        {
            _kinTreeActuatorPtr->jointPtr = m_mapKinTreeJoints[ _jointName ];
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
        auto _ctrlLimits = _grabVec2( actuatorElementPtr, "ctrlrange", { -1, 1 } );
        _kinTreeActuatorPtr->minCtrl = _ctrlLimits.x;
        _kinTreeActuatorPtr->maxCtrl = _ctrlLimits.y;

        // @CHECK|@WIP : Should use variant here for extra parameters for ...
        // each backend type used, as some would support more/less features

        // and the ctrl clamping flag (@GENERIC)
        _kinTreeActuatorPtr->clampCtrl = ( _grabString( actuatorElementPtr, "ctrllimited", "true" ) == "true" );
        // and the position feedback gain (@GENERIC)
        _kinTreeActuatorPtr->kp = _grabFloat( actuatorElementPtr, "kp", 1.0 );
        // and the velocity feedback gain (@GENERIC)
        _kinTreeActuatorPtr->kv = _grabFloat( actuatorElementPtr, "kv", 1.0 );
        // and the gear scaling (@GENERIC)
        _kinTreeActuatorPtr->gear = _grabArrayFloat( actuatorElementPtr, "gear", { 6, { 1., 0., 0., 0., 0., 0. } } );

        // @TODO|CHECK: for some special joints (ball and free) "joint" is not the field we are looking for

        // and store it in the actuators buffer
        m_kinTreeActuators.push_back( _kinTreeActuatorPtr );
        // and to the actuators map
        m_mapKinTreeActuators[ _kinTreeActuatorPtr->name ] = _kinTreeActuatorPtr;
    }

    void TAgentKinTreeMjcf::_extractTransform( mjcf::GenericElement* elementPtr,
                                               TMat4& targetTransform )
    {
        // grab local position from element
        auto _relPosition   = _grabVec3( elementPtr, "pos", { 0.0, 0.0, 0.0 } );
        // and make the vector to be used
        TVec3 _rPosition    = { _relPosition.x, _relPosition.y, _relPosition.z };
        // and set it to the target transform
        targetTransform.setPosition( _rPosition );

        // check if we have euler
        if ( elementPtr->hasAttributeVec3( "euler" ) ||
             _hasDefaultAttrib( elementPtr, "euler" ) )
        {
            // extract rotation using euler
            auto _relEuler = _grabVec3( elementPtr, "euler", { 0.0, 0.0, 0.0 } );
            // and convert it to our tvec3 format
            TVec3 _rEuler = { _relEuler.x * ((float)M_PI) / 180.0f, 
                              _relEuler.y * ((float)M_PI) / 180.0f, 
                              _relEuler.z * ((float)M_PI) / 180.0f };
            // and convert to matrix type
            TMat3 _rRotation    = TMat3::fromEuler( _rEuler );
            // and set it to the target transform
            targetTransform.setRotation( _rRotation );
        }
        else if ( elementPtr->hasAttributeVec4( "quat" ) ||
                  _hasDefaultAttrib( elementPtr, "quat" ) )
        {
            // extract rotation using quaternions
            auto _relQuaternion = _grabVec4( elementPtr, "quat", { 1.0, 0.0, 0.0, 0.0 } );
            // and extract rotation (quaternion convention in mjcf is "wxyz", our is "xyzw", so compensate for this)
            TVec4 _rQuaternion  = { _relQuaternion.y, _relQuaternion.z, _relQuaternion.w, _relQuaternion.x };
            // and convert to matrix type
            TMat3 _rRotation = TMat3::fromQuaternion( _rQuaternion );
            // and set it to the target transform
            targetTransform.setRotation( _rRotation );
        }
        else if ( elementPtr->hasAttributeVec3( "zaxis" ) ||
                  _hasDefaultAttrib( elementPtr, "zaxis" ) )
        {
            // extract rotation using shortest quaternion to get to the given axis
            // grab the local zaxis
            auto _zAxisLocal = _grabVec3( elementPtr, "zaxis", { 0.0, 0.0, 1.0 } );
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

    bool TAgentKinTreeMjcf::_extractStandardSize( mjcf::GenericElement* geomElm,
                                                  TVec3& targetSize,
                                                  TVec3& posFromFromto,
                                                  TMat3& rotFromFromto )
    {
        auto _gname     = geomElm->getAttributeString( "name" );
        auto _gtype     = _grabString( geomElm, "type", "sphere" );
        auto _gsize     = _grabArrayFloat( geomElm, "size", { 1, { 0.01 } } );
        auto _gfromto   = _grabArrayFloat( geomElm, "fromto", { 0, { 0. } } );

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

    void TAgentKinTreeMjcf::_extractMjcfModelSettings()
    {
        auto _settingsElm = mjcf::findFirstChildByType( m_modelElementPtr, "compiler" );

        if ( _settingsElm )
        {
            m_mjcfModelSettings.useLocalCoordinates = ( _settingsElm->getAttributeString( "coordinate", "local" ) == "local" );
        }
        else
        {
            m_mjcfModelSettings.useLocalCoordinates = true;
        }
    }

    void TAgentKinTreeMjcf::_convertGlobalToLocalCoordinates( TKinTreeBody* kinTreeBodyPtr, 
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

    TMat4 TAgentKinTreeMjcf::_convertGlobalToLocalTransform( const TMat4& parentWorldTransform,
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

    std::string TAgentKinTreeMjcf::_grabString( mjcf::GenericElement* elementPtr,
                                                const std::string& attribId,
                                                const std::string& defString )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeString( attribId ) )
            return elementPtr->getAttributeString( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    if ( m_mjcfDefaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return m_mjcfDefaultsPerClass[_classId][_elType].getString( attribId, defString );
                }
            }
        }

        if ( m_mjcfDefaultsNoClass.find( _elType ) !=
             m_mjcfDefaultsNoClass.end() )
        {
            if ( m_mjcfDefaultsNoClass[_elType].hasParam( attribId ) )
                return m_mjcfDefaultsNoClass[_elType].getString( attribId, defString );
        }

        return defString;
    }

    float TAgentKinTreeMjcf::_grabFloat( mjcf::GenericElement* elementPtr,
                                         const std::string& attribId,
                                         const float& defFloat )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeFloat( attribId ) )
            return elementPtr->getAttributeFloat( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    if ( m_mjcfDefaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return m_mjcfDefaultsPerClass[_classId][_elType].getFloat( attribId, defFloat );
                }
            }
        }

        if ( m_mjcfDefaultsNoClass.find( _elType ) !=
             m_mjcfDefaultsNoClass.end() )
        {
            if ( m_mjcfDefaultsNoClass[_elType].hasParam( attribId ) )
                return m_mjcfDefaultsNoClass[_elType].getFloat( attribId, defFloat );
        }

        return defFloat;
    }

    int TAgentKinTreeMjcf::_grabInt( mjcf::GenericElement* elementPtr,
                                     const std::string& attribId,
                                     const int& defInt )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeInt( attribId ) )
            return elementPtr->getAttributeInt( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    if ( m_mjcfDefaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return m_mjcfDefaultsPerClass[_classId][_elType].getInt( attribId, defInt );
                }
            }
        }

        if ( m_mjcfDefaultsNoClass.find( _elType ) !=
             m_mjcfDefaultsNoClass.end() )
        {
            if ( m_mjcfDefaultsNoClass[_elType].hasParam( attribId ) )
                return m_mjcfDefaultsNoClass[_elType].getInt( attribId, defInt );
        }

        return defInt;
    }


    TVec2 TAgentKinTreeMjcf::_grabVec2( mjcf::GenericElement* elementPtr,
                                        const std::string& attribId,
                                        const TVec2& defVec2 )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeVec2( attribId ) )
            return elementPtr->getAttributeVec2( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    if ( m_mjcfDefaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return m_mjcfDefaultsPerClass[_classId][_elType].getVec2( attribId, defVec2 );
                }
            }
        }

        if ( m_mjcfDefaultsNoClass.find( _elType ) !=
             m_mjcfDefaultsNoClass.end() )
        {
            if ( m_mjcfDefaultsNoClass[_elType].hasParam( attribId ) )
                return m_mjcfDefaultsNoClass[_elType].getVec2( attribId, defVec2 );
        }

        return defVec2;
    }

    TVec3 TAgentKinTreeMjcf::_grabVec3( mjcf::GenericElement* elementPtr,
                                        const std::string& attribId,
                                        const TVec3& defVec3 )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeVec3( attribId ) )
            return elementPtr->getAttributeVec3( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    if ( m_mjcfDefaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return m_mjcfDefaultsPerClass[_classId][_elType].getVec3( attribId, defVec3 );
                }
            }
        }
        
        if ( m_mjcfDefaultsNoClass.find( _elType ) !=
             m_mjcfDefaultsNoClass.end() )
        {
            if ( m_mjcfDefaultsNoClass[_elType].hasParam( attribId ) )
                return m_mjcfDefaultsNoClass[_elType].getVec3( attribId, defVec3 );
        }

        return defVec3;
    }

    TVec4 TAgentKinTreeMjcf::_grabVec4( mjcf::GenericElement* elementPtr,
                                        const std::string& attribId,
                                        const TVec4& defVec4 )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeVec4( attribId ) )
            return elementPtr->getAttributeVec4( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    if ( m_mjcfDefaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return m_mjcfDefaultsPerClass[_classId][_elType].getVec4( attribId, defVec4 );
                }
            }
        }

        if ( m_mjcfDefaultsNoClass.find( _elType ) !=
             m_mjcfDefaultsNoClass.end() )
        {
            if ( m_mjcfDefaultsNoClass[_elType].hasParam( attribId ) )
                return m_mjcfDefaultsNoClass[_elType].getVec4( attribId, defVec4 );
        }

        return defVec4;
    }

    TSizef TAgentKinTreeMjcf::_grabArrayFloat( mjcf::GenericElement* elementPtr,
                                               const std::string& attribId,
                                               const TSizef& defSizef )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeArrayFloat( attribId ) )
            return elementPtr->getAttributeArrayFloat( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    if ( m_mjcfDefaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return m_mjcfDefaultsPerClass[_classId][_elType].getSizef( attribId, defSizef );
                }
            }
        }

        if ( m_mjcfDefaultsNoClass.find( _elType ) !=
             m_mjcfDefaultsNoClass.end() )
        {
            if ( m_mjcfDefaultsNoClass[_elType].hasParam( attribId ) )
                return m_mjcfDefaultsNoClass[_elType].getSizef( attribId, defSizef );
        }

        return defSizef;
    }

    TSizei TAgentKinTreeMjcf::_grabArrayInt( mjcf::GenericElement* elementPtr,
                                             const std::string& attribId,
                                             const TSizei& defSizei )
    {
        auto _elType = elementPtr->etype;

        if ( elementPtr->hasAttributeArrayInt( attribId ) )
            return elementPtr->getAttributeArrayInt( attribId );

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    if ( m_mjcfDefaultsPerClass[_classId][_elType].hasParam( attribId ) )
                        return m_mjcfDefaultsPerClass[_classId][_elType].getSizei( attribId, defSizei );
                }
            }
        }
        
        if ( m_mjcfDefaultsNoClass.find( _elType ) !=
             m_mjcfDefaultsNoClass.end() )
        {
            if ( m_mjcfDefaultsNoClass[_elType].hasParam( attribId ) )
                return m_mjcfDefaultsNoClass[_elType].getSizei( attribId, defSizei );
        }

        return defSizei;
    }

    bool TAgentKinTreeMjcf::_hasDefaultAttrib( mjcf::GenericElement* elementPtr,
                                              const std::string& attribId )
    {
        bool _hasDefault = false;
        auto _elmType = elementPtr->etype;

        if ( elementPtr->hasAttributeString( "class" ) )
        {
            auto _classId = elementPtr->getAttributeString( "class" );
            if ( m_mjcfDefaultsPerClass.find( _classId ) !=
                 m_mjcfDefaultsPerClass.end() )
            {
                if ( m_mjcfDefaultsPerClass[_classId].find( _elmType ) !=
                     m_mjcfDefaultsPerClass[_classId].end() )
                {
                    _hasDefault = m_mjcfDefaultsPerClass[_classId][_elmType].hasParam( attribId );
                }
            }
        }
        else if ( m_mjcfDefaultsNoClass.find( _elmType ) !=
                  m_mjcfDefaultsNoClass.end() )
        {
            _hasDefault = m_mjcfDefaultsNoClass[_elmType].hasParam( attribId );
        }

        return _hasDefault;
    }

    mjcf::GenericElement* TAgentKinTreeMjcf::getMjcfModelDataPtr()
    {
        return m_modelElementPtr;
    }

    TAgentKinTree* createKinTreeAgent( const std::string& name,
                                       mjcf::GenericElement* modelDataPtr,
                                       const TVec3& position,
                                       const TVec3& rotation )
    {
        auto _kinTreeAgent = new TAgentKinTreeMjcf( name, modelDataPtr, position, rotation );
        return _kinTreeAgent;
    }
}}