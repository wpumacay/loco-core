#pragma once

#include <components/joint.h>
#include <components/body.h>

namespace tysoc {

    class TCollision;
    class TVisual;
    class TJoint;

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
        *   root-body to the world via a joint, which might be required in some scenarios.
        */
        TCompoundBody( const std::string& name,
                       const TBodyData& data,
                       const TJointData& jointData,
                       const TVec3& position,
                       const TMat3& rotation );

        /**
        *   Creates a root compound body that could be whether free or fixed w.r.t. the world
        *
        *   This is the other of the constructor that creates a root of a chain of compound bodies.
        *   The key difference with this constructor is that it allows the user to create free and
        *   fixed root bodies w.r.t. the world. This means that you could have bases that can move
        *   around, and bases that remain always in the same position.
        */
        TCompoundBody( const std::string& name,
                       const TBodyData& data,
                       const TVec3& position,
                       const TMat3& rotation,
                       const eDynamicsType& dyntype = eDynamicsType::DYNAMIC );

        ~TCompoundBody();

        void setParent( TCompoundBody* parentRef );

        void setJoint( std::unique_ptr< TJoint > jointObj );

        void update() override;

        void reset() override;

        void setPosition( const TVec3& position ) override;

        void setRotation( const TMat3& rotation ) override;

        void setEuler( const TVec3& euler ) override;

        void setQuaternion( const TVec4& quat ) override;

        void setTransform( const TMat4& transform ) override;

        void setLocalPosition( const TVec3& position ) override;

        void setLocalRotation( const TMat3& rotation ) override;

        void setLocalEuler( const TVec3& euler ) override;

        void setLocalQuaternion( const TVec4& quat ) override;

        void setLocalTransform( const TMat4& transform ) override;

        TJoint* joint() const { return m_joint.get(); }

        TCompoundBody* parent() const { return m_parentRef; }

    private :

        /* Single joint connecting this compound-body to a parent compound-body */
        std::unique_ptr< TJoint > m_joint;

        /* Reference to parent compound-body */
        TCompoundBody* m_parentRef;
    };

}