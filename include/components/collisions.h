#pragma once

#include <tysoc_common.h>

#include <components/data.h>
#include <components/bodies.h>

#include <viz/drawable.h>
#include <adapters/collision_adapter.h>

namespace tysoc {

    class TBody;
    class TICollisionAdapter;

    class TCollision
    {

    public :

        TCollision( const std::string& name,
                    const TCollisionData& collisionData );

        ~TCollision();

        void setParentBody( TBody* parentBodyPtr );

        void setAdapter( TICollisionAdapter* collisionImplPtr );

        void setDrawable( TIDrawable* drawableImplPtr );

        void show( bool visible );

        void wireframe( bool wireframe );

        void update();

        void reset();

        void setLocalPosition( const TVec3& localPosition );

        void setLocalRotation( const TMat3& localRotation );

        void setLocalQuat( const TVec4& localQuaternion );

        void setLocalTransform( const TMat4& transform );

        void changeSize( const TVec3& newSize );

        void changeElevationData( const std::vector< float >& heightData );

        std::string name() { return m_name; }

        bool isVisible();

        bool isWireframe();

        TVec3 pos() { return m_pos; }

        TMat3 rot() { return m_rot; }

        TVec3 euler() { return m_tf.getRotEuler(); }

        TVec4 quat() { return m_tf.getRotQuaternion(); }

        TMat4 tf() { return m_tf; }

        TVec3 localPos() { return m_localPos; }

        TMat3 localRot() { return m_localRot; }

        TVec3 localEuler() { return m_localTf.getRotEuler(); }

        TVec4 localQuat() { return m_localTf.getRotQuaternion(); }

        TMat4 localTf() { return m_localTf; }

        eShapeType shape() { return m_data.type; }

        TVec3 size() { return m_data.size; }

        TCollisionData data() { return m_data; }

        TBody* parent() { return m_parentBodyPtr; }

        TICollisionAdapter* adapter() { return m_collisionImplPtr; }

    protected :

        /* unique name identifier */
        std::string m_name;

        /* relative position w.r.t. parent body */
        TVec3 m_localPos;
        /* relative rotation w.r.t. parent body */
        TMat3 m_localRot;
        /* relative transform w.r.t. parent body */
        TMat4 m_localTf;

        /* position in world-space */
        TVec3 m_pos;
        /* rotation in world-space */
        TMat3 m_rot;
        /* transform in world-space */
        TMat4 m_tf;

        /* reference to the parent-body of this collision object */
        TBody* m_parentBodyPtr;

        /* construction data of this collision object */
        TCollisionData m_data;

        /* reference to the drawable resource used for visualization */
        TIDrawable* m_drawableImplPtr;

        /* Adapter object that gives access to the low-level API for a specific backend */
        TICollisionAdapter* m_collisionImplPtr;
    };

}