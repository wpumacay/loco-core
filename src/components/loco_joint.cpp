
#include <components/loco_joint.h>

namespace loco
{
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
            LOCO_CORE_ERROR( "Joint \"{0}\" has unsupported type \"{1}\"", m_name, loco::toString( _jointType ) );
        }

        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Created TJoint @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Created TJoint @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
    }

    TJoint::~TJoint()
    {
        m_jointImplRef = nullptr;
        m_drawableImplRef = nullptr;
        m_ownerRef = nullptr;
        m_ownerParentRef = nullptr;

        #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
            if ( TLogger::IsActive() )
                LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TJoint @ {0}", loco::PointerToHexAddress( this ) );
            else
                std::cout << "Loco::Allocs: Destroyed TJoint @ " << loco::PointerToHexAddress( this ) << std::endl;
        #endif
    }

    void TJoint::SetOwnerBody( TIBody* ownerRef )
    {
        m_ownerRef = ownerRef;
    }

    void TJoint::SetOwnerParent( TIBody* ownerParentRef )
    {
        m_ownerParentRef = ownerParentRef;
    }

    void TJoint::SetAdapter( TIJointAdapter* jointImplRef )
    {
        m_jointImplRef = jointImplRef;
    }

    void TJoint::SetDrawable( TIDrawable* drawableImplRef )
    {
        m_drawableImplRef = drawableImplRef;
    }

    void TJoint::SetLocalPosition( const TVec3& localPosition )
    {
        /* update local-transform for this joint */
        m_localTf.set( localPosition, 3 );

        /* update world-transform for this joint */
        if ( m_ownerRef )
            m_tf = m_ownerRef->tf() * m_localTf;

        if ( m_jointImplRef )
            m_jointImplRef->SetLocalTransform( m_localTf );

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TJoint::SetLocalRotation( const TMat3& localRotation )
    {
        /* update local-transform for this joint */
        m_localTf.set( localRotation );

        /* update world-transform for this joint */
        if ( m_ownerRef )
            m_tf = m_ownerRef->tf() * m_localTf;

        if ( m_jointImplRef )
            m_jointImplRef->SetLocalTransform( m_localTf );

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TJoint::SetLocalEuler( const TVec3& localEuler )
    {
        /* update local-transform for this joint */
        m_localTf.set( tinymath::rotation( localEuler ) );

        /* update world-transform for this joint */
        if ( m_ownerRef )
            m_tf = m_ownerRef->tf() * m_localTf;

        if ( m_jointImplRef )
            m_jointImplRef->SetLocalTransform( m_localTf );

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TJoint::SetLocalQuat( const TVec4& localQuaternion )
    {
        /* update local-transform for this joint */
        m_localTf.set( tinymath::rotation( localQuaternion ) );

        /* update world-transform for this joint */
        if ( m_ownerRef )
            m_tf = m_ownerRef->tf() * m_localTf;

        if ( m_jointImplRef )
            m_jointImplRef->SetLocalTransform( m_localTf );

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TJoint::SetLocalTransform( const TMat4& localTransform )
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

        /* update world-transform for this joint */
        if ( m_ownerRef )
            m_tf = m_ownerRef->tf() * m_localTf;

        if ( m_jointImplRef )
            m_jointImplRef->SetLocalTransform( m_localTf );

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TJoint::SetQpos( const std::vector< TScalar >& qpos )
    {
        if ( qpos.size() != m_nqpos )
        {
            LOCO_CORE_WARN( "Joint >>> tried to set a different number of qpos than the ones available \
                              for this joint. Skipped setting the q-values to avoid undefined behaviour." );
            return;
        }

        for ( size_t i = 0; i < m_nqpos; i++ )
            m_qpos[i] = qpos[i];

        if ( m_jointImplRef )
            m_jointImplRef->SetQpos( m_qpos );
    }

    void TJoint::SetQvel( const std::vector< TScalar >& qvel )
    {
        if ( qvel.size() != m_nqvel )
        {
            LOCO_CORE_WARN( "Joint >>> tried to set a different number of qvel than the ones available \
                              for this joint. Skipped setting the q-values to avoid undefined behaviour." );
            return;
        }

        for ( size_t i = 0; i < m_nqvel; i++ )
            m_qvel[i] = qvel[i];

        if ( m_jointImplRef )
            m_jointImplRef->SetQvel( m_qvel );
    }

    std::vector< TScalar > TJoint::GetQpos() const
    {
        std::vector< TScalar > _qpos( m_nqpos, 0.0f );
        for ( size_t i = 0; i < m_nqpos; i++ )
            _qpos[i] = m_qpos[i];

        return _qpos;
    }

    std::vector< TScalar > TJoint::GetQpos0() const
    {
        std::vector< TScalar > _qpos( m_nqpos, 0.0f );
        for ( size_t i = 0; i < m_nqpos; i++ )
            _qpos[i] = m_qpos0[i];

        return _qpos;
    }

    std::vector< TScalar > TJoint::GetQvel() const
    {
        std::vector< TScalar > _qvel( m_nqvel, 0.0f );
        for ( size_t i = 0; i < m_nqvel; i++ )
            _qvel[i] = m_qvel[i];

        return _qvel;
    }

    std::vector< TScalar > TJoint::GetQvel0() const
    {
        std::vector< TScalar > _qvel( m_nqvel, 0.0f );
        for ( size_t i = 0; i < m_nqvel; i++ )
            _qvel[i] = m_qvel0[i];

        return _qvel;
    }

    void TJoint::ChangeLimits( const TVec2& limits )
    {
        m_data.limits = limits;

        if ( m_jointImplRef )
            m_jointImplRef->ChangeLimits( limits );
    }

    void TJoint::Initialize()
    {
        LOCO_CORE_ASSERT( m_ownerRef, "Joint::Initialize >>> joint \"{0}\" isn't attached to an owner body", m_name );

        if ( m_jointImplRef )
        {
            m_jointImplRef->Initialize();
            m_jointImplRef->GetQpos( m_qpos );
            m_jointImplRef->GetQvel( m_qvel );
        }
    }

    void TJoint::PreStep()
    {
        LOCO_CORE_ASSERT( m_ownerRef, "Joint >>> joint \"{0}\" isn't attached to an owner body", m_name );

        if ( m_jointImplRef )
            m_jointImplRef->PreStep();
    }

    void TJoint::PostStep()
    {
        LOCO_CORE_ASSERT( m_ownerRef, "Joint >>> joint \"{0}\" isn't attached to an owner body", m_name );

        if ( m_jointImplRef )
        {
            m_jointImplRef->PostStep();
            m_jointImplRef->GetQpos( m_qpos );
            m_jointImplRef->GetQvel( m_qvel );
        }

        m_tf = m_ownerRef->tf() * m_localTf;

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }

    void TJoint::Reset()
    {
        LOCO_CORE_ASSERT( m_ownerRef, "Joint >>> joint \"{0}\" isn't attached to an owner body", m_name );

        if ( m_jointImplRef )
        {
            m_jointImplRef->Reset();
            m_jointImplRef->GetQpos( m_qpos );
            m_jointImplRef->GetQvel( m_qvel );
        }

        // @todo: check resets when qpos0 changes, which changes zero|rest configuration
        m_tf = m_ownerRef->tf() * m_localTf;

        if ( m_drawableImplRef )
            m_drawableImplRef->SetWorldTransform( m_tf );
    }
}