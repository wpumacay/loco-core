#pragma once

#include <components/loco_joint.h>

namespace loco
{
    class TJoint;

    class TIJointAdapter
    {
    public :

        TIJointAdapter( TJoint* jointRef )
            :  m_jointRef( jointRef ) {}

        virtual ~TIJointAdapter() {}

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void Reset() = 0;

        virtual void PreStep() = 0;

        virtual void PostStep() = 0;

        // @todo: required for backend to handle dynamic deletion (when detached from a TJoint)
        virtual void Detach() {}

        virtual void SetLocalPosition( const TVec3& position ) = 0;

        virtual void SetLocalRotation( const TMat3& rotation ) = 0;

        virtual void SetLocalTransform( const TMat4& transform ) = 0;

        virtual void SetQpos( const std::array< TScalar, MAX_NUM_QPOS >& qpos ) = 0;

        virtual void SetQvel( const std::array< TScalar, MAX_NUM_QVEL >& qvel ) = 0;

        virtual void GetQpos( std::array< TScalar, MAX_NUM_QPOS >& dstQpos ) = 0;

        virtual void GetQvel( std::array< TScalar, MAX_NUM_QVEL >& dstQvel ) = 0;

        virtual void ChangeLimits( const TVec2& limits ) = 0;

        TJoint* joint() { return m_jointRef; }

        const TJoint* joint() const { return m_jointRef; }

    protected :

        TJoint* m_jointRef;
    };

    /* dl-function to be loaded from the backend support */
    typedef TIJointAdapter* FcnCreateJointAdapter( TJoint* jointRef );
}