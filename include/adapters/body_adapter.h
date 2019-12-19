#pragma once

#include <components/body.h>

namespace tysoc {

    class TIBody;

    class TIBodyAdapter
    {

    public :

        TIBodyAdapter( TIBody* bodyRef )
            : m_bodyRef( bodyRef ), m_active( true ) {}

        virtual ~TIBodyAdapter() {}

        virtual void build() = 0;

        virtual void reset() = 0;

        virtual void preStep() = 0;

        virtual void postStep() = 0;

        virtual void pause() { m_active = false; };

        virtual void resume() { m_active = true; };

        // @todo: required for backend to handle dynamic deletion (when detached from a TIBody)
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

        /***********************************************************
        *                 Local-space functionality                *
        ***********************************************************/

        virtual void setLocalPosition( const TVec3& position ) = 0;

        virtual void setLocalRotation( const TMat3& rotation ) = 0;

        virtual void setLocalTransform( const TMat4& transform ) = 0;

        virtual void getLocalPosition( TVec3& dstPosition ) = 0;

        virtual void getLocalRotation( TMat3& dstRotation ) = 0;

        virtual void getLocalTransform( TMat4& dstTransform ) = 0;

        bool active() const { return m_active; }

        TIBody* body() const { return m_bodyRef; }

    protected :

        TIBody* m_bodyRef;

        bool m_active;
    };

    /* dl-function to be loaded from the backend support */
    typedef TIBodyAdapter* FcnCreateBodyAdapter( TIBody* bodyRef );

}