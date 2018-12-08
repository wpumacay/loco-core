
#include <agent/types/agent_kintree_mjcf.h>

namespace tysoc{
namespace agent{


    TAgentKinTreeMjcf::TAgentKinTreeMjcf( const std::string& name,
                                          const TVec3& position,
                                          mjcf::GenericElement* modelElementPtr )
        : TAgentKinTree( name, position )
    {
        // save the reference to the elementPtr for later usage (if needed to inject resources)
        m_modelElementPtr = modelElementPtr;

        _collectAssets();
        _initializeKinTree();
    }

    TAgentKinTreeMjcf::~TAgentKinTreeMjcf()
    {
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
        auto _actuatorsElmPtr   = mjcf::findFirstChildByType( m_modelElementPtr, "actuator" );
        for ( size_t i = 0; i < _actuatorsElmPtr->children.size(); i++ )
        {
            _processActuator( _actuatorsElmPtr->children[i] );
        }
    }

    void TAgentKinTreeMjcf::_initializeWorldTransforms()
    {
        if ( m_rootBodyPtr )
        {
            // start from the root (set transform as the start position passed)
            m_rootBodyPtr->worldTransform.setPosition( m_position );
            // make an update in the tree (the default transforms ...
            // should give a results that makes sense, at least visually)
            _initializeBody( m_rootBodyPtr );

            // update the sensors and actuators, as they are placed ...
            // in the objects updated before (joints, visuals, collisions, bodies)
            for ( size_t i = 0; i < m_kinTreeSensors.size(); i++ )
            {
                _updateSensor( m_kinTreeSensors[i] );
            }

            for ( size_t i = 0; i < m_kinTreeActuators.size(); i++ )
            {
                _updateActuator( m_kinTreeActuators[i] );
            }
        }
    }

