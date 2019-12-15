
#include <compounds/compound.h>

namespace tysoc
{

    TCompound::TCompound( const std::string& name,
                          const TVec3& position,
                          const TMat3& rotation,
                          const eDynamicsType& dyntype )
    {
        m_name = name;
        m_tf = TMat4( position, rotation );
        m_dyntype = dyntype;

        m_rootBodyRef = nullptr;
        m_compoundImplRef = nullptr;
    }

    TCompound::~TCompound()
    {
        m_bodies.clear();
        m_bodiesMap.clear();

        m_rootBodyRef = nullptr;
        m_compoundImplRef = nullptr;
    }

    TCompoundBody* TCompound::createRootBody( const std::string& name,
                                              const TBodyData& bodyData,
                                              const TMat4& localTransform )
    {
        if ( m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::createRootBody() >>> compound already has a root body. Skipping creation" );
            return nullptr;
        }

        /* create root-body (use fixed|free joint constructor, as no joint constraint was given) */
        auto _rootCompoundBody = new TCompoundBody( name,
                                                    bodyData,
                                                    localTransform.getPosition(),
                                                    localTransform.getRotation(),
                                                    m_dyntype );

        /* take ownership of the root-body and cache it for easy access */
        m_bodies.push_back( std::move( std::unique_ptr< TCompoundBody >( _rootCompoundBody ) ) );
        m_bodiesMap[ _rootCompoundBody->name() ] = _rootCompoundBody;

        /* keep a reference of the root for easier access */
        m_rootBodyRef = _rootCompoundBody;

        /* return a reference for the user to play with */
        return _rootCompoundBody;
    }

    TCompoundBody* TCompound::createRootBody( const std::string& name,
                                              const eShapeType& shape,
                                              const TVec3& size,
                                              const TVec3& color,
                                              const TMat4& localTransform )
    {
        auto _bodyCollisionData = TCollisionData();
        _bodyCollisionData.type = shape;
        _bodyCollisionData.size = size;

        auto _bodyVisualData = TVisualData();
        _bodyVisualData.type = shape;
        _bodyVisualData.size = size;
        _bodyVisualData.ambient = color;
        _bodyVisualData.diffuse = color;
        _bodyVisualData.specular = color;
        _bodyVisualData.shininess = 64.0f;

        auto _bodyData = TBodyData();
        _bodyData.dyntype = m_dyntype;
        _bodyData.collision = _bodyCollisionData;
        _bodyData.visual = _bodyVisualData;

        return createRootBody( name, _bodyData, localTransform );
    }

    std::pair< TCompoundBody*, TJoint* > TCompound::createRootBodyJointPair( const std::string& bodyName,
                                                                             const TBodyData& bodyData,
                                                                             const TJointData& jointData,
                                                                             const TMat4& localTransform )
    {
        if ( m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::createRootBodyJointPair() >>> compound already has a root body. Skipping creation" );
            return { nullptr, nullptr };
        }

        /* create root-body using both body and joint information (check appropriate constructor) */
        auto _rootCompoundBody = new TCompoundBody( bodyName,
                                                    bodyData,
                                                    jointData,
                                                    localTransform.getPosition(),
                                                    localTransform.getRotation() );

        /* take ownership of the root-body and cache it for easy access */
        m_bodies.push_back( std::move( std::unique_ptr< TCompoundBody >( _rootCompoundBody ) ) );
        m_bodiesMap[ _rootCompoundBody->name() ] = _rootCompoundBody;

        /* keep a reference of the root for easier access */
        m_rootBodyRef = _rootCompoundBody;

        /* return body-joint references pair */
        return { _rootCompoundBody, _rootCompoundBody->joint() };
    }

    std::pair< TCompoundBody*, TJoint* > TCompound::createRootBodyJointPair( const std::string& bodyName,
                                                                             const eShapeType& bodyShape,
                                                                             const TVec3& bodySize,
                                                                             const TVec3& bodyColor,
                                                                             const TMat4& bodyLocalTransform,
                                                                             const eJointType& jointType,
                                                                             const TVec3& jointAxis,
                                                                             const TVec2& jointLimits,
                                                                             const TMat4& jointLocalTransform )
    {
        auto _bodyCollisionData = TCollisionData();
        _bodyCollisionData.type = bodyShape;
        _bodyCollisionData.size = bodySize;

        auto _bodyVisualData = TVisualData();
        _bodyVisualData.type = bodyShape;
        _bodyVisualData.size = bodySize;
        _bodyVisualData.ambient = bodyColor;
        _bodyVisualData.diffuse = bodyColor;
        _bodyVisualData.specular = bodyColor;
        _bodyVisualData.shininess = 64.0f;

        auto _bodyData = TBodyData();
        _bodyData.dyntype = m_dyntype;
        _bodyData.collision = _bodyCollisionData;
        _bodyData.visual = _bodyVisualData;

        auto _jointData = TJointData();
        _jointData.type = jointType;
        _jointData.axis = jointAxis;
        _jointData.limits = jointLimits;
        _jointData.localTransform = jointLocalTransform;

        return createRootBodyJointPair( bodyName, _bodyData, _jointData, bodyLocalTransform );
    }

    TCompoundBody* TCompound::addCompoundBody( std::unique_ptr< TCompoundBody > body )
    {
        if ( !body )
        {
            TYSOC_CORE_ERROR( "TCompound::addCompoundBody() >>> tried adding null body" );
            return nullptr;
        }

        if ( m_bodiesMap.find( body->name() ) != m_bodiesMap.end() )
        {
            TYSOC_CORE_ERROR( "TCompound::addCompoundBody() >> tried adding a body with the same name \"{0}\". Skipping addition", body->name() );
            return nullptr;
        }

        if ( !body->parent() && m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::addCompoundBody() >>> body \"{0}\" has no parent. Compounds can only have one root body. Skipping addition", body->name() );
            return nullptr;
        }

