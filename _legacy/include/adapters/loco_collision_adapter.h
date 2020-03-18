#pragma once

#include <components/loco_collision.h>

namespace loco
{
    class TCollision;

    class TICollisionAdapter
    {
    public :

        TICollisionAdapter( TCollision* collisionRef )
            : m_collisionRef( collisionRef ) {}

        virtual ~TICollisionAdapter() {}

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void PreStep() = 0;

        virtual void PostStep() = 0;

        virtual void Reset() = 0;

        // @todo: required for backend to handle dynamic deletion (when detached from a TCollision)
        virtual void OnDetach() {}

        virtual void SetLocalPosition( const TVec3& position ) = 0;

        virtual void SetLocalRotation( const TMat3& rotation ) = 0;

        virtual void SetLocalTransform( const TMat4& transform ) = 0;

        virtual void ChangeSize( const TVec3& newSize ) = 0;

        virtual void ChangeElevationData( const std::vector<float>& heightData ) = 0;

        virtual void ChangeCollisionGroup( int collisionGroup ) = 0;

        virtual void ChangeCollisionMask( int collisionMask ) = 0;

        TCollision* collision() { return m_collisionRef; }

        const TCollision* collision() const { return m_collisionRef; }

    protected :

        TCollision* m_collisionRef;
    };

    /* dl-function to be loaded from the backend support */
    typedef TICollisionAdapter* FcnCreateCollisionAdapter( TCollision* collisionRef );
}