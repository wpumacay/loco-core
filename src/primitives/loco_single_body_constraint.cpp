
#include <primitives/loco_single_body_constraint.h>

namespace loco
{
    std::string ToString( const eConstraintType& type )
    {
        if ( type == eConstraintType::NONE ) return "none";
        else if ( type == eConstraintType::REVOLUTE ) return "revolute";
        else if ( type == eConstraintType::PRISMATIC ) return "prismatic";
        else if ( type == eConstraintType::SPHERICAL ) return "spherical";
        else if ( type == eConstraintType::TRANSLATIONAL3D ) return "translational3d";
        else if ( type == eConstraintType::UNIVERSAL3D ) return "universal3d";
        else if ( type == eConstraintType::PLANAR ) return "planar";
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
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodyRevoluteConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodyRevoluteConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodyRevoluteConstraint::~TSingleBodyRevoluteConstraint()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TISingleBodyConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TISingleBodyConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
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
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodyPrismaticConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodyPrismaticConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodyPrismaticConstraint::~TSingleBodyPrismaticConstraint()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TISingleBodyConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TISingleBodyConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
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
        // Nothing to setup for a reset request (adapter is handled by parent body)
    }

    //********************************************************************************************//
    //                                 Spherical-Constraint Impl.                                 //
    //********************************************************************************************//

    TSingleBodySphericalConstraint::TSingleBodySphericalConstraint( const std::string& name,
                                                                    const TMat4& local_transform )
        : TISingleBodyConstraint( name, local_transform )
    {
        m_ConstraintType = eConstraintType::SPHERICAL;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodySphericalConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodySphericalConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodySphericalConstraint::~TSingleBodySphericalConstraint()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSingleBodySphericalConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TSingleBodySphericalConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TSingleBodySphericalConstraint::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TSingleBodySphericalConstraint::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TSingleBodySphericalConstraint::_PostStepInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf() * m_LocalTransform;
    }

    void TSingleBodySphericalConstraint::_ResetInternal()
    {
        // Nothing to setup for a reset request (adapter is handled by parent body)
    }

    //********************************************************************************************//
    //                              Translational3d-Constraint Impl.                              //
    //********************************************************************************************//

    TSingleBodyTranslational3dConstraint::TSingleBodyTranslational3dConstraint( const std::string& name )
        : TISingleBodyConstraint( name, TMat4() )
    {
        m_ConstraintType = eConstraintType::TRANSLATIONAL3D;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodyTranslational3dConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodyTranslational3dConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodyTranslational3dConstraint::~TSingleBodyTranslational3dConstraint()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSingleBodyTranslational3dConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TSingleBodyTranslational3dConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TSingleBodyTranslational3dConstraint::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TSingleBodyTranslational3dConstraint::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TSingleBodyTranslational3dConstraint::_PostStepInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf();
    }

    void TSingleBodyTranslational3dConstraint::_ResetInternal()
    {
        // Nothing to setup for a reset request (adapter is handled by parent body)
    }

    //********************************************************************************************//
    //                              Universal3d-Constraint Impl.                              //
    //********************************************************************************************//

    TSingleBodyUniversal3dConstraint::TSingleBodyUniversal3dConstraint( const std::string& name )
        : TISingleBodyConstraint( name, TMat4() )
    {
        m_ConstraintType = eConstraintType::UNIVERSAL3D;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodyUniversal3dConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodyUniversal3dConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodyUniversal3dConstraint::~TSingleBodyUniversal3dConstraint()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSingleBodyUniversal3dConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TSingleBodyUniversal3dConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TSingleBodyUniversal3dConstraint::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TSingleBodyUniversal3dConstraint::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TSingleBodyUniversal3dConstraint::_PostStepInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf();
    }

    void TSingleBodyUniversal3dConstraint::_ResetInternal()
    {
        // Nothing to setup for a reset request (adapter is handled by parent body)
    }

    //********************************************************************************************//
    //                                  Planar-Constraint Impl.                                   //
    //********************************************************************************************//

    TSingleBodyPlanarConstraint::TSingleBodyPlanarConstraint( const std::string& name )
        : TISingleBodyConstraint( name, TMat4() )
    {
        m_ConstraintType = eConstraintType::PLANAR;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TSingleBodyPlanarConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TSingleBodyPlanarConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TSingleBodyPlanarConstraint::~TSingleBodyPlanarConstraint()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TSingleBodyPlanarConstraint {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TSingleBodyPlanarConstraint " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TSingleBodyPlanarConstraint::_InitializeInternal()
    {
        // Nothing to setup (adapter is handled by parent body)
    }

    void TSingleBodyPlanarConstraint::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TSingleBodyPlanarConstraint::_PostStepInternal()
    {
        if ( m_ParentBodyRef )
            m_tf = m_ParentBodyRef->tf();
    }

    void TSingleBodyPlanarConstraint::_ResetInternal()
    {
        // Nothing to setup for a reset request (adapter is handled by parent body)
    }
}