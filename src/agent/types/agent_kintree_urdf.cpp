
#include <agent/types/agent_kintree_urdf.h>

namespace tysoc {
namespace agent {



    TAgentKinTreeUrdf::TAgentKinTreeUrdf( const std::string& name,
                                          const TVec3& position,
                                          urdf::UrdfModel* urdfModelPtr )
        : TAgentKinTree( name, position )
    {
        // save the reference to the urdfModelPtr for easier usage (should not modify it, read only)
        m_urdfModelPtr = new urdf::UrdfModel();
        urdf::deepCopy( m_urdfModelPtr, urdfModelPtr, m_name );
        // and set the model template type as being created from a urdf file
        m_modelTemplateType = MODEL_TEMPLATE_TYPE_URDF;

        _collectAssets();
        _initializeKinTree();
    }

    TAgentKinTreeUrdf::~TAgentKinTreeUrdf()
    {
        // @CHECK: calling base virtual destructor?
        m_urdfModelPtr = NULL;
    }

    void TAgentKinTreeUrdf::_collectAssets()
    {
        // collect mesh assets recursively from the urdf model
        if ( m_urdfModelPtr )
            _collectAssetsFromLink( m_urdfModelPtr->rootLinks[0] );
    }

    void TAgentKinTreeUrdf::_collectAssetsFromLink( urdf::UrdfLink* urdfLinkPtr )
    {
        if ( !urdfLinkPtr )
            return;

        // grab the visuals buffer (extend from shapes, so might have assets)
        auto _visuals       = urdfLinkPtr->visuals;
        // grab the collisions buffer as well
        auto _collisions    = urdfLinkPtr->collisions;
        // and combine them into a single shapes buffer (to avoid repeating code)
        auto _shapes        = std::vector< urdf::UrdfShape* >();
        for ( size_t i = 0; i < _visuals.size(); i++ )
            _shapes.push_back( _visuals[i] );
        for ( size_t i = 0; i < _collisions.size(); i++ )
            _shapes.push_back( _collisions[i] );

        // check for potential mesh assets in the "shapes" buffer
        for ( size_t i = 0; i < _shapes.size(); i++ )
        {
            // grab the geometry from the visual
            auto _geometryPtr = _shapes[i]->geometry;

            // just a sanity check
            if ( !_geometryPtr )
                continue;

            // and check if the geometry is a mesh (potential asset)
            if ( _geometryPtr->type != "mesh" )
                continue;

            // also check if the mesh is not already in the set
            if ( m_urdfMeshAssets.find( _geometryPtr->filename ) !=
                 m_urdfMeshAssets.end() )
                continue;

            // grab the mesh data into a meshasset object
            TUrdfMeshAsset _meshAsset;
            // Please, no weird names. Should just be :
            //      FILENAME.EXTENSION
            // Where FILENAME should not include special characters (specially ...
            // '.', as we are spliting the EXTENSION from the FILENAME using '.')
            auto _fileComponents = parsing::split( _geometryPtr->filename, '.' );
            if ( _fileComponents.size() != 2 )
            {
                std::cout << "WARNING> seems that there is an issue with the " 
                          << "the mesh's filename: " << _geometryPtr->filename
                          << std::endl;

                std::cout << "?size: " << _fileComponents.size() << std::endl;

                continue;
            }

            auto _fileName          = _fileComponents[0];
            auto _fileExtension     = _fileComponents[1];

            _meshAsset.name     = _fileName;
            _meshAsset.file     = _geometryPtr->filename;
            _meshAsset.scale    = _geometryPtr->size;

            // and store it into the TUrdfMeshAsset set
            m_urdfMeshAssets[ _meshAsset.name ] = _meshAsset;

            // also cache it into the core TKinTreeMeshAsset dictionary and buffer
            if ( m_mapKinTreeMeshAssets.find( _meshAsset.name ) ==
                 m_mapKinTreeMeshAssets.end() )
            {
                auto _kinTreeMeshAsset = new TKinTreeMeshAsset();
                _kinTreeMeshAsset->name = _meshAsset.name;
                _kinTreeMeshAsset->file = _meshAsset.file;

                m_kinTreeMeshAssets.push_back( _kinTreeMeshAsset );
                m_mapKinTreeMeshAssets[ _meshAsset.name ] = _kinTreeMeshAsset;
            }
        }

        // repeat recursively for all child links
        for ( size_t i = 0; i < urdfLinkPtr->childLinks.size(); i++ )
        {
            _collectAssetsFromLink( urdfLinkPtr->childLinks[i] );
        }
    }

