#pragma once

#include <tysoc_common.h>
#include <components/data.h>

namespace tysoc {

    class TBody;

    class TJoint
    {

    public :

        TJoint( const std::string& name,
                const TJointData& jointData );

        virtual ~TJoint();

        void setOwnerBody( TBody* ownerPtr );

        void setOwnerParent( TBody* ownerParentPtr );

        void setAdapter( TIJointAdapter* jointImplPtr );

        void setDrawable( TIDrawable* drawableImplPtr );

        void update();

        void reset();

        void setQpos( const std::vector< TScalar >& qpos );

        std::string name() const { return m_name; }

        TVec3 pos() { return m_pos; }

        TMat3 rot() { return m_rot; }

        TVec4 quat() { return m_tf.getRotQuaternion(); }

        TVec3 euler() { return m_tf.getRotEuler(); }

        TMat4 tf() { return m_tf; }

        TJointData data() const { return m_data; }

        eJointType type() const { return m_data.type; }

        TVec3 axis() const { return m_data.axis; }

        TVec2 limits() const { return m_data.limits; }

        TScalar limitLow() const { return m_data.limits.x; }

        TScalar limitHigh() const { return m_data.limits.y; }

        TBody* owner() const { return m_ownerPtr; }

        TBody* ownerParent() const { return m_ownerParentPtr; }

    protected :

        /* unique name identifier */
        std::string m_name;

        /* position in world space */
        TVec3 m_pos;
        /* orientation in world space */
        TMat3 m_rot; 
        /* transform in world space */
        TMat4 m_tf;

        /* data of this joint object */
        TJointData m_data;

        /* Adapter-object that gives access to the low-level API for a specific backend */
        TIJointAdapter* m_jointImpl;

        /* reference to the drawable resource used for visualization */
        TIDrawable* m_drawableImplPtr;

        /* reference to the owner body (the one that this dof belongs to) */
        TBody* m_ownerPtr;

        /* reference to the owner's parent body */
        TBody* m_ownerParentPtr;
    };

}