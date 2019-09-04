#pragma once

#include <tysoc_common.h>
#include <components/data.h>
#include <viz/drawable.h>

namespace tysoc {


    class TCollision
    {

    public :

        TCollision( const std::string& name,
                    const TCollisionData& collisionData );

        virtual ~TCollision();

        void setDrawable( viz::TIDrawable* drawablePtr );

        void show( bool visible );

        virtual void build();

        virtual void update();

        virtual void reset();

        virtual void changeSize( const TVec3& newSize );

        std::string name() { return m_name; }

        TVec3 pos() { return m_pos; }

        TMat3 rot() { return m_rot; }

        TMat4 tf() { return m_tf; }

        eShapeType shape() { return m_data.type; }

        TVec3 size() { return m_data.size; }

        TCollisionData data() { return m_data; }

    protected :

        std::string m_name;

        TVec3 m_localPos;
        TMat3 m_localRot;
        TMat4 m_localTf;

        TVec3 m_pos;
        TMat3 m_rot;
        TMat4 m_tf;

        TCollisionData m_data;

        viz::TIDrawable* m_drawablePtr;
    };

}