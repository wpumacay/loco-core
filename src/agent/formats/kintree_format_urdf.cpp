
#include <agent/formats/kintree_format_urdf.h>

namespace tysoc {
namespace agent {


    TAgent* createAgentFromModel( urdf::UrdfModel* modelDataPtr,
                                  const std::string& name,
                                  const TVec3& position,
                                  const TVec3& rotation )
    {
        TUrdfParsingContext _context;
        _context.agentPtr = new TAgent( name, position, rotation );
        _context.modelDataPtr = new urdf::UrdfModel();

        // create a copy of the model data with the names modified appropriately
        urdf::deepCopy( _context.modelDataPtr, modelDataPtr, name );

        // grab some required info before start constructing the kintree
        _collectAssetsFromLink( _context, _context.modelDataPtr->rootLinks[0] );

        // set the model format we are about to parse
        _context.agentPtr->setModelFormat( MODEL_FORMAT_URDF );

        /**********************************************************************/
        /*                       KINTREE CONSTRUCTION                         */
        /**********************************************************************/

        // Some sanity check. The parser already handled the warning/error message
        if ( _context.modelDataPtr->rootLinks.size() < 1 )
        {
            std::cout << "ERROR> no root links found in urdf model "
                      << "for agent with name: " << name << std::endl;
            return NULL;
        }

        // more sanity check. Just remind the user in case there are more root links
        if ( _context.modelDataPtr->rootLinks.size() > 1 )
        {
            std::cout << "WARNING> It seems that agent (" << name << ") "
                      << "has more root links that it should(1). Using first one"
                      << std::endl;
        }


        // grab the first root link
        auto _rootLink = _context.modelDataPtr->rootLinks[0];
        // and start the recursive process
        auto _rootBodyPtr = _processBodyFromUrdf( _context, _rootLink, NULL );

        if ( !_rootBodyPtr )
        {
            std::cout << "ERROR> something went wrong while parsing agent: "
                      << name << ". Processed root body is NULL" << std::endl;
            return NULL;
        }

        // make sure we set the root for this agent we are constructing
        _context.agentPtr->setRootBody( _rootBodyPtr );
        // store a reference to the model data
        _context.agentPtr->setModelDataUrdf( _context.modelDataPtr );

        // @CHECK
        // Process actuators ( if not defined in the urdf as xml extensions ...
        // then we should just create some default actuators as PD controllers )

        _constructDefaultActuators( _context );

        // Set the compensation matrix (fixes orientation mismatch with format)
        _context.agentPtr->setZeroCompensationMatrix( modelDataPtr->zeroCompensation );

        // // @DEBUG: exlude all contacts
        // for ( size_t i = 0; i < _context.agentPtr->bodies.size(); i++ )
        // {
        //     for ( size_t j = 0; j < _context.agentPtr->bodies.size(); j++ )
        //     {
        //         _context.agentPtr->exclusionContacts.push_back( std::make_pair( _context.agentPtr->bodies[i]->name,
        //                                                                         _context.agentPtr->bodies[j]->name ) );
        //     }
        // }

        // Grab all extra exclusion contacts from the urdf file
        auto _exclusionPairs = _context.modelDataPtr->exclusionPairs;
        for ( size_t q = 0; q < _exclusionPairs.size(); q++ )
            _context.agentPtr->exclusionContacts.push_back( _exclusionPairs[q] );

        // Finally, initialize the agent
        _context.agentPtr->initialize();

        /**********************************************************************/

        return _context.agentPtr;
    }

    TKinTreeBody* _processBodyFromUrdf( TUrdfParsingContext& context, 
                                        urdf::UrdfLink* urdfLinkPtr, 
                                        TKinTreeBody* parentKinBodyPtr )
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
        context.agentPtr->bodies.push_back( _kinTreeBodyPtr );
        // and to the bodies map
        context.agentPtr->bodiesMap[ _kinTreeBodyPtr->name ] = _kinTreeBodyPtr;

        // grab all urdfvisuals and store them in the body node
        auto _urdfVisuals = urdfLinkPtr->visuals;
        for ( size_t i = 0; i < _urdfVisuals.size(); i++ )
        {
            // process the element to create a TKinTreeVisual
            auto _kinTreeVisualPtr = _processVisualFromUrdf( context, _urdfVisuals[i] );
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
            auto _kinTreeCollisionPtr = _processCollisionFromUrdf( context, _urdfCollisions[i] );
            // link this collision to its parent (current body being processed)
            _kinTreeCollisionPtr->parentBodyPtr = _kinTreeBodyPtr;
            // and add it to the children of the current body being processed
            _kinTreeBodyPtr->childCollisions.push_back( _kinTreeCollisionPtr );
        }

