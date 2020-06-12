
#include <kinematic_trees/loco_kinematic_tree_joint.h>

namespace loco {
namespace kintree {

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                 Joint Interface impl.                                  ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    TKinematicTreeJoint::TKinematicTreeJoint( const std::string& name,
                                              const TKinematicTreeJointData& joint_data )
        : TObject( name )
    {
        m_Data = joint_data;

    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TKinematicTreeJoint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TKinematicTreeJoint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TKinematicTreeJoint::~TKinematicTreeJoint()
    {
        m_ParentBodyRef = nullptr;
        m_JointAdapterRef = nullptr;

    #if defined(LOCO_CORE_USE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TKinematicTreeJoint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TKinematicTreeJoint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TKinematicTreeJoint::SetParentBody( TKinematicTreeBody* parent_body_ref, const TMat4& local_tf )
    {
        LOCO_CORE_ASSERT( parent_body_ref, "TKinematicTreeJoint::SetParentBody >>> tried adding nullptr as parent to joint {0}", m_name );
        m_ParentBodyRef = parent_body_ref;
        m_LocalTf = local_tf;
        m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }

    void TKinematicTreeJoint::SetJointAdapter( TIKinematicTreeJointAdapter* joint_adapter_ref )
    {
        LOCO_CORE_ASSERT( joint_adapter_ref, "TKinematicTreeJoint::SetJointAdapter >>> tried adding nullptr as joint-adapter to joint {0}", m_name );
        m_JointAdapterRef = joint_adapter_ref;
    }

    void TKinematicTreeJoint::SetQpos( const std::vector<TScalar>& qpos )
    {
        if ( qpos.size() != m_NumQpos )
        {
            LOCO_CORE_WARN( "TKinematicTreeJoint::SetQpos >>> mismatch between qpos. Expected num={0}, but got num={1}", m_NumQpos, qpos.size() );
            return;
        }
        if ( m_JointAdapterRef ) // we're on a simulation, so let the backend handle this
            m_JointAdapterRef->SetQpos( qpos );
        else // we're not on a simulation, so let's save the qpos values directly
            m_Qpos = qpos;
    }

    void TKinematicTreeJoint::SetQpos0( const std::vector<TScalar>& qpos0 )
    {
        if ( qpos0.size() != m_NumQpos )
        {
            LOCO_CORE_WARN( "TKinematicTreeJoint::SetQpos0 >>> mismatch between qpos0. Expected num={0}, but got num={1}", m_NumQpos, qpos0.size() );
            return;
        }
        m_Qpos0 = qpos0;
    }

    void TKinematicTreeJoint::SetQvel( const std::vector<TScalar>& qvel )
    {
        if ( qvel.size() != m_NumQvel )
        {
            LOCO_CORE_WARN( "TKinematicTreeJoint::SetQvel >>> mismatch between qvel. Expected num={0}, but got num={1}", m_NumQvel, qvel.size() );
            return;
        }
        if ( m_JointAdapterRef ) // we're on a simulation, so let the backend handle this
            m_JointAdapterRef->SetQvel( qvel );
        else // we're not on a simulation, so let's save the qvel values directly
            m_Qvel = qvel;
    }

    void TKinematicTreeJoint::SetQvel0( const std::vector<TScalar>& qvel0 )
    {
        if ( qvel0.size() != m_NumQvel )
        {
            LOCO_CORE_WARN( "TKinematicTreeJoint::SetQvel0 >>> mismatch between qvel0. Expected num={0}, but got num={1}", m_NumQvel, qvel0.size() );
            return;
        }
        m_Qvel0 = qvel0;
    }

    void TKinematicTreeJoint::SetStiffness( const TScalar& stiffness )
    {
        m_Data.stiffness = stiffness;
        if ( m_JointAdapterRef )
            m_JointAdapterRef->ChangeStiffness( stiffness );
    }

    void TKinematicTreeJoint::SetArmature( const TScalar& armature )
    {
        m_Data.armature = armature;
        if ( m_JointAdapterRef )
            m_JointAdapterRef->ChangeArmature( armature );
    }

    void TKinematicTreeJoint::SetLocalTransform( const TMat4& local_tf )
    {
        m_LocalTf = local_tf;
        m_Data.local_tf = local_tf;
        if ( m_JointAdapterRef )
            m_JointAdapterRef->SetLocalTransform( local_tf );
    }

    void TKinematicTreeJoint::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TKinematicTreeJoint::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TKinematicTreeJoint::_PostStepInternal()
    {
        if ( m_JointAdapterRef )
        {
            m_JointAdapterRef->GetQpos( m_Qpos );
            m_JointAdapterRef->GetQvel( m_Qvel );
        }

        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }

    void TKinematicTreeJoint::_ResetInternal()
    {
        m_Qpos = m_Qpos0;
        m_Qvel = m_Qvel0;
        if ( m_JointAdapterRef )
            m_JointAdapterRef->Reset();

        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }

    void TKinematicTreeJoint::_DetachSimInternal()
    {
        if ( m_JointAdapterRef )
            m_JointAdapterRef->OnDetach();
        m_JointAdapterRef = nullptr;
    }

    void TKinematicTreeJoint::_DetachVizInternal()
    {
        /* Nothing to detach, as there's no drawable resource attached to this joint */
    }

    void TKinematicTreeJoint::_SetTransformInternal( const TMat4& transform )
    {
        LOCO_CORE_WARN( "TKinematicTreeCollider::_SetTransformInternal >>> collider objects don't have \
                        this functionality. Change the transform of the parent body instead. (collider={0})", m_name );
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTf;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                 Revolute-joint impl.                                   ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    TKinematicTreeRevoluteJoint::TKinematicTreeRevoluteJoint( const std::string& name,
                                                              const TMat4& local_tf,
                                                              const TVec3& local_axis,
                                                              const TVec2& limits,
                                                              const TScalar& stiffness,
                                                              const TScalar& armature )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {
        m_Data.type = eJointType::REVOLUTE;
        m_Data.limits = limits;
        m_Data.stiffness = stiffness;
        m_Data.armature = armature;
        m_Data.local_axis = local_axis;
        m_Data.local_tf = local_tf;

        m_NumQpos = 1;
        m_NumQvel = 1;
        m_Qpos = { 0.0f };
        m_Qvel = { 0.0f };
        m_Qpos0 = { 0.0f };
        m_Qvel0 = { 0.0f };
    }

    void TKinematicTreeRevoluteJoint::SetAngle( const TScalar& angle )
    {
        SetQpos( { angle } );
    }

    void TKinematicTreeRevoluteJoint::SetAxis( const TVec3& axis )
    {
        if ( m_JointAdapterRef )
            m_JointAdapterRef->ChangeAxis( axis );
    }

    void TKinematicTreeRevoluteJoint::SetLimits( const TVec2& limits )
    {
        if ( m_JointAdapterRef )
            m_JointAdapterRef->ChangeLimits( limits );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                Prismatic-joint impl.                                   ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    TKinematicTreePrismaticJoint::TKinematicTreePrismaticJoint( const std::string& name,
                                                                const TMat4& local_tf,
                                                                const TVec3& local_axis,
                                                                const TVec2& limits,
                                                                const TScalar& stiffness,
                                                                const TScalar& armature )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {
        m_Data.type = eJointType::PRISMATIC;
        m_Data.limits = limits;
        m_Data.stiffness = stiffness;
        m_Data.armature = armature;
        m_Data.local_axis = local_axis;
        m_Data.local_tf = local_tf;

        m_NumQpos = 1;
        m_NumQvel = 1;
        m_Qpos = { 0.0f };
        m_Qvel = { 0.0f };
        m_Qpos0 = { 0.0f };
        m_Qvel0 = { 0.0f };
    }

    void TKinematicTreePrismaticJoint::SetPosition( const TScalar& position )
    {
        SetQpos( { position } );
    }

    void TKinematicTreePrismaticJoint::SetAxis( const TVec3& axis )
    {
        if ( m_JointAdapterRef )
            m_JointAdapterRef->ChangeAxis( axis );
    }

    void TKinematicTreePrismaticJoint::SetLimits( const TVec2& limits )
    {
        if ( m_JointAdapterRef )
            m_JointAdapterRef->ChangeLimits( limits );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                Spherical-joint impl.                                   ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    TKinematicTreeSphericalJoint::TKinematicTreeSphericalJoint( const std::string& name,
                                                                const TMat4& local_tf,
                                                                const TVec2& limits,
                                                                const TScalar& stiffness,
                                                                const TScalar& armature )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {
        m_Data.type = eJointType::SPHERICAL;
        m_Data.limits = limits;
        m_Data.stiffness = stiffness;
        m_Data.armature = armature;
        m_Data.local_tf = local_tf;

        m_NumQpos = 4;
        m_NumQvel = 3;
        m_Qpos = { 0.0f, 0.0f, 0.0f, 1.0f };
        m_Qvel = { 0.0f, 0.0f, 0.0f };
        m_Qpos0 = { 0.0f, 0.0f, 0.0f, 1.0f };
        m_Qvel0 = { 0.0f, 0.0f, 0.0f };
    }

    void TKinematicTreeSphericalJoint::SetQuat( const TVec4& quat )
    {
        SetQpos( { quat.x(), quat.y(), quat.z(), quat.w() } );
    }

    void TKinematicTreeSphericalJoint::SetLimits( const TVec2& limits )
    {
        if ( m_JointAdapterRef )
            m_JointAdapterRef->ChangeLimits( limits );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                  Planar-joint impl.                                    ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    TKinematicTreePlanarJoint::TKinematicTreePlanarJoint( const std::string& name,
                                                          const TVec3& plane_axis_1,
                                                          const TVec3& plane_axis_2 )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {
        m_Data.type = eJointType::PLANAR;
        m_Data.plane_axis_1 = plane_axis_1;
        m_Data.plane_axis_2 = plane_axis_2;

        m_NumQpos = 3;
        m_NumQvel = 3;
        m_Qpos = { 0.0f, 0.0f, 0.0f };
        m_Qvel = { 0.0f, 0.0f, 0.0f };
        m_Qpos0 = { 0.0f, 0.0f, 0.0f };
        m_Qvel0 = { 0.0f, 0.0f, 0.0f };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                  FIxed-joint impl.                                     ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    TKinematicTreeFixedJoint::TKinematicTreeFixedJoint( const std::string& name,
                                                        const TMat4& local_tf )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {
        m_Data.type = eJointType::FIXED;
        m_Data.local_tf = local_tf;

        m_NumQpos = 0;
        m_NumQvel = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////                                                                                        ////
    ////                                   Free-joint impl.                                     ////
    ////                                                                                        ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    TKinematicTreeFreeJoint::TKinematicTreeFreeJoint( const std::string& name )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {
        m_Data.type = eJointType::FREE;

        m_NumQpos = 7;
        m_NumQvel = 6;
        m_Qpos = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; // x-y-z | qx-qy-qz-qw
        m_Qvel = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; // x-y-z | rx-ry-rz
        m_Qpos0 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; // x-y-z | qx-qy-qz-qw
        m_Qvel0 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; // x-y-z | rx-ry-rz
    }
}}