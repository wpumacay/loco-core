#pragma once

#include <tysoc_common.h>
#include <components/data.h>
#include <components/body.h>
#include <viz/drawable.h>

#include <adapters/joint_adapter.h>

namespace tysoc {

    class TIBody;
    class TIJointAdapter;

    /**
    *   Joint object, used to connect bodies together
    *
    *   This object defines the functionality exposed to the user to create and interact
    *   with joints. A joint connects two bodies in the following way:
    *
    *       > A child body (owner of this joint) is connected to a parent body (parent 
    *         of the owner) via some type of constraint.
    *       > The constraint imposed reduces the number of degrees of freedom. To keep things
    *         more general, we assume instead that joints "add" degrees of freedom, to reuse
    *         the same specification with kinematic trees, which usually follow the Featherstone
    *         multibody dynamics implementation.
    *
    *   The specification gives access to user to the following:
    *
    *       > Interact (set|get) with the joint DOFs by setting q-values according to the joint-type,
    *         e.g. revolutes expose 1 q-value (joint angle), spherical expose 4 q-values (quaternion), 
    *         and free-joints expose 7 q-values (xyz + quaternion).
    *       > Set|get the relative transform of this joint w.r.t. the owner.
    *       > Set|get the properties of the joint, stored in TJointData and used for the simulation.
    */
    class TJoint
    {

    public :

        TJoint( const std::string& name,
                const TJointData& jointData );

        ~TJoint();

        void setOwnerBody( TIBody* ownerRef );

        void setOwnerParent( TIBody* ownerParentRef );

        void setAdapter( TIJointAdapter* jointImplRef );

        void setDrawable( TIDrawable* drawableImplRef );

        void setLocalTransform( const TMat4& transform );

        void setQpos( const std::vector< TScalar >& qpos );

        void setQvel( const std::vector< TScalar >& qvel );

        std::vector< TScalar > getQpos() const;

        std::vector< TScalar > getQvel() const;

        void changeLimits( const TVec2& limits );

        void update();

        void reset();

        /*************************************************************
        *                      World-space getters                   *
        *************************************************************/

        TVec3 pos() const { return m_pos; }

        TMat3 rot() const { return m_rot; }

        TVec4 quat() const { return m_tf.getRotQuaternion(); }

        TVec3 euler() const { return m_tf.getRotEuler(); }

        TMat4 tf() const { return m_tf; }

        /*************************************************************
        *                      Local-space getters                   *
        *************************************************************/

        TVec3 localPos() const { return m_localPos; }

        TMat3 localRot() const { return m_localRot; }

        TVec4 localQuat() const { return m_localTf.getRotQuaternion(); }

        TVec3 localEuler() const { return m_localTf.getRotEuler(); }

        TMat4 localTf() const { return m_localTf; }

        /*************************************************************
        *                        Helper-getters                      *
        *************************************************************/

        std::string name() const { return m_name; }

        TJointData data() const { return m_data; }

        eJointType type() const { return m_data.type; }

        TVec3 axis() const { return m_data.axis; }

        TVec2 limits() const { return m_data.limits; }

        TScalar limitLow() const { return m_data.limits.x; }

        TScalar limitHigh() const { return m_data.limits.y; }

        TIBody* owner() const { return m_ownerRef; }

        TIBody* ownerParent() const { return m_ownerParentRef; }

    protected :

        /* unique name identifier */
        std::string m_name;

        /* position in world space */
        TVec3 m_pos;
        /* orientation in world space */
        TMat3 m_rot; 
        /* transform in world space */
        TMat4 m_tf;

        /* relative position to owner body */
        TVec3 m_localPos;
        /* relative rotation to owner body */
        TMat3 m_localRot;
        /* relative transform to owner body */
        TMat4 m_localTf;

        /* data of this joint object */
        TJointData m_data;

        /* qpos values: generalized coordinates */
        size_t m_nqpos;
        std::array< TScalar, TYSOC_MAX_NUM_QPOS > m_qpos;

        /* qvel values: speeds of degrees of freedom */
        size_t m_nqvel;
        std::array< TScalar, TYSOC_MAX_NUM_QVEL > m_qvel;

        /* Adapter-object that gives access to the low-level API for a specific backend */
        TIJointAdapter* m_jointImplRef;

        /* reference to the drawable resource used for visualization */
        TIDrawable* m_drawableImplRef;

        /* reference to the owner body (the one that this dof belongs to) */
        TIBody* m_ownerRef;

        /* reference to the owner's parent body */
        TIBody* m_ownerParentRef;
    };

}