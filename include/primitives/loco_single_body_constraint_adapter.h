#pragma once

#include <primitives/loco_single_body_constraint.h>

namespace loco {
namespace primitives {

    class TISingleBodyConstraint;

    class TISingleBodyConstraintAdapter
    {
    public :

        TISingleBodyConstraintAdapter( TISingleBodyConstraint* constraint_ref );

        virtual ~TISingleBodyConstraintAdapter();

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void Reset() = 0;

        virtual void OnDetach();

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

        ~TISingleBodyRevoluteConstraintAdapter() = default;

        virtual void SetHingeAngle( TScalar hinge_angle ) = 0;

        virtual void SetLimits( const TVec2& limits ) = 0;

        virtual void GetHingeAngle( TScalar& dst_hinge_angle ) = 0;
    };

    class TISingleBodyPrismaticConstraintAdapter : public TISingleBodyConstraintAdapter
    {
    public :

        TISingleBodyPrismaticConstraintAdapter( TISingleBodyConstraint* constraint_ref )
            : TISingleBodyConstraintAdapter( constraint_ref ) {}

        ~TISingleBodyPrismaticConstraintAdapter() = default;

        virtual void SetSlidePosition( TScalar slide_position ) = 0;

        virtual void SetLimits( const TVec2& limits ) = 0;

        virtual void GetSlidePosition( TScalar& dst_slide_position ) = 0;
    };
}}