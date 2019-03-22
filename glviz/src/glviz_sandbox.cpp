
#include <glviz_sandbox.h>

namespace tysoc {
namespace viz {

    TGLVizSandbox::TGLVizSandbox( const std::vector< sandbox::TBody* >& bodies,
                                  engine::LScene* scenePtr,
                                  const std::string& workingDir )
    {
        m_scenePtr = scenePtr;
        m_workingDir = workingDir;

        _collectBodies( bodies );
    }

    TGLVizSandbox::~TGLVizSandbox()
    {
        m_scenePtr = NULL;

        for ( size_t i = 0; i < m_vizBodies.size(); i++ )
        {
            m_vizBodies[i].bodyPtr          = NULL;
            m_vizBodies[i].axesPtr          = NULL;
            m_vizBodies[i].renderablePtr    = NULL;
        }
        m_vizBodies.clear();

        for ( size_t i = 0; i < m_vizJoints.size(); i++ )
        {
            m_vizJoints[i].jointPtr         = NULL;
            m_vizJoints[i].axesPtr          = NULL;
            m_vizJoints[i].renderablePtr    = NULL;
        }
        m_vizJoints.clear();

    }

    void TGLVizSandbox::_collectBodies( const std::vector< sandbox::TBody* >& bodies )
    {
        for ( size_t i = 0; i < bodies.size(); i++ )
        {
            TGLVizSandboxBody _vizBody;
            // wrap the body object
            _vizBody.bodyPtr = bodies[i];
            // and create the appropriate renderable
            _vizBody.renderablePtr = _createRenderable( bodies[i]->type,
                                                        bodies[i]->size,
                                                        bodies[i]->color,
                                                        bodies[i]->color,
                                                        bodies[i]->color,
                                                        bodies[i]->filename );

            // and create the frame axes
            _vizBody.axesPtr = engine::LMeshBuilder::createAxes( VIZSANDBOX_AXES_DEFAULT_SIZE );
            // and add the related axes to the scene
            m_scenePtr->addRenderable( _vizBody.axesPtr );
            // and add it to the list of bodies viz-wrappers
            m_vizBodies.push_back( _vizBody );

            // Collect the joints(dofs) that this body has
            auto _joints = bodies[i]->joints;
            for ( size_t j = 0; j < _joints.size(); j++ )
            {
                TGLVizSandboxJoint _vizJoint;
                // wrap the joint object
                _vizJoint.jointPtr = _joints[j];
                // and create the appropriate renderable
                _vizJoint.renderablePtr = _createRenderable( "cylinder",
                                                             { 0.0125f, 0.025f, 0.0f },
                                                             { 0.1f, 0.1f, 0.85f },
                                                             { 0.1f, 0.1f, 0.85f },
                                                             { 0.1f, 0.1f, 0.85f },
                                                             "" );

                // and create the frame axes
                _vizJoint.axesPtr = engine::LMeshBuilder::createAxes( VIZSANDBOX_AXES_DEFAULT_SIZE );
                // and add the related axes to the scene
                m_scenePtr->addRenderable( _vizJoint.axesPtr );
                // and add it to the list of joints viz-wrappers
                m_vizJoints.push_back( _vizJoint );
            }

            // Collect the child bodies that this body has
            if ( bodies[i]->bodies.size() > 0 )
                _collectBodies( bodies[i]->bodies );
        }
    }

    engine::LIRenderable* TGLVizSandbox::_createRenderable( const std::string& type,
                                                            const TVec3& size,
                                                            const TVec3& cAmbient,
                                                            const TVec3& cDiffuse,
                                                            const TVec3& cSpecular,
                                                            const std::string& filename )
    {
        engine::LIRenderable* _renderable = NULL;

        if ( type == "box" )
        {
            _renderable = engine::LMeshBuilder::createBox( size.x,
                                                           size.y,
                                                           size.z );
        }
        else if ( type == "sphere" )
        {
            _renderable = engine::LMeshBuilder::createSphere( size.x );
        }
        else if ( type == "capsule" )
        {
            _renderable = engine::LMeshBuilder::createCapsule( size.x, size.y );
        }
        else if ( type == "cylinder" )
        {
            _renderable = engine::LMeshBuilder::createCylinder( size.x, size.y );
        }
        else if ( type == "mesh" )
        {
            auto _meshFilePath = m_workingDir + filename;
            _renderable = engine::LMeshBuilder::createModelFromFile( _meshFilePath,
                                                                     "" );
        }
        else if ( type == "plane" )
        {
            _renderable = engine::LMeshBuilder::createPlane( size.x, size.y );
        }

        if ( _renderable )
        {
            _setRenderableColor( _renderable, cAmbient, cDiffuse, cSpecular );

            m_scenePtr->addRenderable( _renderable );
        }
        else
        {
            std::cout << "WARNING> could not create mesh of type: " << type << std::endl;
            if ( type == "mesh" )
            {
                std::cout << "WARNING> filename of mesh: " << filename << std::endl;
            }
        }

        return _renderable;
    }