        // create a single joint as dof using the link's parentjoint
        auto _urdfJointPtr = urdfLinkPtr->parentJoint;
        if ( _urdfJointPtr )
        {
            auto _kinTreeJointPtr = _processJointFromUrdf( context, _urdfJointPtr );
            _kinTreeJointPtr->parentBodyPtr = _kinTreeBodyPtr;
            _kinTreeBodyPtr->childJoints.push_back( _kinTreeJointPtr );
        }

        // grab the inertial properties (if given, as by default is computed from the geometries)
        auto _urdfInertia = urdfLinkPtr->inertia;
        if ( _urdfInertia )
            _kinTreeBodyPtr->inertiaPtr = _processInertialFromUrdf( context, _urdfInertia );

        // @TODO: Add support for sites, as some models may need them (sites ...
        // here consists of more abstract objects that can be used to trigger ...
        // special functionality in more complicated environments, like opening ...
        // something if a button is touched, or getting a reward (+/-) if this ...
        // abstract site touches an object)

        // grab child bodies (if any) and repeat process
        auto _urdfChildLinks = urdfLinkPtr->childLinks;
        for ( size_t i = 0; i < _urdfChildLinks.size(); i++ )
        {
            auto _childKinTreeBodyPtr = _processBodyFromUrdf( context,
                                                              _urdfChildLinks[i], 
                                                              _kinTreeBodyPtr );
            _kinTreeBodyPtr->childBodies.push_back( _childKinTreeBodyPtr );

            // exclude contact between this body and this child body
            context.agentPtr->exclusionContacts.push_back( std::make_pair( 
                                                                _kinTreeBodyPtr->name,
                                                                _childKinTreeBodyPtr->name ) );
        }

        return _kinTreeBodyPtr;
    }

    TKinTreeJoint* _processJointFromUrdf( TUrdfParsingContext& context, 
                                          urdf::UrdfJoint* urdfJointPtr )
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
        if ( urdfJointPtr->lowerLimit > urdfJointPtr->upperLimit )
            _kinTreeJointPtr->limited = false;
        else
            _kinTreeJointPtr->limited = true;
        // and the joint stiffness
        _kinTreeJointPtr->stiffness = 0.0;
        // and the joint armature
        _kinTreeJointPtr->armature = 0.0;
        // and the joint damping
        _kinTreeJointPtr->damping = 0.0;

        // and store it in the joints buffer
        context.agentPtr->joints.push_back( _kinTreeJointPtr );
        // and to the joints map
        context.agentPtr->jointsMap[ _kinTreeJointPtr->name ] = _kinTreeJointPtr;

