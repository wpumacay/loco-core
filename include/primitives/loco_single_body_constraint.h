#pragma once

#include <loco_data.h>
#include <primitives/loco_single_body.h>
#include <primitives/loco_single_body_constraint_adapter.h>

namespace loco {
namespace primitives {

    class TSingleBody;
    class TISingleBodyConstraintAdapter;

    enum class eConstraintType : uint8_t
    {
        NONE = 0,
        REVOLUTE,
        PRISMATIC,
        SPHERICAL,
        TRANSLATIONAL3D,
        UNIVERSAL3D,
        PLANAR
    };

    std::string ToString( const eConstraintType& type );

    class TISingleBodyConstraint : public TObject
    {
    public :

        TISingleBodyConstraint( const std::string& name,
                                const TMat4& local_transform );

        ~TISingleBodyConstraint();

        void SetParentBody( TSingleBody* parent_body_ref );

        void SetConstraintAdapter( TISingleBodyConstraintAdapter* constraint_adapter_ref );

        TMat4 local_tf() const { return m_LocalTransform; }

        eConstraintType constraint_type() const { return m_ConstraintType; }

        static eObjectType GetStaticType() { return eObjectType::SINGLE_BODY_CONSTRAINT; }

        TSingleBody* parent() { return m_ParentBodyRef; }

        const TSingleBody* parent() const { return m_ParentBodyRef; }

        TISingleBodyConstraintAdapter* constraint_adapter() { return m_ConstraintAdapterRef; }

        const TISingleBodyConstraintAdapter* constraint_adapter() const { return m_ConstraintAdapterRef; }

    protected :

        void _DetachSimInternal() override;

        void _DetachVizInternal() override;

        void _SetTransformInternal( const TMat4& transform ) override;

        eObjectType _GetObjectTypeInternal() const override { return eObjectType::SINGLE_BODY_CONSTRAINT; }

    protected :

        // Type of constraint applied to the single-body
        eConstraintType m_ConstraintType;
        // Reference to the body this constraint is applied to
        TSingleBody* m_ParentBodyRef;
        // Relative transform of this constraint w.r.t parent's frame
        TMat4 m_LocalTransform;
        // Reference to the adapter object that gives access to the low-leve API for a specific backend
        TISingleBodyConstraintAdapter* m_ConstraintAdapterRef;
    };

    class TSingleBodyRevoluteConstraint : public TISingleBodyConstraint
    {
    public :

        TSingleBodyRevoluteConstraint( const std::string& name,
                                       const TMat4& local_transform,
                                       const TVec3& axis );

        ~TSingleBodyRevoluteConstraint();

        void SetHingeAngle( TScalar hinge_angle );

        void SetLimits( const TVec2& limits );

        TScalar hinge_angle() const { return m_HingeAngle; }

        TVec2 limits() const { return m_Limits; }

        TVec3 axis() const { return m_Axis; }

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;

    private :
        // Current value of the hinge-angle of the constraint
        TScalar m_HingeAngle;
        // Limits of rotation (min-angle, max-angle) around the rotation axis
        TVec2 m_Limits;
        // Axis of rotation of the revolute constraint
        TVec3 m_Axis;
    };

    class TSingleBodyPrismaticConstraint : public TISingleBodyConstraint
    {
    public :

        TSingleBodyPrismaticConstraint( const std::string& name,
                                        const TMat4& local_transform,
                                        const TVec3& axis );

        ~TSingleBodyPrismaticConstraint();

        void SetSlidePosition( TScalar slide_position );

        void SetLimits( const TVec2& limits );

        TScalar slide_position() const { return m_SlidePosition; }

        TVec2 limits() const { return m_Limits; }

        TVec3 axis() const { return m_Axis; }

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;

    private :
        // Current value of the linear position of the constraint
        TScalar m_SlidePosition;
        // Limits of linear movement (min-linear-pos, max-linear-pos) along the slide axis
        TVec2 m_Limits;
        // Axis of movement of the prismatic constraint
        TVec3 m_Axis;
    };

    class TSingleBodySphericalConstraint : public TISingleBodyConstraint
    {
    public :

        TSingleBodySphericalConstraint( const std::string& name,
                                        const TMat4& local_transform );

        ~TSingleBodySphericalConstraint();

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;
    };

    class TSingleBodyTranslational3dConstraint : public TISingleBodyConstraint
    {
    public :

        TSingleBodyTranslational3dConstraint( const std::string& name );

        ~TSingleBodyTranslational3dConstraint();

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;
    };

    class TSingleBodyUniversal3dConstraint : public TISingleBodyConstraint
    {
    public :

        TSingleBodyUniversal3dConstraint( const std::string& name );

        ~TSingleBodyUniversal3dConstraint();

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;
    };

    class TSingleBodyPlanarConstraint : public TISingleBodyConstraint
    {
    public :

        TSingleBodyPlanarConstraint( const std::string& name );

        ~TSingleBodyPlanarConstraint();

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;
    };
}}