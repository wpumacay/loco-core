
#include <primitives/loco_single_body.h>

namespace loco
{
    TSingleBody::TSingleBody( const std::string& name,
                              const TBodyData& data,
                              const TVec3& position,
                              const TMat3& rotation )
        : TObject( name )
    {
        m_Data = data;
        m_BodyAdapter = nullptr;
        m_Constraint = nullptr;

        m_tf.set( position, 3 );
        m_tf.set( rotation );

        m_tf0.set( position, 3 );
        m_tf0.set( rotation );

        m_TotalForce = { 0.0, 0.0, 0.0 };
        m_TotalTorque = { 0.0, 0.0, 0.0 };
        m_LinearVelocity = { 0.0, 0.0, 0.0 };
        m_AngularVelocity = { 0.0, 0.0, 0.0 };
        m_LinearVelocity0 = { 0.0, 0.0, 0.0 };
        m_AngularVelocity0 = { 0.0, 0.0, 0.0 };

        m_Collider = std::make_unique<TSingleBodyCollider>( m_name + loco::SUFFIX_COLLIDER, m_Data.collision );
        m_Drawable = std::make_unique<TDrawable>( m_name + loco::SUFFIX_DRAWABLE, m_Data.visual );
        m_Collider->SetParentBody( this );
        m_Drawable->SetParentObject( this );

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBody {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBody " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBody::~TSingleBody()
    {
        DetachSim();
        DetachViz();

        m_BodyAdapter = nullptr;
        m_Constraint = nullptr;
        m_Collider = nullptr;
        m_Drawable = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSingleBody {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TSingleBody " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TSingleBody::SetBodyAdapter( TISingleBodyAdapter* body_adapter_ref )
    {
        m_BodyAdapter = body_adapter_ref;
    }

    void TSingleBody::SetCollider( std::unique_ptr<TSingleBodyCollider> collider )
    {
        LOCO_CORE_ASSERT( collider, "TSingleBody::SetCollider >>> tried adding nullptr to body {0}", m_name );

        m_Collider = std::move( collider );
        m_Collider->SetParentBody( this );
        m_Data.collision = m_Collider->data();
    }

    void TSingleBody::SetConstraint( std::unique_ptr<TISingleBodyConstraint> constraint )
    {
        LOCO_CORE_ASSERT( constraint, "TSingleBody::SetConstraint >>> tried adding nullptr to body {0}", m_name );

        m_Constraint = std::move( constraint );
        m_Constraint->SetParentBody( this );
    }

    void TSingleBody::SetDrawable( std::unique_ptr<TDrawable> drawable )
    {
        LOCO_CORE_ASSERT( drawable, "TSingleBody::SetDrawable >>> tried adding nullptr to body {0}", m_name );

        m_Drawable = std::move( drawable );
        m_Drawable->SetParentObject( this );
        m_Data.visual = m_Drawable->data();
    }

    void TSingleBody::SetLinearVelocity( const TVec3& linear_vel )
    {
        m_LinearVelocity = linear_vel;
        if ( m_BodyAdapter )
            m_BodyAdapter->SetLinearVelocity( linear_vel );
    }

    void TSingleBody::SetAngularVelocity( const TVec3& angular_vel )
    {
        m_AngularVelocity = angular_vel;
        if ( m_BodyAdapter )
            m_BodyAdapter->SetAngularVelocity( angular_vel );
    }

    void TSingleBody::AddForceCOM( const TVec3& force )
    {
        m_TotalForce = m_TotalForce + force;
    }

    void TSingleBody::AddTorqueCOM( const TVec3& torque )
    {
        m_TotalTorque = m_TotalTorque + torque;
    }

    void TSingleBody::_InitializeInternal()
    {
        if ( m_BodyAdapter )
            m_BodyAdapter->Initialize();

        if ( m_Constraint )
            m_Constraint->Initialize();

        if ( m_Collider )
            m_Collider->Initialize();

        if ( m_Drawable )
            m_Drawable->Initialize();
    }

    void TSingleBody::_PreStepInternal()
    {
        if ( m_BodyAdapter )
        {
            m_BodyAdapter->SetForceCOM( m_TotalForce );
            m_BodyAdapter->SetTorqueCOM( m_TotalTorque );
        }

        if ( m_Constraint )
            m_Constraint->PreStep();

        if ( m_Collider )
            m_Collider->PreStep();

        if ( m_Drawable )
            m_Drawable->PreStep();
    }

    void TSingleBody::_PostStepInternal()
    {
        if ( m_BodyAdapter )
        {
            m_BodyAdapter->GetLinearVelocity( m_LinearVelocity );
            m_BodyAdapter->GetAngularVelocity( m_AngularVelocity );
            m_BodyAdapter->GetTransform( m_tf );
        }

        if ( m_Constraint )
            m_Constraint->PostStep();

        if ( m_Collider )
            m_Collider->PostStep();

        if ( m_Drawable )
            m_Drawable->PostStep();

        m_TotalForce = { 0.0, 0.0, 0.0 };
        m_TotalTorque = { 0.0, 0.0, 0.0 };
    }

    void TSingleBody::_ResetInternal()
    {
        if ( m_Collider )
            m_Collider->Reset();

        if ( m_Drawable )
            m_Drawable->Reset();

        SetTransform( m_tf0 );
        SetLinearVelocity( m_LinearVelocity0 );
        SetAngularVelocity( m_AngularVelocity0 );

        if ( m_Constraint )
            m_Constraint->Reset();

        if ( m_BodyAdapter )
            m_BodyAdapter->Reset();

        m_TotalForce = { 0.0, 0.0, 0.0 };
        m_TotalTorque = { 0.0, 0.0, 0.0 };
    }

    void TSingleBody::_DetachSimInternal()
    {
        if ( m_BodyAdapter )
            m_BodyAdapter->OnDetach();
        m_BodyAdapter = nullptr;

        if ( m_Constraint )
            m_Constraint->DetachSim();

        if ( m_Collider )
            m_Collider->DetachSim();

        if ( m_Drawable )
            m_Drawable->DetachSim();
    }

    void TSingleBody::_DetachVizInternal()
    {
        if ( m_Constraint )
            m_Constraint->DetachViz();

        if ( m_Collider )
            m_Collider->DetachViz();

        if ( m_Drawable )
            m_Drawable->DetachViz();
    }

    void TSingleBody::_SetTransformInternal( const TMat4& transform )
    {
        m_tf = transform;

        if ( m_BodyAdapter )
            m_BodyAdapter->SetTransform( m_tf );

        if ( m_Constraint )
            m_Constraint->PostStep();

        if ( m_Collider )
            m_Collider->PostStep();

        if ( m_Drawable )
            m_Drawable->PostStep();
    }
}