    void TAgentKinTreeMjcf::_initializeBody( TKinTreeBody* kinTreeBodyPtr )
    {
        if ( !kinTreeBodyPtr )
        {
            return;
        }

        if ( kinTreeBodyPtr->parentBodyPtr )
        {
            kinTreeBodyPtr->worldTransform = kinTreeBodyPtr->parentBodyPtr->worldTransform * 
                                             kinTreeBodyPtr->relTransform;
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childVisuals.size(); i++ )
        {
            if ( kinTreeBodyPtr->childVisuals[i] )
            {
                _updateVisual( kinTreeBodyPtr->childVisuals[i] );
            }
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childCollisions.size(); i++ )
        {
            if ( kinTreeBodyPtr->childCollisions[i] )
            {
                _updateCollision( kinTreeBodyPtr->childCollisions[i] );
            }
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childJoints.size(); i++ )
        {
            if ( kinTreeBodyPtr->childJoints[i] )
            {
                _updateJoint( kinTreeBodyPtr->childJoints[i] );
            }
        }

        for ( size_t i = 0; i < kinTreeBodyPtr->childBodies.size(); i++ )
        {
            if ( kinTreeBodyPtr->childBodies[i] )
            {
                _initializeBody( kinTreeBodyPtr->childBodies[i] );
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
                TMjcfMeshAsset _meshAsset;

                // grab the name of the mesh
                _meshAsset.name = _asset->getAttributeString( "name" );
                // and the file resource
                _meshAsset.file = _asset->getAttributeString( "file" );

                // as well as the scale (in case there is)
                auto _scale = _asset->getAttributeVec3( "scale", { 1.0f, 1.0f, 1.0f } );
                _meshAsset.scale.x = _scale.x;
                _meshAsset.scale.y = _scale.y;
                _meshAsset.scale.z = _scale.z;

                m_mjcfMeshAssets[ _meshAsset.name ] = _meshAsset;
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

        // grab all geom elements and process them as visual and collisions
        auto _geomElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "geom" );
        for ( size_t i = 0; i < _geomElementPtrs.size(); i++ )
        {
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
           auto _kinTreeJointPtr =  _processJointFromMjcf( _jointElementPtrs[i] );

           _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;

           _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
        }

        // grab child bodies (if any) and repeat process
        auto _childBodiesElementPtrs = mjcf::getChildrenByType( bodyElementPtr, "body" );
        for ( size_t i = 0; i < _childBodiesElementPtrs.size(); i++ )
        {
            _kinTreeBodyPtr->childBodies.push_back( _processBodyFromMjcf( _childBodiesElementPtrs[i], 
                                                                          _kinTreeBodyPtr ) );
        }

        return _kinTreeBodyPtr;
    }

    TKinTreeJoint* TAgentKinTreeMjcf::_processJointFromMjcf( mjcf::GenericElement* jointElementPtr )
    {
        auto _kinTreeJointPtr = new TKinTreeJoint();
        // grab the name
        _kinTreeJointPtr->name = jointElementPtr->getAttributeString( "name" );
        // and the relative transform to the parent body
        _extractTransform( jointElementPtr, _kinTreeJointPtr->relTransform );
        // and the type of joint
        _kinTreeJointPtr->type = jointElementPtr->getAttributeString( "type" );
        // and the joint axis
        auto _axis = jointElementPtr->getAttributeVec3( "axis" );
        _kinTreeJointPtr->axis = { _axis.x, _axis.y, _axis.z };
        // child body should be set to NULL (used only for urdf)
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
        _kinTreeVisualPtr->geometry.type = geomElementPtr->getAttributeString( "type" );
        // and the mesh filename in case there is any (this one is tricky)
        auto _meshId = geomElementPtr->getAttributeString( "mesh" );
        if ( m_mjcfMeshAssets.find( _meshId ) != m_mjcfMeshAssets.end() )
        {
            // if the mesh is is a "pure link" to a filename, then use the stored asset in map
            _kinTreeVisualPtr->geometry.filename = m_mjcfMeshAssets[ _meshId ].file;
        }
        else
        {
            // if not, then is a pure path to the mesh file
            _kinTreeVisualPtr->geometry.filename = _meshId;
        }
        // and the visual/geom size (and check if uses fromto, so we can extract the relative transform)
        TVec3 _posFromFromto;
        TMat3 _rotFromFromto;
        bool _usesFromto = _extractStandardSize( geomElementPtr, 
                                                 _kinTreeVisualPtr->geometry.size,
                                                 _posFromFromto,
                                                 _rotFromFromto );
        if ( _usesFromto )
        {
            _kinTreeVisualPtr->relTransform.setPosition( _posFromFromto );
            _kinTreeVisualPtr->relTransform.setRotation( _rotFromFromto );
        }
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
        _kinTreeCollisionPtr->geometry.type = geomElementPtr->getAttributeString( "type" );
        // and the mesh filename in case there is any
        _kinTreeCollisionPtr->geometry.filename = geomElementPtr->getAttributeString( "mesh" );
        // and the mesh filename in case there is any (this one is tricky)
        auto _meshId = geomElementPtr->getAttributeString( "mesh" );
        if ( m_mjcfMeshAssets.find( _meshId ) != m_mjcfMeshAssets.end() )
        {
            // if the mesh is is a "pure link" to a filename, then use the stored asset in map
            _kinTreeCollisionPtr->geometry.filename = m_mjcfMeshAssets[ _meshId ].file;
        }
        else
        {
            // if not, then is a pure path to the mesh file
            _kinTreeCollisionPtr->geometry.filename = _meshId;
        }
        // and the collision/geom size (and check if uses fromto, so we can extract the relative transform)
        TVec3 _posFromFromto;
        TMat3 _rotFromFromto;
        bool _usesFromto = _extractStandardSize( geomElementPtr, 
                                                 _kinTreeCollisionPtr->geometry.size,
                                                 _posFromFromto,
                                                 _rotFromFromto );
        if ( _usesFromto )
        {
            _kinTreeCollisionPtr->relTransform.setPosition( _posFromFromto );
            _kinTreeCollisionPtr->relTransform.setRotation( _rotFromFromto );
        }
        // and store it in the collisions buffer
        m_kinTreeCollisions.push_back( _kinTreeCollisionPtr );
        // and to the collisions map
        m_mapKinTreeCollisions[ _kinTreeCollisionPtr->name ] = _kinTreeCollisionPtr;

        return _kinTreeCollisionPtr;
    }

    void TAgentKinTreeMjcf::_processActuator( mjcf::GenericElement* actuatorElementPtr )
    {
        auto _kinTreeActuatorPtr = new TKinTreeActuator();
        // grab the name
        _kinTreeActuatorPtr->name = actuatorElementPtr->getAttributeString( "name" );
        // and the type of actuator
        _kinTreeActuatorPtr->type = actuatorElementPtr->getAttributeString( "type" );
        // and a reference to the joint it controls
        _kinTreeActuatorPtr->jointPtr = m_mapKinTreeJoints[ actuatorElementPtr->getAttributeString( "joint" ) ];

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
        auto _relPosition   = elementPtr->getAttributeVec3( "pos", { 0.0, 0.0, 0.0 } );
        // and make the vector to be used
        TVec3 _rPosition    = { _relPosition.x, _relPosition.y, _relPosition.z };
        // and set it to the target transform
        targetTransform.setPosition( _rPosition );

        // check if we have euler
        if ( elementPtr->hasAttributeVec3( "euler" ) )
        {
            // extract rotation using euler
            auto _relEuler = elementPtr->getAttributeVec3( "euler", { 0.0, 0.0, 0.0 } );
            // and convert it to our tvec3 format
            TVec3 _rEuler = { _relEuler.x * ((float)M_PI) / 180.0f, 
                              _relEuler.y * ((float)M_PI) / 180.0f, 
                              _relEuler.z * ((float)M_PI) / 180.0f };
            // and convert to matrix type
            TMat3 _rRotation    = TMat3::fromEuler( _rEuler );
            // and set it to the target transform
            targetTransform.setRotation( _rRotation );
        }
        else
        {
            // extract rotation using quaternions
            auto _relQuaternion = elementPtr->getAttributeVec4( "quat", { 1.0, 0.0, 0.0, 0.0 } );
            // and extract rotation (quaternion convention in mjcf is "wxyz", our is "xyzw", so compensate for this)
            TVec4 _rQuaternion  = { _relQuaternion.y, _relQuaternion.z, _relQuaternion.w, _relQuaternion.x };
            // and convert to matrix type
            TMat3 _rRotation    = TMat3::fromQuaternion( _rQuaternion );
            // and set it to the target transform
            targetTransform.setRotation( _rRotation );
        }
    }

    bool TAgentKinTreeMjcf::_extractStandardSize( mjcf::GenericElement* geomElm,
                                                  TVec3& targetSize,
                                                  TVec3& posFromFromto,
                                                  TMat3& rotFromFromto )
    {
        auto _gname     = geomElm->getAttributeString( "name" );
        auto _gtype     = geomElm->getAttributeString( "type" );
        auto _gsize     = geomElm->getAttributeArrayFloat( "size" );
        auto _gfromto   = geomElm->getAttributeArrayFloat( "fromto" );

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
}}