
#include <compounds/compound_body.h>

namespace tysoc {

    TCompoundBody::TCompoundBody( const std::string& name,
                                  const TBodyData& data,
                                  const TJointData& jointData,
                                  TCompoundBody* parentRef,
                                  const TVec3& localPosition,
                                  const TMat3& localRotation )
        : TIBody( name, data )
    {
        /* keep a reference to the parent */
        m_parentRef = parentRef;
        if ( !parentRef )
            TYSOC_CORE_ERROR( "Compound body >>> created child compound-body without a parent-reference" );

        /* create and keep ownership of the joint */
        m_joint = std::unique_ptr< TJoint >( new TJoint( m_name + "_joint", jointData ) );
        m_joint->setOwnerBody( this );
        m_joint->setOwnerParent( m_parentRef );

        /* configure initial and current local-space pose */
        m_localPos = m_localPos0 = localPosition;
        m_localRot = m_localRot0 = localRotation;
        m_localTf.setPosition( m_localPos );
        m_localTf.setRotation( m_localRot );
        m_localTf0.setPosition( m_localPos0 );
        m_localTf0.setRotation( m_localRot0 );

        /* configure initial and current world-space pose w.r.t. parent's world space */
        m_tf = m_parentRef->tf() * m_localTf;
        m_tf0 = m_parentRef->tf0() * m_localTf0;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();
        m_pos0 = m_tf0.getPosition();
        m_rot0 = m_tf0.getRotation();
    }

    TCompoundBody::TCompoundBody( const std::string& name,
                                  const TBodyData& data,
                                  const TJointData& jointData,
                                  const TVec3& position,
                                  const TMat3& rotation )
        : TIBody( name, data )
    {
        /* this is a root body, so no parent-reference */
        m_parentRef = nullptr;

        /* create and keep ownership of the joint */
        m_joint = std::unique_ptr< TJoint >( new TJoint( m_name + "_joint", jointData ) );
        m_joint->setOwnerBody( this );
        m_joint->setOwnerParent( nullptr );

        /* configure initial and current world-space pose */
        m_pos = m_pos0 = position;
        m_rot = m_rot0 = rotation;
        m_tf.setPosition( m_pos );
        m_tf.setRotation( m_rot );
        m_tf0.setPosition( m_pos0 );
        m_tf0.setRotation( m_rot0 );

        /* configure initial and current local-space pose with defaults */
        m_localPos = m_localPos0 = tysoc::TVec3(); // zeros
        m_localRot = m_localRot0 = tysoc::TMat3(); // identity
        m_localTf = m_localTf0 = tysoc::TMat4(); // identity
    }

    TCompoundBody::TCompoundBody( const std::string& name,
                                  const TBodyData& data,
                                  const TVec3& position,
                                  const TMat3& rotation,
                                  const eDynamicsType& dyntype )
        : TIBody( name, data )
    {
        /* this is a root body, so no parent-reference */
        m_parentRef = nullptr;

        /* create and keep ownership of the joint */
        TJointData _jointData;
        _jointData.type = ( dyntype == eDynamicsType::STATIC ) ? eJointType::FIXED : eJointType::FREE;
        _jointData.localTransform.setIdentity();
        m_joint = std::unique_ptr< TJoint >( new TJoint( m_name + "_joint", _jointData ) );
        m_joint->setOwnerBody( this );
        m_joint->setOwnerParent( nullptr );

        /* configure initial and current world-space pose */
        m_pos = m_pos0 = position;
        m_rot = m_rot0 = rotation;
        m_tf.setPosition( m_pos );
        m_tf.setRotation( m_rot );
        m_tf0.setPosition( m_pos0 );
        m_tf0.setRotation( m_rot0 );

        /* configure initial and current local-space pose with defaults */
        m_localPos = m_localPos0 = tysoc::TVec3(); // zeros
        m_localRot = m_localRot0 = tysoc::TMat3(); // identity
        m_localTf = m_localTf0 = tysoc::TMat4(); // identity
    }

    TCompoundBody::~TCompoundBody()
    {
        m_parentRef = nullptr;
        m_joint = nullptr;
    }

    void TCompoundBody::setParent( TCompoundBody* parentRef )
    {
        // @experimental: allows to dynamically modify chains, which might cause breaks
        /* keep the reference to the parent body (user should handle deletion if needed) */
        m_parentRef = parentRef;
    }

    void TCompoundBody::setJoint( std::unique_ptr< TJoint > jointObj )
    {
        // @experimental: allows to dynamically modify chains, which might cause breaks
        /* keep ownership of the joint */
        m_joint = std::move( jointObj );
    }

    void TCompoundBody::update()
    {
        TIBody::update();

        if ( m_joint )
            m_joint->update();
    }

    void TCompoundBody::reset()
    {
        TIBody::reset();

        if ( m_joint )
        {
            m_joint->reset();
            m_joint->update();
        }
    }

