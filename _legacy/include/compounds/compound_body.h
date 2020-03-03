#pragma once

#include <components/joint.h>
#include <components/body.h>

#include <compounds/compound.h>

namespace loco
{
    class TCollision;
    class TVisual;
    class TJoint;
    class TCompound;

    /**
    *   Body object used to handle composite bodies (compounds)
    *
    *   This object defines the functionality exposed to the user to create and interact
    *   with objects that form part of a compound (chain), when more complex structures are
    *   required for more complicated tasks.
    *
    *   @checklist:
    *       (1) implementation separate from single-body API
    *       (2) implement adapters in mujoco-backend
    *       (3) implement adapters in bullet-backend
    *       (4) implement adapters in raisim-backend
    *       (5) implement adapters in dart-backend
    *       (6) merge single-body and compound-body functionality (subclassing properly from abstract body)
    */
    class TCompoundBody : public TIBody
    {

    public :

        /**
        *   Creates a compound-body, given a parent-body to connect through a given joint
        *
        *   This constructor creates a compound-body, which consists of a node in a chain of
        *   compound bodies. Hence, this compound-body could be either a child node in the
        *   chain (connected to a parent body through a joint) or a root compound-body, not
        *   connected to any parent body and therefore not requiring any joint. For the last
        *   case a free joint is defined in the specific backend if required.
        */
        TCompoundBody( const std::string& name,
                       const TBodyData& data,
                       const TJointData& jointData,
                       TCompoundBody* parentRef,
                       const TVec3& localPosition,
                       const TMat3& localRotation );

        /**
        *   Creates a root compound body connected to the world through a given joint
        *
        *   This is one of the constructors that create the root of a chain of compound bodies.
        *   The key difference with this constructor is that it allows the user to attach this
        *   root-body to the world via a joint, which might be required in some scenarios. Notice
        *   that the body is created w.r.t. the base of a compound, that's why the local|relative
        *   position and rotation are given instead of world-space information.
        */
        TCompoundBody( const std::string& name,
                       const TBodyData& data,
                       const TJointData& jointData,
                       const TVec3& localPosition,
                       const TMat3& localRotation );

        /**
        *   Creates a root compound body that could be whether free or fixed w.r.t. the world
        *
        *   This is the other of the constructor that creates a root of a chain of compound bodies.
        *   The key difference with this constructor is that it allows the user to create free and
        *   fixed root bodies w.r.t. the world. This means that you could have bases that can move
        *   around, and bases that remain always in the same position. Notice that the body is created 
        *   w.r.t. the base of a compound, that's why the local|relative position and rotation are 
        *   given instead of world-space information.
        */
        TCompoundBody( const std::string& name,
                       const TBodyData& data,
                       const TVec3& localPosition,
                       const TMat3& localRotation,
                       const eDynamicsType& dyntype );

        ~TCompoundBody();

        void setCompound( TCompound* compoundRef );

        void setParent( TCompoundBody* parentRef );

        void setJoint( std::unique_ptr< TJoint > jointObj );

        std::pair< TCompoundBody*, TJoint* > addBodyJointPair( const std::string& name,
                                                               const TBodyData& bodyData,
                                                               const TJointData& jointData,
                                                               const TMat4& bodyLocalTransform );

        std::pair< TCompoundBody*, TJoint* > addBodyJointPair( const std::string& name,
                                                               const eShapeType& bodyShape,
                                                               const TVec3& bodySize,
                                                               const TVec3& bodyColor,
                                                               const TMat4& bodyLocalTransform,
                                                               const eJointType& jointType,
                                                               const TVec3& jointAxis,
                                                               const TVec2& jointLimits,
                                                               const TMat4& jointLocalTransform );

        TCompound* compound() const { return m_compoundRef; }

        TJoint* joint() { return m_joint.get(); }

        const TJoint* joint() const { return m_joint.get(); }

        TCompoundBody* parent() { return m_parentRef; }

        const TCompoundBody* parent() const { return m_parentRef; }

        bool isRoot() const { return m_parentRef == nullptr; }

        std::vector< TCompoundBody* >& children() { return m_childrenRefs; }

        const std::vector< TCompoundBody* >& children() const { return m_childrenRefs; }

    protected :

        void _preStepInternal() override;
        void _postStepInternal() override;
        void _resetInternal() override;

        void _setPositionInternal( const TVec3& position ) override;
        void _setRotationInternal( const TMat3& rotation ) override;
        void _setEulerInternal( const TVec3& euler ) override;
        void _setQuaternionInternal( const TVec4& quat ) override;
        void _setTransformInternal( const TMat4& transform ) override;

        void _setLocalPositionInternal( const TVec3& position ) override;
        void _setLocalRotationInternal( const TMat3& rotation ) override;
        void _setLocalEulerInternal( const TVec3& euler ) override;
        void _setLocalQuaternionInternal( const TVec4& quat ) override;
        void _setLocalTransformInternal( const TMat4& transform ) override;

    private :

        void _addChildRef( TCompoundBody* childRef );
        void _updateLocalTransform();
        void _preStepComponents();
        void _postStepComponents();
        void _resetComponents();

    private :

        /* Reference to the compound that owns this body */
        TCompound* m_compoundRef;

        /* Single joint connecting this compound-body to a parent compound-body */
        std::unique_ptr< TJoint > m_joint;

        /* Reference to parent compound-body */
        TCompoundBody* m_parentRef;

        /* References to children compound-bodies */
        std::vector< TCompoundBody* > m_childrenRefs;
    };

}