    void TAgentKinTreeUrdf::_constructKinTree()
    {
        // Some sanity check. The parser already handled the warning/error message
        if ( m_urdfModelPtr->rootLinks.size() < 1 )
            return;

        // more sanity check. Just remind the user in case there are more root links
        if ( m_urdfModelPtr->rootLinks.size() > 1 )
        {
            std::cout << "WARNING> It seems that agent: " << m_name << " "
                      << "has more root links that it should(1). Using first one"
                      << std::endl;
        }

        // grab the first root link
        auto _rootLink = m_urdfModelPtr->rootLinks[0];
        // and start the recursive process
        m_rootBodyPtr = _processBodyFromUrdf( _rootLink, NULL );

        // @CHECK
        // Process actuators ( if not defined in the urdf as xml extensions ...
        // then we should just create some default actuators as PD controllers )

        _constructDefaultActuators();
    }

    TKinTreeBody* TAgentKinTreeUrdf::_processBodyFromUrdf( urdf::UrdfLink* urdfLinkPtr, TKinTreeBody* parentKinBodyPtr )
    {
        // grab body information
        auto _kinTreeBodyPtr = new TKinTreeBody();
        // grab the name
        _kinTreeBodyPtr->name = urdfLinkPtr->name;
        // and the relative transform to the parent body
        if ( urdfLinkPtr->parentJoint )
            _kinTreeBodyPtr->relTransform = urdfLinkPtr->parentJoint->parentLinkToJointTransform;
        else
            _kinTreeBodyPtr->relTransform = TMat4();// just identity (must be root)
        // and the parent bodyptr as well
        _kinTreeBodyPtr->parentBodyPtr = parentKinBodyPtr;
        // and store it in the bodies buffer
        m_kinTreeBodies.push_back( _kinTreeBodyPtr );
        // and to the bodies map
        m_mapKinTreeBodies[ _kinTreeBodyPtr->name ] = _kinTreeBodyPtr;

        // grab all urdfvisuals and store them in the body node
        auto _urdfVisuals = urdfLinkPtr->visuals;
        for ( size_t i = 0; i < _urdfVisuals.size(); i++ )
        {
            // process the element to create a TKinTreeVisual
            auto _kinTreeVisualPtr = _processVisualFromUrdf( _urdfVisuals[i] );
            // link this visual to its parent (current body being processed)
            _kinTreeVisualPtr->parentBodyPtr = _kinTreeBodyPtr;
            // and add it to the children of the current body being processed
            _kinTreeBodyPtr->childVisuals.push_back( _kinTreeVisualPtr );
        }

        // grab all urdfcollisions and store them in the body node
        auto _urdfCollisions = urdfLinkPtr->collisions;
        for ( size_t i = 0; i < _urdfCollisions.size(); i++ )
        {
            // process the element to create a TKinTreeCollision
            auto _kinTreeCollisionPtr = _processCollisionFromUrdf( _urdfCollisions[i] );
            // link this collision to its parent (current body being processed)
            _kinTreeCollisionPtr->parentBodyPtr = _kinTreeBodyPtr;
            // and add it to the children of the current body being processed
            _kinTreeBodyPtr->childCollisions.push_back( _kinTreeCollisionPtr );
        }

        // create a single joint as dof using the link's parentjoint
        auto _urdfJointPtr = urdfLinkPtr->parentJoint;
        if ( _urdfJointPtr )
        {
            auto _kinTreeJointPtr = _processJointFromUrdf( _urdfJointPtr );
            _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;
            _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
        }
        // auto _urdfJoints = urdfLinkPtr->childJoints;
        // for ( size_t i = 0; i < _urdfJoints.size(); i++ )
        // {
        //     // process the element to create a TKinTreeJoint
        //     auto _kinTreeJointPtr = _processJointFromUrdf( _urdfJoints[i] );
        //     // link this joint to its parent (current body being processed)
        //     _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;
        //     // wait for the connections step, as this joints are children of the ...
        //     // parent of this body, not to this body itself
        // }

        // grab the inertial properties (if not given, by default it's computed from the geometries)
        // (If no inertia is given, by default is set to NULL, and inertia properties should be ...
        //  extracted once the model creation has been completed, so an extra initialization stage ...
        //  should be added. This is needed to ensure that for other backends they can grab the appropiate ...
        //  inertia computation) (Perhaps this could be avoided if we are sure that for any backend ...
        //  if no inertia is given, then by default the inertia is computed from primitives, and by the ...
        //  backend itself)
        auto _urdfInertia = urdfLinkPtr->inertia;
        if ( _urdfInertia )
            _kinTreeBodyPtr->inertiaPtr = _processInertialFromUrdf( _urdfInertia );

        // and sites? @CHECK
        // ...

        // grab child bodies (if any) and repeat process
        auto _urdfChildLinks = urdfLinkPtr->childLinks;
        for ( size_t i = 0; i < _urdfChildLinks.size(); i++ )
        {
            _kinTreeBodyPtr->childBodies.push_back( _processBodyFromUrdf( _urdfChildLinks[i], 
                                                                          _kinTreeBodyPtr ) );
        }

        return _kinTreeBodyPtr;
    }