    void TCompoundBody::setPosition( const TVec3& position )
    {
        /* set world-space position normally (the local-transform w.r.t. parent might have changed) */
        TIBody::setPosition( position );

        /* update joint if applicable */
        if ( m_joint )
            m_joint->update();

        /* update local-transform w.r.t. parent body if this is a child body */
        if ( m_parentRef )
        {
            m_localTf = m_parentRef->tf().inverse() * m_tf;
            m_localPos = m_localTf.getPosition();
            m_localRot = m_localTf.getRotation();
        }
    }

    void TCompoundBody::setRotation( const TMat3& rotation )
    {
        /* set world-space rotation-matrix normally (the local-transform w.r.t. parent might have changed) */
        TIBody::setRotation( rotation );

        /* update joint if applicable */
        if ( m_joint )
            m_joint->update();

        /* update local-transform w.r.t. parent body if this is a child body */
        if ( m_parentRef )
        {
            m_localTf = m_parentRef->tf().inverse() * m_tf;
            m_localPos = m_localTf.getPosition();
            m_localRot = m_localTf.getRotation();
        }
    }

    void TCompoundBody::setEuler( const TVec3& euler )
    {
        /* set world-space euler-angles normally (the local-transform w.r.t. parent might have changed) */
        TIBody::setEuler( euler );

        /* update joint if applicable */
        if ( m_joint )
            m_joint->update();

        /* update local-transform w.r.t. parent body if this is a child body */
        if ( m_parentRef )
        {
            m_localTf = m_parentRef->tf().inverse() * m_tf;
            m_localPos = m_localTf.getPosition();
            m_localRot = m_localTf.getRotation();
        }
    }

    void TCompoundBody::setQuaternion( const TVec4& quat )
    {
        /* set world-space quaternion normally (the local-transform w.r.t. parent might have changed) */
        TIBody::setQuaternion( quat );

        /* update joint if applicable */
        if ( m_joint )
            m_joint->update();

        /* update local-transform w.r.t. parent body if this is a child body */
        if ( m_parentRef )
        {
            m_localTf = m_parentRef->tf().inverse() * m_tf;
            m_localPos = m_localTf.getPosition();
            m_localRot = m_localTf.getRotation();
        }
    }

    void TCompoundBody::setTransform( const TMat4& transform )
    {
        /* set world-space transform normally (the local-transform w.r.t. parent might have changed) */
        TIBody::setTransform( transform );

        /* update joint if applicable */
        if ( m_joint )
            m_joint->update();

        /* update local-transform w.r.t. parent body if this is a child body */
        if ( m_parentRef )
        {
            m_localTf = m_parentRef->tf().inverse() * m_tf;
            m_localPos = m_localTf.getPosition();
            m_localRot = m_localTf.getRotation();
        }
    }

    void TCompoundBody::setLocalPosition( const TVec3& localPosition )
    {
        if ( !m_parentRef )
        {
            TYSOC_CORE_WARN( "Compound Body >>> tried setting relative position of a root\
                              compound-body, use \"setPosition\" instead" );
            return;
        }

        /* set the local-space position */
        TIBody::setLocalPosition( localPosition );

        /* update the joint (relative position w.r.t. owner might have changed) */
        if ( m_joint )
            m_joint->update();
    }

    void TCompoundBody::setLocalRotation( const TMat3& localRotation )
    {
        if ( !m_parentRef )
        {
            TYSOC_CORE_WARN( "Compound Body >>> tried setting relative rotation matrix of a root\
                              compound-body, use \"setRotation\" instead" );
            return;
        }

        /* set the local-space position */
        TIBody::setLocalRotation( localRotation );

        /* update the joint (relative position w.r.t. owner might have changed) */
        if ( m_joint )
            m_joint->update();
    }

    void TCompoundBody::setLocalEuler( const TVec3& localEuler )
    {
        if ( !m_parentRef )
        {
            TYSOC_CORE_WARN( "Compound Body >>> tried setting relative rotation-euler of a root\
                              compound-body, use \"setEuler\" instead" );
            return;
        }

        /* set the local-space position */
        TIBody::setLocalEuler( localEuler );

        /* update the joint (relative position w.r.t. owner might have changed) */
        if ( m_joint )
            m_joint->update();
    }

    void TCompoundBody::setLocalQuaternion( const TVec4& localQuat )
    {
        if ( !m_parentRef )
        {
            TYSOC_CORE_WARN( "Compound Body >>> tried setting world-space rotation-quaternion of a\
                              root compound-body, use \"setQuaternion\" instead" );
            return;
        }

        /* set the local-space position */
        TIBody::setLocalQuaternion( localQuat );

        /* update the joint (relative position w.r.t. owner might have changed) */
        if ( m_joint )
            m_joint->update();
    }

    void TCompoundBody::setLocalTransform( const TMat4& localTransform )
    {
        if ( !m_parentRef )
        {
            TYSOC_CORE_WARN( "Compound Body >>> tried setting relative transform of a root\
                              compound-body, use \"setTransform\" instead" );
            return;
        }

        /* set the local-space position */
        TIBody::setLocalTransform( localTransform );

        /* update the joint (relative position w.r.t. owner might have changed) */
        if ( m_joint )
            m_joint->update();
    }

}