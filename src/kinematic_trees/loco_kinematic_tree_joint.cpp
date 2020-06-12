
#include <kinematic_trees/loco_kinematic_tree_joint.h>

namespace loco {
namespace kintree {

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




    TKinematicTreeRevoluteJoint::TKinematicTreeRevoluteJoint( const std::string& name,
                                                              const TMat4& local_tf,
                                                              const TVec3& local_axis,
                                                              const TVec2& limits,
                                                              const TScalar& stiffness,
                                                              const TScalar& armature )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {

    }

    void SetAngle( const TScalar& angle )
    {

    }

    void SetAxis( const TVec3& axis )
    {

    }

    void TKinematicTreeRevoluteJoint::SetLimits( const TVec2& limits )
    {

    }




    TKinematicTreePrismaticJoint::TKinematicTreePrismaticJoint( const std::string& name,
                                                                const TMat4& local_tf,
                                                                const TVec3& local_axis,
                                                                const TVec2& limits,
                                                                const TScalar& stiffness,
                                                                const TScalar& armature )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {

    }

    void TKinematicTreePrismaticJoint::SetPosition( const TScalar& position )
    {

    }

    void TKinematicTreePrismaticJoint::SetAxis( const TVec3& axis )
    {

    }

    void TKinematicTreePrismaticJoint::SetLimits( const TVec2& limits )
    {

    }




    TKinematicTreeSphericalJoint::TKinematicTreeSphericalJoint( const std::string& name,
                                                                const TMat4& local_tf,
                                                                const TVec2& limits,
                                                                const TScalar& stiffness,
                                                                const TScalar& armature )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {

    }

    void TKinematicTreeSphericalJoint::SetQuat( const TVec4& quat )
    {

    }

    void TKinematicTreeSphericalJoint::SetLimits( const TVec2& limits )
    {

    }




    TKinematicTreePlanarJoint::TKinematicTreePlanarJoint( const std::string& name,
                                                          const TVec3& plane_axis_1,
                                                          const TVec3& plane_axis_2 )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {

    }




    TKinematicTreeFixedJoint::TKinematicTreeFixedJoint( const std::string& name,
                                                        const TMat4& local_tf )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {

    }




    TKinematicTreeFreeJoint::TKinematicTreeFreeJoint( const std::string& name )
        : TKinematicTreeJoint( name, TKinematicTreeJointData() )
    {

    }
}}