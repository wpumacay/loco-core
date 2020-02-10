#pragma once

#include <components/loco_body_base.h>

namespace loco
{
    class TIBody;

    class TIBodyAdapter
    {
    public :

        TIBodyAdapter( TIBody* bodyRef )
            : m_bodyRef( bodyRef ), m_active( true ) {}

        virtual ~TIBodyAdapter() {}

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void Reset() = 0;

        virtual void PreStep() = 0;

        virtual void PostStep() = 0;

        virtual void Pause() { m_active = false; };

        virtual void Resume() { m_active = true; };

        // @todo: required for backend to handle dynamic deletion (when detached from a TIBody)
        virtual void Detach() {}

        /************************************************************
        *                 World-space functionality                 *
        ************************************************************/

        virtual void SetPosition( const TVec3& position ) = 0;

        virtual void SetRotation( const TMat3& rotation ) = 0;

        virtual void SetTransform( const TMat4& transform ) = 0;

        virtual void GetPosition( TVec3& dstPosition ) = 0;

        virtual void GetRotation( TMat3& dstRotation ) = 0;

        virtual void GetTransform( TMat4& dstTransform ) = 0;

        /***********************************************************
        *                 Local-space functionality                *
        ***********************************************************/

        virtual void SetLocalPosition( const TVec3& position ) = 0;

        virtual void SetLocalRotation( const TMat3& rotation ) = 0;

        virtual void SetLocalTransform( const TMat4& transform ) = 0;

        virtual void GetLocalPosition( TVec3& dstPosition ) = 0;

        virtual void GetLocalRotation( TMat3& dstRotation ) = 0;

        virtual void GetLocalTransform( TMat4& dstTransform ) = 0;

        bool active() const { return m_active; }

        TIBody* body() { return m_bodyRef; }

        const TIBody* body() const { return m_bodyRef; }

    protected :

        TIBody* m_bodyRef;

        bool m_active;
    };

    /* dl-function to be loaded from the backend support */
    typedef TIBodyAdapter* FcnCreateBodyAdapter( TIBody* bodyRef );
}