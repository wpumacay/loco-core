#pragma once

#include <primitives/loco_single_body_constraint.h>

namespace loco
{
    class TISingleBodyConstraint;

    class TISingleBodyConstraintAdapter
    {
    public :

        TISingleBodyConstraintAdapter( TISingleBodyConstraint* constraint_ref )
            : m_ConstraintRef( constraint_ref ), m_Detached( false ) {}

        virtual ~TISingleBodyConstraintAdapter() = default;

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void Reset() = 0;

        virtual void OnDetach() = 0;

        TISingleBodyConstraint* constraint() { return m_ConstraintRef; }

        const TISingleBodyConstraint* constraint() const { return m_ConstraintRef; }

        bool detached() const { return m_Detached; }

    protected :

        // Reference to owner constraint
        TISingleBodyConstraint* m_ConstraintRef;
        // Flag used to check resource state
        bool m_Detached;
    };

    class TISingleBodyRevoluteConstraintAdapter : public TISingleBodyConstraintAdapter
    {
    public :

        TISingleBodyRevoluteConstraintAdapter( TISingleBodyConstraint* constraint_ref )
            : TISingleBodyConstraintAdapter( constraint_ref ) {}

        virtual ~TISingleBodyRevoluteConstraintAdapter() = default;

        virtual void SetHingeAngle( TScalar hinge_angle ) = 0;

        virtual void SetLimits( const TVec2& limits ) = 0;

        virtual void GetHingeAngle( TScalar& dst_hinge_angle ) = 0;
    };

    class TISingleBodyPrismaticConstraintAdapter : public TISingleBodyConstraintAdapter
    {
    public :

        TISingleBodyPrismaticConstraintAdapter( TISingleBodyConstraint* constraint_ref )
            : TISingleBodyConstraintAdapter( constraint_ref ) {}

        virtual ~TISingleBodyPrismaticConstraintAdapter() = default;

        virtual void SetSlidePosition( TScalar slide_position ) = 0;

        virtual void SetLimits( const TVec2& limits ) = 0;

        virtual void GetSlidePosition( TScalar& dst_slide_position ) = 0;
    };
}