    TKinTreeJoint* TAgentKinTreeUrdf::_processJointFromUrdf( urdf::UrdfJoint* urdfJointPtr )
    {
        auto _kinTreeJointPtr = new TKinTreeJoint();
        // grab the name
        _kinTreeJointPtr->name = urdfJointPtr->name;
        // and the relative transform to the parent body
        _kinTreeJointPtr->relTransform = TMat4();
        // and the type of joint
        _kinTreeJointPtr->type = urdfJointPtr->type;
        // and the joint axis
        _kinTreeJointPtr->axis = urdfJointPtr->localJointAxis;
        // and the range limits
        _kinTreeJointPtr->lowerLimit = urdfJointPtr->lowerLimit;
        _kinTreeJointPtr->upperLimit = urdfJointPtr->upperLimit;
        // and the joint value clamping flag
        if ( urdfJointPtr->lowerLimit == 0.0 && urdfJointPtr->upperLimit == 0.0 )
            _kinTreeJointPtr->limited = false;
        else
            _kinTreeJointPtr->limited = true;
        // and the joint stiffness (@GENERIC)
        _kinTreeJointPtr->stiffness = 0.0;
        // and the joint armature (@GENERIC)
        _kinTreeJointPtr->armature = 0.0;
        // and the joint damping (@GENERIC)
        _kinTreeJointPtr->damping = 0.0;
        // child body should be set to NULL
        _kinTreeJointPtr->childBodyPtr = NULL;
        // and store it in the joints buffer
        m_kinTreeJoints.push_back( _kinTreeJointPtr );
        // and to the joints map
        m_mapKinTreeJoints[ _kinTreeJointPtr->name ] = _kinTreeJointPtr;

        return _kinTreeJointPtr;
    }

