#pragma once

#include <tysoc_common.h>

#include <components/data.h>
#include <components/body.h>

#include <viz/drawable.h>
#include <adapters/collision_adapter.h>

namespace tysoc {

    class TIBody;
    class TICollisionAdapter;

    class TCollision
    {

    public :

        TCollision( const std::string& name,
                    const TCollisionData& collisionData );

        ~TCollision();

        void setParentBody( TIBody* parentBodyRef );

        void setAdapter( TICollisionAdapter* collisionImplRef );

        void setDrawable( TIDrawable* drawableImplRef );

        void show( bool visible );

        void wireframe( bool wireframe );

        void preStep();

        void postStep();

        void reset();

        void setLocalPosition( const TVec3& localPosition );

        void setLocalRotation( const TMat3& localRotation );

        void setLocalEuler( const TVec3& localEuler );

        void setLocalQuat( const TVec4& localQuaternion );

        void setLocalTransform( const TMat4& transform );

        void changeSize( const TVec3& newSize );

        void changeElevationData( const std::vector< float >& heightData );

        std::string name() const { return m_name; }

        bool isVisible() const;

        bool isWireframe() const;

        TVec3 pos() const { return TVec3( m_tf.col( 3 ) ); }

        TMat3 rot() const { return TMat3( m_tf ); }

        TVec3 euler() const { return tinymath::euler( m_tf ); }

        TVec4 quat() const { return tinymath::quaternion( m_tf ); }

        TMat4 tf() const { return m_tf; }

        TVec3 localPos() const { return TVec3( m_localTf.col( 3 ) ); }

        TMat3 localRot() const { return TMat3( m_localTf ); }

        TVec3 localEuler() const { return tinymath::euler( m_localTf ); }

        TVec4 localQuat() const { return tinymath::quaternion( m_localTf ); }

        TMat4 localTf() const { return m_localTf; }

        eShapeType shape() const { return m_data.type; }

        TVec3 size() const { return m_data.size; }

        TCollisionData data() const { return m_data; }

        TCollisionData& dataRef() { return m_data; }

        const TCollisionData& dataRef() const { return m_data; }

        TIBody* parent() { return m_parentBodyRef; }

        const TIBody* parent() const { return m_parentBodyRef; }

        TICollisionAdapter* adapter() { return m_collisionImplRef; }

    protected :

        /// Unique name identifier
        std::string m_name;
        /// Transform in world-space
        TMat4 m_tf;
        /// Relative transform w.r.t. parent body
        TMat4 m_localTf;
        /// Reference to the parent-body of this collision object
        TIBody* m_parentBodyRef;
        /// Construction data of this collision object
        TCollisionData m_data;
        /// Reference to the drawable resource used for visualization
        TIDrawable* m_drawableImplRef;
        /// Adapter object that gives access to the low-level API for a specific backend
        TICollisionAdapter* m_collisionImplRef;
    };

}