        return _kinTreeJointPtr;
    }

    TKinTreeVisual* _processVisualFromUrdf( TUrdfParsingContext& context, 
                                            urdf::UrdfVisual* urdfVisualtPtr )
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
                          << ". It should be of the form NAME.EXTENSION"
                          << std::endl;

                // Set filename as the geometry->filename
                _kinTreeVisualPtr->geometry.meshId      = "";
                _kinTreeVisualPtr->geometry.filename    = urdfVisualtPtr->geometry->filename;
            }
            else
            {
                auto _meshId = _fileComponents[0];

                if ( context.agentPtr->meshAssetsMap.find( _meshId ) != 
                     context.agentPtr->meshAssetsMap.end() )
                {
                    _kinTreeVisualPtr->geometry.meshId = 
                            context.agentPtr->meshAssetsMap[ _meshId ]->name;

                    _kinTreeVisualPtr->geometry.filename = 
                            context.agentPtr->meshAssetsMap[ _meshId ]->file;
                }
                else
                {
                    _kinTreeVisualPtr->geometry.meshId   = "";
                    _kinTreeVisualPtr->geometry.filename = _meshId;
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

        // and the material name (not used for this format)
        _kinTreeVisualPtr->material.name = "not-used";
        // and store it in the visuals buffer
        context.agentPtr->visuals.push_back( _kinTreeVisualPtr );
        // and to the visuals map
        context.agentPtr->visualsMap[ _kinTreeVisualPtr->name ] = _kinTreeVisualPtr;

        return _kinTreeVisualPtr;
    }

    TKinTreeCollision* _processCollisionFromUrdf( TUrdfParsingContext& context, 
                                                  urdf::UrdfCollision* urdfCollisionPtr )
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

                if ( context.agentPtr->meshAssetsMap.find( _meshId ) != 
                     context.agentPtr->meshAssetsMap.end() )
                {
                    _kinTreeCollisionPtr->geometry.meshId = 
                        context.agentPtr->meshAssetsMap[ _meshId ]->name;

                    _kinTreeCollisionPtr->geometry.filename = 
                        context.agentPtr->meshAssetsMap[ _meshId ]->file;
                }
                else
                {
                    _kinTreeCollisionPtr->geometry.meshId   = "";
                    _kinTreeCollisionPtr->geometry.filename = _meshId;
                }
            }
        }

        // and the collision/geom size
        _extractStandardSize( urdfCollisionPtr->geometry, 
                              _kinTreeCollisionPtr->geometry.size );
        _kinTreeCollisionPtr->geometry.usesFromto = false;

        // and the contype collision bitmask
        _kinTreeCollisionPtr->contype = 1;
        // and the conaffinity collision bitmask
        _kinTreeCollisionPtr->conaffinity = 1;
        // and the condim contact dimensionality
        _kinTreeCollisionPtr->condim = 3;
        // and store it in the collisions buffer
        context.agentPtr->collisions.push_back( _kinTreeCollisionPtr );
        // and to the collisions map
        context.agentPtr->collisionsMap[ _kinTreeCollisionPtr->name ] = _kinTreeCollisionPtr;

        return _kinTreeCollisionPtr;
    }

    TKinTreeInertia* _processInertialFromUrdf( TUrdfParsingContext& context, 
                                               urdf::UrdfInertia* urdfInertiaPtr )
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

    void _collectAssetsFromLink( TUrdfParsingContext& context, 
                                  urdf::UrdfLink* urdfLinkPtr )
    {
        if ( !urdfLinkPtr )
            return;

        // grab the visuals buffer (extend from shapes, so might have assets)
        auto _visuals = urdfLinkPtr->visuals;
        // grab the collisions buffer as well
        auto _collisions = urdfLinkPtr->collisions;
        // and combine them into a single shapes buffer (to avoid repeating code)
        auto _shapes = std::vector< urdf::UrdfShape* >();
        for ( size_t i = 0; i < _visuals.size(); i++ )
            _shapes.push_back( _visuals[i] );
        for ( size_t i = 0; i < _collisions.size(); i++ )
            _shapes.push_back( _collisions[i] );

        // check for potential mesh assets in the "shapes" buffer
        for ( size_t i = 0; i < _shapes.size(); i++ )
        {
            // grab the geometry from the shape (visual|collision)
            auto _geometryPtr = _shapes[i]->geometry;

            // just a sanity check
            if ( !_geometryPtr )
                continue;

            // if not a mesh, well we're not in bussiness
            if ( _geometryPtr->type != "mesh" )
                continue;

            // grab the mesh data into a meshasset object
            auto _meshAsset = new TKinTreeMeshAsset();
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

            _meshAsset->name     = _fileName; // id is the name without extension
            _meshAsset->file     = _geometryPtr->filename; // filename if full path
            _meshAsset->scale    = _geometryPtr->size;

            if ( context.agentPtr->meshAssetsMap.find( _meshAsset->name ) ==
                 context.agentPtr->meshAssetsMap.end() )
            {
                // save in storage for later usage
                context.agentPtr->meshAssets.push_back( _meshAsset );
                context.agentPtr->meshAssetsMap[ _meshAsset->name ] = _meshAsset;
            }
            else
            {
                delete _meshAsset;
                _meshAsset = NULL;
            }
        }

        // repeat recursively for all child links
        for ( size_t i = 0; i < urdfLinkPtr->childLinks.size(); i++ )
        {
            _collectAssetsFromLink( context, urdfLinkPtr->childLinks[i] );
        }
    }

    void _constructDefaultActuators( TUrdfParsingContext& context )
    {
        for ( size_t q = 0; q < context.agentPtr->joints.size(); q++ )
        {
            if ( context.agentPtr->joints[q]->type == "free" ||
                 context.agentPtr->joints[q]->type == "fixed" ||
                 context.agentPtr->joints[q]->type == "world" )
            {
                continue;
            }

            auto _kinTreeActuatorPtr = new TKinTreeActuator();
            _kinTreeActuatorPtr->name = urdf::computeUrdfName( "actuator",
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
            _kinTreeActuatorPtr->gear = { 1, { 2.0f } };

            context.agentPtr->actuators.push_back( _kinTreeActuatorPtr );
            context.agentPtr->actuatorsMap[ _kinTreeActuatorPtr->name ] = _kinTreeActuatorPtr;
        }
    }

    void _extractStandardSize( urdf::UrdfGeometry* urdfGeometryPtr,
                               TVec3& targetSize )
    {
        // The sizes are given in standard form for urdf files, but ...
        // just in case we ran into some funky model definition we're ...
        // checkint the sizes either way

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


}}