    TKinTreeVisual* TAgentKinTreeUrdf::_processVisualFromUrdf( urdf::UrdfVisual* urdfVisualtPtr )
    {
        auto _kinTreeVisualPtr = new TKinTreeVisual();
        // grab the name
        _kinTreeVisualPtr->name = urdfVisualtPtr->name;
        // and the relative transform to the parent body
        _kinTreeVisualPtr->relTransform = urdfVisualtPtr->localTransform;
        // and the type of visual/geom
        _kinTreeVisualPtr->geometry.type = urdfVisualtPtr->geometry->type;
        // and the mesh filename in case there is any
        if ( urdfVisualtPtr->geometry->type == "mesh" )
        {
            auto _fileComponents = parsing::split( urdfVisualtPtr->geometry->filename, '.' );
            if ( _fileComponents.size() != 2 )
            {
                std::cout << "WARNING> seems that there is an issue with the " 
                          << "the mesh's filename: " << urdfVisualtPtr->geometry->filename
                          << std::endl;

                // Set filename as the geometry->filename
                _kinTreeVisualPtr->geometry.meshId      = "";
                _kinTreeVisualPtr->geometry.filename    = urdfVisualtPtr->geometry->filename;
            }
            else
            {
                auto _meshId = _fileComponents[0];

                if ( m_urdfMeshAssets.find( _meshId ) != m_urdfMeshAssets.end() )
                {
                    _kinTreeVisualPtr->geometry.meshId      = m_urdfMeshAssets[ _meshId ].name;
                    _kinTreeVisualPtr->geometry.filename    = m_urdfMeshAssets[ _meshId ].file;
                }
                else
                {
                    _kinTreeVisualPtr->geometry.meshId      = "";
                    _kinTreeVisualPtr->geometry.filename    = _meshId;
                }
            }
        }
        // and the visual size
        _extractStandardSize( urdfVisualtPtr->geometry, 
                              _kinTreeVisualPtr->geometry.size );
        _kinTreeVisualPtr->geometry.usesFromto = false;

        // and the material (colors)
        auto _rgba = urdfVisualtPtr->material->color;
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

    TKinTreeCollision* TAgentKinTreeUrdf::_processCollisionFromUrdf( urdf::UrdfCollision* urdfCollisionPtr )
    {
        auto _kinTreeCollisionPtr = new TKinTreeCollision();
        // grab the name
        _kinTreeCollisionPtr->name = urdfCollisionPtr->name;
        // and the relative transform to the parent body
        _kinTreeCollisionPtr->relTransform = urdfCollisionPtr->localTransform;
        // and the collision/geom shape
        _kinTreeCollisionPtr->geometry.type = urdfCollisionPtr->geometry->type;
        // and the mesh filename in case there is any
        if ( urdfCollisionPtr->geometry->type == "mesh" )
        {
            auto _fileComponents = parsing::split( urdfCollisionPtr->geometry->filename, '.' );
            if ( _fileComponents.size() != 2 )
            {
                std::cout << "WARNING> seems that there is an issue with the " 
                          << "the mesh's filename: " << urdfCollisionPtr->geometry->filename
                          << std::endl;

                // Set filename as the geometry->filename
                _kinTreeCollisionPtr->geometry.meshId       = "";
                _kinTreeCollisionPtr->geometry.filename     = urdfCollisionPtr->geometry->filename;
            }
            else
            {
                auto _meshId = _fileComponents[0];

                if ( m_urdfMeshAssets.find( _meshId ) != m_urdfMeshAssets.end() )
                {
                    _kinTreeCollisionPtr->geometry.meshId       = m_urdfMeshAssets[ _meshId ].name;
                    _kinTreeCollisionPtr->geometry.filename     = m_urdfMeshAssets[ _meshId ].file;
                }
                else
                {
                    _kinTreeCollisionPtr->geometry.meshId       = "";
                    _kinTreeCollisionPtr->geometry.filename     = _meshId;
                }
            }
        }
        // and the collision/geom size
        _extractStandardSize( urdfCollisionPtr->geometry, 
                              _kinTreeCollisionPtr->geometry.size );
        _kinTreeCollisionPtr->geometry.usesFromto = false;
        // and the contype collision bitmask (@GENERIC)
        _kinTreeCollisionPtr->contype = 1;
        // and the conaffinity collision bitmask (@GENERIC)
        _kinTreeCollisionPtr->conaffinity = 0;
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

    TKinTreeInertia* TAgentKinTreeUrdf::_processInertialFromUrdf( urdf::UrdfInertia* urdfInertiaPtr )
    {
        // For this element, check this documentation from ros-urdf
        // url: http://wiki.ros.org/urdf/XML/link
        auto _kinTreeInertia = new TKinTreeInertia();
        // grab the transform of the inertial frame:
        _kinTreeInertia->relTransform = urdfInertiaPtr->localTransform;
        // grab the mass as well (required)
        _kinTreeInertia->mass = urdfInertiaPtr->mass;
        // and also the inertia matrix
        _kinTreeInertia->ixx = urdfInertiaPtr->ixx;
        _kinTreeInertia->iyy = urdfInertiaPtr->iyy;
        _kinTreeInertia->izz = urdfInertiaPtr->izz;
        _kinTreeInertia->ixy = urdfInertiaPtr->ixy;
        _kinTreeInertia->ixz = urdfInertiaPtr->ixz;
        _kinTreeInertia->iyz = urdfInertiaPtr->iyz;

        return _kinTreeInertia;
    }

    void TAgentKinTreeUrdf::_constructDefaultActuators()
    {
        for ( size_t q = 0; q < m_kinTreeJoints.size(); q++ )
        {
            if ( m_kinTreeJoints[q]->type == "free" ||
                 m_kinTreeJoints[q]->type == "fixed" ||
                 m_kinTreeJoints[q]->type == "world" )
            {
                continue;
            }

            auto _kinTreeActuatorPtr = new TKinTreeActuator();
            _kinTreeActuatorPtr->name = urdf::computeUrdfName( "actuator",
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

    void TAgentKinTreeUrdf::_extractStandardSize( urdf::UrdfGeometry* urdfGeometryPtr,
                                                  TVec3& targetSize )
    {
        // I know, just copy the vec3. But maybe we will run into extra checks ...
        // as we debug the code. @CHECK|@REFACTOR|@REMOVE

        auto _gtype = urdfGeometryPtr->type;
        auto _gsize = urdfGeometryPtr->size;

        if ( _gtype == "plane" )
        {
            targetSize.x = _gsize.x;
            targetSize.y = _gsize.y;
        }
        else if ( _gtype == "sphere" )
        {
            targetSize.x = _gsize.x;
        }
        else if ( _gtype == "capsule" ||
                  _gtype == "cylinder" )
        {
            targetSize.x = _gsize.x;
            targetSize.y = _gsize.y;
        }
        else if ( _gtype == "box" )
        {
            targetSize.x = _gsize.x;
            targetSize.y = _gsize.y;
            targetSize.z = _gsize.z;
        }
        else if ( _gtype == "mesh" )
        {
            targetSize.x = _gsize.x;
            targetSize.y = _gsize.y;
            targetSize.z = _gsize.z;
        }
    }

    urdf::UrdfModel* TAgentKinTreeUrdf::getUrdfModelDataPtr()
    {
        return m_urdfModelPtr;
    }

    TAgentKinTree* createKinTreeAgent( const std::string& name,
                                       const TVec3& position,
                                       urdf::UrdfModel* modelDataPtr )
    {
        auto _kinTreeAgent = new TAgentKinTreeUrdf( name, position, modelDataPtr );
        return _kinTreeAgent;
    }

}}