        auto _bodyRef = body.get();
        m_bodies.push_back( std::move( body ) );
        m_bodiesMap[ _bodyRef->name() ] = _bodyRef;

        if ( !_bodyRef->parent() && !m_rootBodyRef )
            m_rootBodyRef = _bodyRef;

        return _bodyRef;
    }

    void TCompound::update()
    {
        if ( !m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::update() >>> compound \"{0}\" has no root-body, so we can't\
                               do the traversal", m_name );
            return;
        }

        /* Update internal low-level state using the adapter (link to the backend) */
        if ( m_compoundImplRef )
        {
            // update the adapter to handle internal stuff
            m_compoundImplRef->update();

            // grab the latest world-transform from the backend
            m_compoundImplRef->getTransform( m_tf );
        }

        /* Send update-request to all bodies recursively */
        std::stack< TCompoundBody* > _bodiesToUpdate;
        _bodiesToUpdate.push( m_rootBodyRef );
        while ( _bodiesToUpdate.size() > 0 )
        {
            auto _body = _bodiesToUpdate.top();
            _bodiesToUpdate.pop();
            if ( _body )
            {
                _body->update();
                auto _children = _body->children();
                for ( auto _child : _children )
                    _bodiesToUpdate.push( _child );
            }
        }
    }

    void TCompound::reset()
    {
        if ( !m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::reset() >>> compound \"{0}\" has no root-body, so we can't\
                               do the traversal", m_name );
            return;
        }

        /* Reset internal low-level state using the adapter (link to the backend) */
        if ( m_compoundImplRef )
        {
            // reset to rest configuration using the adapter
            m_compoundImplRef->reset();

            // grab the latest world-transfrom from the backend
            m_compoundImplRef->getTransform( m_tf );
        }

        /* Send update-request to all bodies recursively */
        std::stack< TCompoundBody* > _bodiesToReset;
        _bodiesToReset.push( m_rootBodyRef );
        while ( _bodiesToReset.size() > 0 )
        {
            auto _body = _bodiesToReset.top();
            _bodiesToReset.pop();
            if ( _body )
            {
                _body->reset();
                auto _children = _body->children();
                for ( auto _child : _children )
                    _bodiesToReset.push( _child );
            }
        }
    }

    void TCompound::initializeToRestConfiguration()
    {
        if ( !m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::initializeToRestConfiguration() >>> compound \"{0}\" has \
                               no root-body, so we can't do the traversal", m_name );
            return;
        }

        if ( m_compoundImplRef )
        {
            TYSOC_CORE_ERROR( "TCompound::initializeToRestConfiguration() >>> compound \"{0}\"'s \
                               state' is handled by an adapter, so we shouldn't do the traversal", m_name );
            return;
        }

        _updateCurrentConfigurationRecursive( m_rootBodyRef, m_tf0 );
    }

    void TCompound::setPosition( const TVec3& position )
    {
        if ( !m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::setPosition() >>> compound \"{0}\" has no root-body. Skipping translation", m_name );
            return;
        }

        m_tf.setPosition( position );

        if ( m_compoundImplRef && m_compoundImplRef->active() )
            m_compoundImplRef->setPosition( position );
        else
            _updateCurrentConfigurationRecursive( m_rootBodyRef, m_tf );
    }

    void TCompound::setRotation( const TMat3& rotation )
    {
        if ( !m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::setRotation() >>> compound \"{0}\" has no root-body. Skipping rotation", m_name );
            return;
        }

        m_tf.setRotation( rotation );

        if ( m_compoundImplRef && m_compoundImplRef->active() )
            m_compoundImplRef->setRotation( rotation );
        else
            _updateCurrentConfigurationRecursive( m_rootBodyRef, m_tf );
    }

    void TCompound::setTransform( const TMat4& transform )
    {
        if ( !m_rootBodyRef )
        {
            TYSOC_CORE_ERROR( "TCompound::setTransform() >>> compound \"{0}\" has no root-body. Skipping world-transform update", m_name );
            return;
        }

        m_tf = transform;

        if ( m_compoundImplRef && m_compoundImplRef->active() )
            m_compoundImplRef->setTransform( transform );
        else
            _updateCurrentConfigurationRecursive( m_rootBodyRef, m_tf );
    }

    TCompoundBody* TCompound::getBodyByName( const std::string& name )
    {
        if ( m_bodiesMap.find( name ) == m_bodiesMap.end() )
        {
            TYSOC_CORE_ERROR( "TCompound::getBodyByName() >>> couldn't find body \"{0}\" in \
                               compound \"{1}\". Returning nullptr instead", name, m_name );
            return nullptr;
        }

        return m_bodiesMap[name];
    }

    std::vector< TCompoundBody* > TCompound::bodies()
    {
        std::vector< TCompoundBody* > _bodiesFound;
        for ( auto& _body : m_bodies )
            _bodiesFound.push_back( _body.get() );

        return _bodiesFound;
    }

    void TCompound::_updateCurrentConfigurationRecursive( TCompoundBody* body, const TMat4& parentWorldTf )
    {
        if ( !body )
            return;

        /* compute body's world-transform (components like col, vis, etc. are updated internally by the body) */
        auto _bodyWorldTf = parentWorldTf * body->localTf();
        body->setTransform( _bodyWorldTf );

        /* traverse the children in a similar fashion */
        auto _childrenRefs = body->children();
        for ( auto _child : _childrenRefs )
            _updateCurrentConfigurationRecursive( _child, _bodyWorldTf );
    }

}