#pragma once

#include <loco_common.h>
#include <components/loco_data.h>
#include <components/loco_body_base.h>
#include <visualizer/loco_drawable.h>
#include <adapters/loco_joint_adapter.h>

namespace loco
{
    class TIBody;
    class TIJointAdapter;

    /// Joint object, used to connect bodies together
    ///
    /// This object defines the functionality exposed to the user to create and interact
    /// with joints. A joint connects two bodies in the following way:
    ///
    ///    > A child body (owner of this joint) is connected to a parent body (parent 
    ///      of the owner) via some type of constraint.
    ///    > The constraint imposed reduces the number of degrees of freedom. To keep things
    ///      more general, we assume instead that joints "add" degrees of freedom, to reuse
    ///      the same specification with kinematic trees, which usually follow the Featherstone
    ///      multibody dynamics implementation.
    ///
    /// The specification gives access to user to the following:
    ///
    ///    > Interact (set|get) with the joint DOFs by setting q-values according to the joint-type,
    ///      e.g. revolutes expose 1 q-value (joint angle), spherical expose 4 q-values (quaternion), 
    ///      and free-joints expose 7 q-values (xyz + quaternion).
    ///    > Set|get the relative transform of this joint w.r.t. the owner.
    ///    > Set|get the properties of the joint, stored in TJointData and used for the simulation.
    class TJoint
    {
    public :

        TJoint( const std::string& name,
                const TJointData& jointData );

        ~TJoint();

        void SetOwnerBody( TIBody* ownerRef );

        void SetOwnerParent( TIBody* ownerParentRef );

        void SetAdapter( TIJointAdapter* jointImplRef );

        void SetDrawable( TIDrawable* drawableImplRef );

        void SetLocalPosition( const TVec3& localPosition );

        void SetLocalRotation( const TMat3& localRotation );

        void SetLocalEuler( const TVec3& localEuler );

        void SetLocalQuat( const TVec4& localQuaternion );

        void SetLocalTransform( const TMat4& transform );

        void SetQpos( const std::vector< TScalar >& qpos );

        void SetQvel( const std::vector< TScalar >& qvel );

        std::vector< TScalar > GetQpos() const;

        std::vector< TScalar > GetQvel() const;

        std::vector< TScalar > GetQpos0() const;

        std::vector< TScalar > GetQvel0() const;

        void ChangeLimits( const TVec2& limits );

        void Initialize();

        void PreStep();

        void PostStep();

        void Reset();

        /*************************************************************
        *                      World-space getters                   *
        *************************************************************/

        TVec3 pos() const { return TVec3( m_tf.col( 3 ) ); }

        TMat3 rot() const { return TMat3( m_tf ); }

        TVec4 quat() const { return tinymath::quaternion( m_tf ); }

        TVec3 euler() const { return tinymath::euler( m_tf ); }

        TMat4 tf() const { return m_tf; }

        /*************************************************************
        *                      Local-space getters                   *
        *************************************************************/

        TVec3 localPos() const { return TVec3( m_localTf.col( 3 ) ); }

        TMat3 localRot() const { return TMat3( m_localTf ); }

        TVec4 localQuat() const { return tinymath::quaternion( m_localTf ); }

        TVec3 localEuler() const { return tinymath::euler( m_localTf ); }

        TMat4 localTf() const { return m_localTf; }

        /*************************************************************
        *                        Helper-getters                      *
        *************************************************************/

        std::string name() const { return m_name; }

        TJointData& data() { return m_data; }

        const TJointData& data() const { return m_data; }

        eJointType type() const { return m_data.type; }

        TVec3 axis() const { return m_data.axis; }

        TVec2 limits() const { return m_data.limits; }

        TScalar limitLow() const { return m_data.limits.x(); }

        TScalar limitHigh() const { return m_data.limits.y(); }

        TIJointAdapter* adapter() { return m_jointImplRef; }

        const TIJointAdapter* adapter() const { return m_jointImplRef; }

        TIBody* owner() { return m_ownerRef; }

        const TIBody* owner() const { return m_ownerRef; }

        TIBody* ownerParent() { return m_ownerParentRef; }

        const TIBody* ownerParent() const { return m_ownerParentRef; }

    protected :

        // Unique name identifier
        std::string m_name;
        // Transform in world space
        TMat4 m_tf;
        // Relative transform to owner body
        TMat4 m_localTf;
        // Properties(struct) of this joint
        TJointData m_data;
        // Number of generalized coordinates (qpos) associated with this joint
        size_t m_nqpos;
        // Generalized coordinates for this joint
        std::array< TScalar, MAX_NUM_QPOS > m_qpos;
        // Initial values of the generalized coordinates for this joint
        std::array< TScalar, MAX_NUM_QPOS > m_qpos0;
        // Number of degrees of freedom (qvel) associated with this joint
        size_t m_nqvel;
        // QVel values for this joint
        std::array< TScalar, MAX_NUM_QVEL > m_qvel;
        // Initial QVel values for this joint
        std::array< TScalar, MAX_NUM_QVEL > m_qvel0;
        // Adapter-object that gives access to the low-level API for a specific backend
        TIJointAdapter* m_jointImplRef;
        // Reference to the drawable resource used for visualization
        TIDrawable* m_drawableImplRef;
        // Reference to the owner body (the one that this dof belongs to)
        TIBody* m_ownerRef;
        // Reference to the owner's parent body
        TIBody* m_ownerParentRef;
    };
}