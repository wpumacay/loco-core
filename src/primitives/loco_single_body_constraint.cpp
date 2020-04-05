
#include <primitives/loco_single_body_constraint.h>

namespace loco
{
    std::string ToString( const eConstraintType& type )
    {
        if ( type == eConstraintType::NONE ) return "none";
        else if ( type == eConstraintType::REVOLUTE ) return "revolute";
        else if ( type == eConstraintType::PRISMATIC ) return "prismatic";
        else if ( type == eConstraintType::SPHERICAL ) return "spherical";
        else if ( type == eConstraintType::PLANAR ) return "planar";
        else if ( type == eConstraintType::TRANSLATIONAL3D ) return "translational3d";
        return "undefined";
    }

    //********************************************************************************************//
    //                                  Constraint-Interface Impl.                                //
    //********************************************************************************************//

    TISingleBodyConstraint::TISingleBodyConstraint( const std::string& name,
                                                    const TMat4& local_transform )
        : TObject( name )
    {
        m_LocalTransform = local_transform;
        m_ParentBodyRef = nullptr;
        m_ConstraintType = eConstraintType::NONE;
        m_ConstraintAdapterRef = nullptr;
    }

    TISingleBodyConstraint::~TISingleBodyConstraint()
    {
        DetachSim();
        DetachViz();

        m_ParentBodyRef = nullptr;
        m_ConstraintAdapterRef = nullptr;
    }

    void TISingleBodyConstraint::SetParentBody( TSingleBody* parent_body_ref )
    {
        LOCO_CORE_ASSERT( parent_body_ref, "TISingleBodyConstraint::SetParentBody >>> tried adding nullptr" );

        m_ParentBodyRef = parent_body_ref;
        m_tf = m_ParentBodyRef->tf() * m_LocalTransform;
    }

    void TISingleBodyConstraint::SetConstraintAdapter( TISingleBodyConstraintAdapter* constraint_adapter_ref )
    {
        LOCO_CORE_ASSERT( constraint_adapter_ref, "TISingleBodyConstraint::SetAdapter >>> tried adding nullptr" );

        m_ConstraintAdapterRef = constraint_adapter_ref;
    }

    void TISingleBodyConstraint::_DetachSimInternal()
    {
        if ( m_ConstraintAdapterRef )
            m_ConstraintAdapterRef->OnDetach();
        m_ConstraintAdapterRef = nullptr;
    }

    void TISingleBodyConstraint::_DetachVizInternal()
    {
        // Nothing to detach from visualizer
    }

    void TISingleBodyConstraint::_SetTransformInternal( const TMat4& transform )
    {
        LOCO_CORE_WARN( "TISingleBodyConstraint::_SetTransformInternal >>> constraint objects don't have \
                        this functionality. Change the transform of the parent body instead. (constraint={0})", m_name );

        // Recover transform from parent
        m_tf = m_ParentBodyRef->tf() * m_LocalTransform;
    }

    //********************************************************************************************//
    //                                  Revolute-Constraint Impl.                                 //
    //********************************************************************************************//

    TSingleBodyRevoluteConstraint::TSingleBodyRevoluteConstraint( const std::string& name,
                                                                  const TMat4& local_transform,
                                                                  const TVec3& axis )
        : TISingleBodyConstraint( name, local_transform )
    {
        m_Axis = axis;
        m_Limits = { 1.0f, -1.0f }; // min > max <=> no limits
        m_HingeAngle = 0.0f;
        m_ConstraintType = eConstraintType::REVOLUTE;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodyRevoluteConstraint {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodyRevoluteConstraint " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodyRevoluteConstraint::~TSingleBodyRevoluteConstraint()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TISingleBodyConstraint {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TISingleBodyConstraint " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TSingleBodyRevoluteConstraint::SetHingeAngle( TScalar hinge_angle )
    {
        m_HingeAngle = hinge_angle;

        if ( m_ConstraintAdapterRef )
            if ( auto revolute_constraint_adapter = dynamic_cast<TISingleBodyRevoluteConstraintAdapter*>( m_ConstraintAdapterRef ) )
                revolute_constraint_adapter->SetHingeAngle( hinge_angle );
    }

    void TSingleBodyRevoluteConstraint::SetLimits( const TVec2& limits )
    {
        m_Limits = limits;

        if ( m_ConstraintAdapterRef )
            if ( auto revolute_constraint_adapter = dynamic_cast<TISingleBodyRevoluteConstraintAdapter*>( m_ConstraintAdapterRef ) )
                revolute_constraint_adapter->SetLimits( limits );
    }

    void TSingleBodyRevoluteConstraint::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TSingleBodyRevoluteConstraint::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TSingleBodyRevoluteConstraint::_PostStepInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTransform;

        if ( m_ConstraintAdapterRef )
            if ( auto revolute_constraint_adapter = dynamic_cast<TISingleBodyRevoluteConstraintAdapter*>( m_ConstraintAdapterRef ) )
                revolute_constraint_adapter->GetHingeAngle( m_HingeAngle );
    }

    void TSingleBodyRevoluteConstraint::_ResetInternal()
    {
        // Nothing to setup for a reset request
    }

    //********************************************************************************************//
    //                                 Prismatic-Constraint Impl.                                 //
    //********************************************************************************************//

    TSingleBodyPrismaticConstraint::TSingleBodyPrismaticConstraint( const std::string& name,
                                                                  const TMat4& local_transform,
                                                                  const TVec3& axis )
        : TISingleBodyConstraint( name, local_transform )
    {
        m_Axis = axis;
        m_Limits = { 1.0f, -1.0f }; // min > max <=> no limits
        m_SlidePosition = 0.0f;
        m_ConstraintType = eConstraintType::PRISMATIC;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodyPrismaticConstraint {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodyPrismaticConstraint " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodyPrismaticConstraint::~TSingleBodyPrismaticConstraint()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TISingleBodyConstraint {0} @ {1}", m_name, loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TISingleBodyConstraint " << m_name << " @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TSingleBodyPrismaticConstraint::SetSlidePosition( TScalar slide_position )
    {
        m_SlidePosition = slide_position;

        if ( m_ConstraintAdapterRef )
            if ( auto prismatic_constraint_adapter = dynamic_cast<TISingleBodyPrismaticConstraintAdapter*>( m_ConstraintAdapterRef ) )
                prismatic_constraint_adapter->SetSlidePosition( slide_position );
    }

    void TSingleBodyPrismaticConstraint::SetLimits( const TVec2& limits )
    {
        m_Limits = limits;

        if ( m_ConstraintAdapterRef )
            if ( auto prismatic_constraint_adapter = dynamic_cast<TISingleBodyPrismaticConstraintAdapter*>( m_ConstraintAdapterRef ) )
                prismatic_constraint_adapter->SetLimits( limits );
    }

    void TSingleBodyPrismaticConstraint::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TSingleBodyPrismaticConstraint::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TSingleBodyPrismaticConstraint::_PostStepInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTransform;

        if ( m_ConstraintAdapterRef )
            if ( auto prismatic_constraint_adapter = dynamic_cast<TISingleBodyPrismaticConstraintAdapter*>( m_ConstraintAdapterRef ) )
                prismatic_constraint_adapter->GetSlidePosition( m_SlidePosition );
    }

    void TSingleBodyPrismaticConstraint::_ResetInternal()
    {
        // Nothing to setup for a reset request
    }
}