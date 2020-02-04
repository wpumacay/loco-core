#pragma once

#include <components/collision.h>

namespace loco
{
    class TCollision;

    class TICollisionAdapter
    {
        public :

        TICollisionAdapter( TCollision* collisionPtr ) { m_collisionPtr = collisionPtr; }

        virtual ~TICollisionAdapter() {}

        virtual void build() = 0;

        virtual void reset() = 0;

        virtual void preStep() = 0;

        virtual void postStep() = 0;

        // @todo: required for backend to handle dynamic deletion (when detached from a TCollision)
        virtual void detach() {}

        virtual void setLocalPosition( const TVec3& position ) = 0;

        virtual void setLocalRotation( const TMat3& rotation ) = 0;

        virtual void setLocalTransform( const TMat4& transform ) = 0;

        virtual void changeSize( const TVec3& newSize ) = 0;

        virtual void changeElevationData( const std::vector<float>& heightData ) = 0;

        virtual void changeCollisionGroup( int collisionGroup ) = 0;

        virtual void changeCollisionMask( int collisionMask ) = 0;

        TCollision* collision() { return m_collisionPtr; }

        protected :

        TCollision* m_collisionPtr;
    };

    /* dl-function to be loaded from the backend support */
    typedef TICollisionAdapter* FcnCreateCollisionAdapter( TCollision* collisionPtr );
}