    void TGLVizSandbox::_setRenderableColor( engine::LIRenderable* renderablePtr,
                                             const TVec3& cAmbient,
                                             const TVec3& cDiffuse,
                                             const TVec3& cSpecular )
    {
        // and update the color as well
        if ( renderablePtr->getType() != RENDERABLE_TYPE_MODEL )
        {
            renderablePtr->getMaterial()->ambient   = { cAmbient.x, cAmbient.y, cAmbient.z };
            renderablePtr->getMaterial()->diffuse   = { cDiffuse.x, cDiffuse.y, cDiffuse.z };
            renderablePtr->getMaterial()->specular  = { cSpecular.x, cSpecular.y, cSpecular.z };
        }
        else
        {
            auto _children = reinterpret_cast< engine::LModel* >( renderablePtr )->getMeshes();
            for ( size_t i = 0; i < _children.size(); i++ )
            {
                _children[i]->getMaterial()->ambient   = { cAmbient.x, cAmbient.y, cAmbient.z };
                _children[i]->getMaterial()->diffuse   = { cDiffuse.x, cDiffuse.y, cDiffuse.z };
                _children[i]->getMaterial()->specular  = { cSpecular.x, cSpecular.y, cSpecular.z };
            }
        }
    }

    void TGLVizSandbox::update()
    {
        for ( size_t i = 0; i < m_vizBodies.size(); i++ )
            _updateBody( m_vizBodies[i] );

        for ( size_t i = 0; i < m_vizJoints.size(); i++ )
            _updateJoint( m_vizJoints[i] );
    }

    void TGLVizSandbox::_updateBody( TGLVizSandboxBody& body )
    {
        if ( !body.bodyPtr )
            return;

        if ( !body.renderablePtr )
            return;

        if ( !body.axesPtr )
            return;

        body.renderablePtr->setVisibility( drawState.showBodies );
        body.renderablePtr->setWireframeMode( drawState.wireframe );
        body.axesPtr->setVisibility( drawState.frameAxes && drawState.showBodies );

        engine::LVec3 _pos = fromTVec3( body.bodyPtr->worldTransform.getPosition() );
        engine::LMat4 _rot = fromTMat3( body.bodyPtr->worldTransform.getRotation() );

        body.renderablePtr->pos = _pos;
        body.renderablePtr->rotation = _rot;

        body.axesPtr->pos = _pos;
        body.axesPtr->rotation = _rot;

        body.trail.append( _pos );

        engine::DebugSystem::drawTrailPoints( body.trail.points() );
    }

    void TGLVizSandbox::_updateJoint( TGLVizSandboxJoint& joint )
    {
        if ( !joint.jointPtr )
            return;

        if ( !joint.renderablePtr )
            return;

        if ( !joint.axesPtr )
            return;

        joint.renderablePtr->setVisibility( drawState.showJoints );
        joint.axesPtr->setVisibility( drawState.frameAxes && drawState.showJoints );

        engine::LVec3 _pos = fromTVec3( joint.jointPtr->worldTransform.getPosition() );
        engine::LMat4 _rot = fromTMat3( joint.jointPtr->worldTransform.getRotation() );

        joint.renderablePtr->pos = _pos;
        joint.renderablePtr->rotation = _rot;

        joint.axesPtr->pos = _pos;
        joint.axesPtr->rotation = _rot;

        auto _axisInWorld = joint.jointPtr->worldTransform.getRotation() * joint.jointPtr->axis;
        auto _axw1 = joint.jointPtr->worldTransform.getPosition() - _axisInWorld * 0.1;
        auto _axw2 = joint.jointPtr->worldTransform.getPosition() + _axisInWorld * 0.1;

        engine::DebugSystem::drawArrow( { _axw1.x, _axw1.y, _axw1.z }, 
                                        { _axw2.x, _axw2.y, _axw2.z }, 
                                        { 0.5f, 0.7f, 0.9f } );
    }


}}