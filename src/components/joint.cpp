
#include <components/joint.h>

namespace tysoc {

    TJoint::TJoint( const std::string& name, const TJointData& jointData )
    {
        m_name = name;
        m_data = jointData;

        m_jointImplRef = nullptr;
        m_drawableImplRef = nullptr;
        m_ownerRef = nullptr;
        m_ownerParentRef = nullptr;

        /* initialze defaults for degrees of freedom */
        m_nqpos = 0;
        m_nqvel = 0;
        m_qpos.fill( 0.0f ); m_qpos0.fill( 0.0f );
        m_qvel.fill( 0.0f ); m_qvel0.fill( 0.0f );

        /* grab local-transform w.r.t. owner body */
        m_localTf = jointData.localTransform;
        m_localPos = m_localTf.getPosition();
        m_localRot = m_localTf.getRotation();

        /* Configure joint nqs (number of generalized coordinates and degrees of freedom). 
           According to the backend, this functionality might be implemented with maximal
           instead of minimal coordinates (bullet constraints without btMultibody), so in
           those cases, we'd have to transform these quantities to the appropriate representation
           expected by the backend API */
        auto _jointType = m_data.type;
        if ( _jointType == eJointType::REVOLUTE )
        {
            // revolute joints have just 1dof, and 1qpos for this dof
            m_data.nqpos = m_nqpos = 1;
            m_data.nqvel = m_nqvel = 1;
            // qpos and qvel set to zero by default
            m_qpos[0] = m_qpos0[0] = 0.0f;
            m_qvel[0] = m_qvel0[0] = 0.0f;
        }
        else if ( _jointType == eJointType::PRISMATIC )
        {
            // prismatic joints have just 1dof, and 1qpos for this dof
            m_data.nqpos = m_nqpos = 1;
            m_data.nqvel = m_nqvel = 1;
            // qpos and qvel set to zero by default
            m_qpos[0] = m_qpos0[0] = 0.0f;
            m_qvel[0] = m_qvel0[0] = 0.0f;
        }
        else if ( _jointType == eJointType::SPHERICAL )
        {
            // spherical joints have 3dofs, and 4qpos (quaternion) representing it
            m_data.nqpos = m_nqpos = 4;
            m_data.nqvel = m_nqvel = 3;
            // qpos and qvel set to "zero" by default
            //        qx  qy  qz  qw   --unused--
            m_qpos = { 0., 0., 0., 1. }; // zero-quaternion
            m_qpos0 = { 0., 0., 0., 1. }; // zero-quaternion
            //        ex  ey  ez  ---unused---
            m_qvel = { 0., 0., 0. }; // zero velocity
            m_qvel0 = { 0., 0., 0. }; // zero velocity
        }
        else if ( _jointType == eJointType::FREE )
        {
            // free joints have 6dofs, and 7qpos (quat + xyz) representing it
            m_data.nqpos = m_nqpos = 7;
            m_data.nqvel = m_nqvel = 6;
            // qpos and qvel set to "zero" by default
            //         x   y   z  qx  qy  qz  qw
            m_qpos = { 0., 0., 0., 0., 0., 0., 1. }; // zero-xyz + zero-quat
            m_qpos0 = { 0., 0., 0., 0., 0., 0., 1. }; // zero-xyz + zero-quat
            //         x   y   z   ex  ey  ez
            m_qvel = { 0., 0., 0., 0., 0., 0. }; // zero-xyz + zero-euler? velocity
            m_qvel0 = { 0., 0., 0., 0., 0., 0. }; // zero-xyz + zero-euler? velocity
        }
        else if ( _jointType == eJointType::FIXED )
        {
            // fixed joints have 0-dofs, as expected
            m_data.nqpos = m_nqpos = 0;
            m_data.nqvel = m_nqvel = 0;
        }
        else
        {
            TYSOC_CORE_ERROR( "Joint \"{0}\" has unsupported type \"{1}\"", m_name, tysoc::toString( _jointType ) );
        }
    }

    TJoint::~TJoint()
    {
        m_jointImplRef = nullptr;
        m_drawableImplRef = nullptr;
        m_ownerRef = nullptr;
        m_ownerParentRef = nullptr;
    }

    void TJoint::setOwnerBody( TIBody* ownerRef )
    {
        /* keep a reference to the owner for later usage */
        m_ownerRef = ownerRef;
    }

    void TJoint::setOwnerParent( TIBody* ownerParentRef )
    {
        /* keep a reference to the owner's parent body for later usage */
        m_ownerParentRef = ownerParentRef;
    }

    void TJoint::setAdapter( TIJointAdapter* jointImplRef )
    {
        /* notify the backend that the current adapter is ready for deletion */
        if ( m_jointImplRef )
            m_jointImplRef->detach();

        /* keep a reference to the joint-adapter, to access the low-level API for each backend */
        m_jointImplRef = jointImplRef;
    }

    void TJoint::setDrawable( TIDrawable* drawableImplRef )
    {
        /* notify the backend that the current adapter is ready for deletion */
        if ( m_drawableImplRef )
            m_drawableImplRef->detach();

        /* keep a reference to the drawable (from specific rendering backend) */
        m_drawableImplRef = drawableImplRef;
    }

