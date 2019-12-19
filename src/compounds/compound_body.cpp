
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

        /* make sure our type is compound-body */
        m_classType = eBodyClassType::COMPOUND_BODY;

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
        m_joint = std::unique_ptr< TJoint >( new TJoint( m_name + ":/joint", jointData ) );
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

        /* make sure our type is compound-body */
        m_classType = eBodyClassType::COMPOUND_BODY;

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

        /* make sure our type is compound-body */
        m_classType = eBodyClassType::COMPOUND_BODY;

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

    void TCompoundBody::preStep()
    {
        if ( m_bodyImplRef && m_bodyImplRef->active() )
            m_bodyImplRef->preStep();

        /* update all internal components required prior to take a simulation step */
        _preStepComponents();
    }

    void TCompoundBody::postStep()
    {
        /* Update|grab the state of the body either from the adapter (if available and 
           active) or from the last configuration (either initial or simulated) */
        if ( m_bodyImplRef && m_bodyImplRef->active() )
        {
            // update the adapter to handle internal stuff
            m_bodyImplRef->postStep();

            // grab the latest world-transform from the backend
            m_bodyImplRef->getPosition( m_pos );
            m_bodyImplRef->getRotation( m_rot );
            m_bodyImplRef->getTransform( m_tf );
        }
        else
        {
            auto _parentWorldTf = TMat4();
            if ( m_parentRef )
                _parentWorldTf = m_parentRef->tf();
            else
                _parentWorldTf = m_compoundRef->tf();

            m_tf = _parentWorldTf * m_localTf;
            m_pos = m_tf.getPosition();
            m_rot = m_tf.getRotation();
        }

        /* update all internal components after the simulation step was taken */
        _postStepComponents();
    }

    void TCompoundBody::reset()
    {
        /* Request a reset of the internal state of the body either from the adapter (if available
           and active) or from the initial configuration */
        if ( m_bodyImplRef && m_bodyImplRef->active() )
        {
            // request a reset of the adapter's internal resources
            m_bodyImplRef->reset();

            // grab the latest world-transform from the backend
            m_bodyImplRef->getPosition( m_pos );
            m_bodyImplRef->getRotation( m_rot );
            m_bodyImplRef->getTransform( m_tf );
        }
        else
        {
            auto _parentWorldTf = TMat4();
            if ( m_parentRef )
                _parentWorldTf = m_parentRef->tf0();
            else
                _parentWorldTf = m_compoundRef->tf0();

            m_tf = _parentWorldTf * m_localTf0;
            m_pos = m_tf.getPosition();
            m_rot = m_tf.getRotation();
        }

        /* reset and update all internal components */
        _resetComponents();
        _postStepComponents();
    }

    void TCompoundBody::setPosition( const TVec3& position )
    {
        /* update world-position and world-transform (as in the base class) */
        m_pos = position;
        m_tf.setPosition( m_pos );

        /* update local-transform w.r.t. parent body if this is a child body */
        _updateLocalTransform();

        /* request adapter to handle the changes (as in the base class) */
        if ( m_bodyImplRef )
            m_bodyImplRef->setPosition( m_pos );

        /* update all internal components */
        _postStepComponents();

        /* update the state of our children */
        for ( auto _child : m_childrenRefs )
            _child->postStep();
    }

    void TCompoundBody::setRotation( const TMat3& rotation )
    {
        /* update world-rotation and world-transform (as in the base class) */
        m_rot = rotation;
        m_tf.setRotation( m_rot );

        /* update local-transform w.r.t. parent body if this is a child body */
        _updateLocalTransform();

        /* request adapter to handle the changes (as in the base class) */
        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( m_rot );

        /* update all internal components */
        _postStepComponents();

        /* update the state of our children */
        for ( auto _child : m_childrenRefs )
            _child->postStep();
    }

    void TCompoundBody::setEuler( const TVec3& euler )
    {
        /* update world-rotation and world-transform (as in the base class) */
        m_rot = TMat3::fromEuler( euler );
        m_tf.setRotation( m_rot );

        /* update local-transform w.r.t. parent body if this is a child body */
        _updateLocalTransform();

        /* request adapter to handle the changes (as in the base class) */
        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( m_rot );

        /* update all internal components */
        _postStepComponents();

        /* update the state of our children */
        for ( auto _child : m_childrenRefs )
            _child->postStep();
    }

    void TCompoundBody::setQuaternion( const TVec4& quat )
    {
        /* update world-rotation and world-transform (as in the base class) */
        m_rot = TMat3::fromQuaternion( quat );
        m_tf.setRotation( m_rot );

        /* update local-transform w.r.t. parent body if this is a child body */
        _updateLocalTransform();

        /* request adapter to handle the changes (as in the base class) */
        if ( m_bodyImplRef )
            m_bodyImplRef->setRotation( m_rot );

        /* update all internal components */
        _postStepComponents();

        /* update the state of our children */
        for ( auto _child : m_childrenRefs )
            _child->postStep();
    }

    void TCompoundBody::setTransform( const TMat4& transform )
    {
        /* update world-transform (as in the base class) */
        m_tf = transform;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();

        /* update local-transform w.r.t. parent body if this is a child body */
        _updateLocalTransform();

        /* request adapter to handle the changes (as in the base class) */
        if ( m_bodyImplRef )
            m_bodyImplRef->setTransform( m_tf );

        /* update all internal components */
        _postStepComponents();

        /* update the state of our children */
        for ( auto _child : m_childrenRefs )
            _child->postStep();
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
            m_joint->postStep();
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
            m_joint->postStep();
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
            m_joint->postStep();
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
            m_joint->postStep();
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
            m_joint->postStep();
    }

    void TCompoundBody::_addChildRef( TCompoundBody* childRef )
    {
        /* keep the reference to a child-body in a compound */
        m_childrenRefs.push_back( childRef );
    }

    void TCompoundBody::_updateLocalTransform()
    {
        /* Update localTf of this body w.r.t. parent body, or compound if no-parent (root) */
        if ( m_parentRef )
            m_localTf = m_parentRef->tf().inverse() * m_tf;
        else if ( m_compoundRef )
            m_localTf = m_compoundRef->tf().inverse() * m_tf;
        else
            TYSOC_CORE_ERROR( "TCompoundBody::_updateLocalTransform() >>> body \"{0}\" does not have \
                               a valid compound reference", m_name );

        m_localPos = m_localTf.getPosition();
        m_localRot = m_localTf.getRotation();
    }

    void TCompoundBody::_preStepComponents()
    {
        if ( m_collision )
            m_collision->preStep();

        if ( m_visual )
            m_visual->preStep();

        if ( m_joint )
            m_joint->preStep();
    }

    void TCompoundBody::_postStepComponents()
    {
        if ( m_collision )
            m_collision->postStep();

        if ( m_visual )
            m_visual->postStep();

        if ( m_joint )
            m_joint->postStep();
    }

    void TCompoundBody::_resetComponents()
    {
        if ( m_collision )
            m_collision->reset();

        if ( m_visual )
            m_visual->reset();

        if ( m_joint )
            m_joint->reset();
    }

}