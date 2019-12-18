#pragma once

#include <compounds/compound.h>

namespace tysoc
{

    class TCompound;

    class TICompoundAdapter
    {

   public :

        TICompoundAdapter( TCompound* compoundRef )
            : m_compoundRef( compoundRef ), m_active( true ) {}

        virtual ~TICompoundAdapter() {}

        virtual void build() = 0;

        virtual void reset() = 0;

        virtual void preStep() = 0;

        virtual void postStep() = 0;

        // @todo: required to notify the backend to pause simulating this adapter's compound
        virtual void pause() { m_active = true; };

        // @todo: required to notify the backend to continue simulating this adapter's compound
        virtual void resume() { m_active = false; };

        // @todo: required for backend to handle dynamic deletion (when detached from a TCompound)
        virtual void detach() {}

        /************************************************************
        *                 World-space functionality                 *
        ************************************************************/

        virtual void setPosition( const TVec3& position ) = 0;

        virtual void setRotation( const TMat3& rotation ) = 0;

        virtual void setTransform( const TMat4& transform ) = 0;

        virtual void getPosition( TVec3& dstPosition ) = 0;

        virtual void getRotation( TMat3& dstRotation ) = 0;

        virtual void getTransform( TMat4& dstTransform ) = 0;

        bool active() const { return m_active; }

        TCompound* compound() const { return m_compoundRef; }

    protected :

        TCompound* m_compoundRef;

        bool m_active;
    };

    /* dl-function to be loaded from the backend support */
    typedef TICompoundAdapter* FcnCreateCompoundAdapter( TCompound* compoundRef );
}