    void TJoint::setLocalTransform( const TMat4& localTransform )
    {
        /* @note: the current design allows joints to be placed wherever the user wants. However,
                  the owner body is not expected to change its own transform to keep the constraint
                  in the same relative pose. For example, if a hinge for a door is moved with this
                  method, the behaviour would be to move the constraint to the new relative place,
                  but the panel that this joint controls would not change its relative position w.r.t.
                  its parent body. It's up to the user to correct the local-transform of its parent
                  accordingly to get the desired behaviour. */

        /* update local-transform for this joint */
        m_localTf = localTransform;
        m_localPos = m_localTf.getPosition();
        m_localRot = m_localTf.getRotation();

        /* update world-transform for this joint */
        m_tf = m_ownerRef->tf() * m_localTf;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();

        if ( m_jointImplRef )
            m_jointImplRef->setLocalTransform( m_localTf );

        if ( m_drawableImplRef )
            m_drawableImplRef->setWorldTransform( m_tf );
    }

    void TJoint::setQpos( const std::vector< TScalar >& qpos )
    {
        if ( qpos.size() != m_nqpos )
        {
            TYSOC_CORE_WARN( "Joint >>> tried to set a different number of qpos than the ones available \
                              for this joint. Skipped setting the q-values to avoid undefined behaviour." );
            return;
        }

        for ( size_t i = 0; i < m_nqpos; i++ )
            m_qpos[i] = qpos[i];

        if ( m_jointImplRef )
            m_jointImplRef->setQpos( m_qpos );
    }

    void TJoint::setQvel( const std::vector< TScalar >& qvel )
    {
        if ( qvel.size() != m_nqvel )
        {
            TYSOC_CORE_WARN( "Joint >>> tried to set a different number of qvel than the ones available \
                              for this joint. Skipped setting the q-values to avoid undefined behaviour." );
            return;
        }

        for ( size_t i = 0; i < m_nqvel; i++ )
            m_qvel[i] = qvel[i];

        if ( m_jointImplRef )
            m_jointImplRef->setQvel( m_qvel );
    }

    std::vector< TScalar > TJoint::getQpos() const
    {
        std::vector< TScalar > _qpos( m_nqpos, 0.0f );
        for ( size_t i = 0; i < m_nqpos; i++ )
            _qpos[i] = m_qpos[i];

        return _qpos;
    }

    std::vector< TScalar > TJoint::getQpos0() const
    {
        std::vector< TScalar > _qpos( m_nqpos, 0.0f );
        for ( size_t i = 0; i < m_nqpos; i++ )
            _qpos[i] = m_qpos0[i];

        return _qpos;
    }

    std::vector< TScalar > TJoint::getQvel() const
    {
        std::vector< TScalar > _qvel( m_nqvel, 0.0f );
        for ( size_t i = 0; i < m_nqvel; i++ )
            _qvel[i] = m_qvel[i];

        return _qvel;
    }

    std::vector< TScalar > TJoint::getQvel0() const
    {
        std::vector< TScalar > _qvel( m_nqvel, 0.0f );
        for ( size_t i = 0; i < m_nqvel; i++ )
            _qvel[i] = m_qvel0[i];

        return _qvel;
    }

    void TJoint::changeLimits( const TVec2& limits )
    {
        m_data.limits = limits;

        if ( m_jointImplRef )
            m_jointImplRef->changeLimits( limits );
    }

    void TJoint::preStep()
    {
        TYSOC_CORE_ASSERT( m_ownerRef, "Joint >>> joint \"{0}\" isn't attached to an owner body", m_name );

        if ( m_jointImplRef )
            m_jointImplRef->preStep();
    }

    void TJoint::postStep()
    {
        TYSOC_CORE_ASSERT( m_ownerRef, "Joint >>> joint \"{0}\" isn't attached to an owner body", m_name );

        if ( m_jointImplRef )
        {
            m_jointImplRef->postStep();
            m_jointImplRef->getQpos( m_qpos );
            m_jointImplRef->getQvel( m_qvel );
        }

        m_tf = m_ownerRef->tf() * m_localTf;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();

        if ( m_drawableImplRef )
            m_drawableImplRef->setWorldTransform( m_tf );
    }

    void TJoint::reset()
    {
        TYSOC_CORE_ASSERT( m_ownerRef, "Joint >>> joint \"{0}\" isn't attached to an owner body", m_name );

        if ( m_jointImplRef )
        {
            m_jointImplRef->reset();
            m_jointImplRef->getQpos( m_qpos );
            m_jointImplRef->getQvel( m_qvel );
        }

        // @todo: check resets when qpos0 changes, which changes zero|rest configuration
        m_tf = m_ownerRef->tf() * m_localTf;
        m_pos = m_tf.getPosition();
        m_rot = m_tf.getRotation();

        if ( m_drawableImplRef )
            m_drawableImplRef->setWorldTransform( m_tf );
    }

}