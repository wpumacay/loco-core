#pragma once

#include <components/bodies.h>

namespace tysoc {

    class TBody;

    class TIBodyAdapter
    {

    public :

        TIBodyAdapter( TBody* bodyPtr ) { m_bodyPtr = bodyPtr; }

        virtual ~TIBodyAdapter() {}

        virtual void build() = 0;

        virtual void reset() = 0;

        virtual void update() = 0;

        virtual void setPosition( const TVec3& position ) = 0;

        virtual void setRotation( const TMat3& rotation ) = 0;

        virtual void setTransform( const TMat4& transform ) = 0;

        virtual void getPosition( TVec3& dstPosition ) = 0;

        virtual void getRotation( TMat3& dstRotation ) = 0;

        virtual void getTransform( TMat4& dstTransform ) = 0;

        TBody* body() { return m_bodyPtr; }

    protected :

        TBody* m_bodyPtr;
    };

    /* dl-function to be loaded from the backend support */
    typedef TIBodyAdapter* FcnCreateBodyAdapter( TBody* bodyPtr );

}