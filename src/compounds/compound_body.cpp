
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
        /* define some defaults */
        m_parentRef = nullptr;
        m_joint = nullptr;
        m_compoundRef = nullptr;

        /* keep a reference to the parent (if no parent, notify that this is the wrong constructor to use) */
        if ( !parentRef )
        {
            TYSOC_CORE_ERROR( "TCompoundBody::TCompoundBody() >>> no parent-reference given for body \"{0}\". \
                               If you wanted a root-body, use another constructor", name );
            return;
        }
        m_parentRef = parentRef;
        m_parentRef->_addChildRef( this );

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
    }

    TCompoundBody::TCompoundBody( const std::string& name,
                                  const TBodyData& data,
                                  const TJointData& jointData,
                                  const TVec3& localPosition,
                                  const TMat3& localRotation )
        : TIBody( name, data )
    {
        /* define some defaults */
        m_parentRef = nullptr;
        m_joint = nullptr;
        m_compoundRef = nullptr;

        /* this is a root body, so no parent-reference */
        m_parentRef = nullptr;

        /* create and keep ownership of the joint */
        m_joint = std::unique_ptr< TJoint >( new TJoint( m_name + ":/joint", jointData ) );
        m_joint->setOwnerBody( this );
        m_joint->setOwnerParent( nullptr );

        /* configure initial and current local-space pose */
        m_localPos = m_localPos0 = localPosition;
        m_localRot = m_localRot0 = localRotation;
        m_localTf.setPosition( m_localPos );
        m_localTf.setRotation( m_localRot );
        m_localTf0.setPosition( m_localPos0 );
        m_localTf0.setRotation( m_localRot0 );
    }

    TCompoundBody::TCompoundBody( const std::string& name,
                                  const TBodyData& data,
                                  const TVec3& localPosition,
                                  const TMat3& localRotation,
                                  const eDynamicsType& dyntype )
        : TIBody( name, data )
    {
        /* define some defaults */
        m_parentRef = nullptr;
        m_joint = nullptr;
        m_compoundRef = nullptr;

        /* this is a root body, so no parent-reference */
        m_parentRef = nullptr;

        /* create and keep ownership of the joint */
        TJointData _jointData;
        _jointData.type = ( dyntype == eDynamicsType::STATIC ) ? eJointType::FIXED : eJointType::FREE;
        _jointData.localTransform.setIdentity();
        m_joint = std::unique_ptr< TJoint >( new TJoint( m_name + ":/joint", _jointData ) );
        m_joint->setOwnerBody( this );
        m_joint->setOwnerParent( nullptr );

        /* configure initial and current local-space pose */
        m_localPos = m_localPos0 = localPosition;
        m_localRot = m_localRot0 = localRotation;
        m_localTf.setPosition( m_localPos );
        m_localTf.setRotation( m_localRot );
        m_localTf0.setPosition( m_localPos0 );
        m_localTf0.setRotation( m_localRot0 );
    }

    TCompoundBody::~TCompoundBody()
    {
        m_parentRef = nullptr;
        m_joint = nullptr;
        m_compoundRef = nullptr;
    }

    void TCompoundBody::setCompound( TCompound* compoundRef )
    {
        /* keep the reference to the compound owner of this body */
        m_compoundRef = compoundRef;
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

    std::pair< TCompoundBody*, TJoint* > TCompoundBody::addBodyJointPair( const std::string& name,
                                                                          const TBodyData& bodyData,
                                                                          const TJointData& jointData,
                                                                          const TMat4& localTransform )
    {
        if ( !m_compoundRef )
        {
            TYSOC_CORE_ERROR( "TCompoundBody::addBodyJointPair() >>> body \"{0}\" must have a valid\
                               reference to the compound that owns it before adding more bodies to the compound.\
                               Skipping addition of body \"{1}\" and returning (nullptr,nullptr) instead", m_name, name );
            return { nullptr, nullptr };
        }

        auto _childBody = new TCompoundBody( name,
                                             bodyData,
                                             jointData,
                                             this,
                                             localTransform.getPosition(),
                                             localTransform.getRotation() );
        m_childrenRefs.push_back( _childBody );
        m_compoundRef->addCompoundBody( std::unique_ptr< TCompoundBody >( _childBody ) );

        return { _childBody, _childBody->joint() };
    }

    std::pair< TCompoundBody*, TJoint* > TCompoundBody::addBodyJointPair( const std::string& name,
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
        _bodyData.dyntype = eDynamicsType::DYNAMIC;
        _bodyData.collision = _bodyCollisionData;
        _bodyData.visual = _bodyVisualData;

        auto _jointData = TJointData();
        _jointData.type = jointType;
        _jointData.axis = jointAxis;
        _jointData.limits = jointLimits;
        _jointData.localTransform = jointLocalTransform;

        return addBodyJointPair( name, _bodyData, _jointData, bodyLocalTransform );
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
            TYSOC_CORE_WARN( "TCompoundBody::setLocalPosition() >>> tried setting relative position of \
                              root compound-body \"{0}\", use \"setPosition\" instead", m_name );
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
            TYSOC_CORE_WARN( "TCompoundBody::setLocalRotation() >>> tried setting relative rotation matrix\
                              of root compound-body \"{0}\", use \"setRotation\" instead", m_name );
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
            TYSOC_CORE_WARN( "TCompoundBody::setLocalEuler() >>> tried setting relative rotation-euler\
                              of root compound-body \"{0}\", use \"setEuler\" instead", m_name );
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
            TYSOC_CORE_WARN( "TCompoundBody::setLocalQuaternion() >>> tried setting world-space rotation-quaternion\
                               of root compound-body \"{0}\", use \"setQuaternion\" instead", m_name );
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
            TYSOC_CORE_WARN( "TCompoundBody::setLocalTransform() >>> tried setting relative transform\
                               of root compound-body \"{0}\", use \"setTransform\" instead", m_name );
            return;
        }

        /* set the local-space position */
        TIBody::setLocalTransform( localTransform );

        /* update the joint (relative position w.r.t. owner might have changed) */
        if ( m_joint )
            m_joint->update();
    }


    void TCompoundBody::_addChildRef( TCompoundBody* childRef )
    {
        /* keep the reference to a child-body in a compound */
        m_childrenRefs.push_back( childRef );
    }

}