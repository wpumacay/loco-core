#pragma once

#include <components/joint.h>

namespace tysoc {

    class TJoint;

    class TIJointAdapter
    {

    public :

        TIJointAdapter( TJoint* jointPtr ) { m_jointPtr = jointPtr; }

        virtual ~TIJointAdapter() {}

        virtual void build() = 0;

        virtual void reset() = 0;

        virtual void update() = 0;

        virtual void setLocalPosition( const TVec3& position ) = 0;

        virtual void setLocalRotation( const TMat3& rotation ) = 0;

        virtual void setLocalTransform( const TMat4& transform ) = 0;

        virtual void setQpos( const std::vector< TScalar >& qpos ) = 0;

        virtual void getQpos( std::vector< TScalar >& dstQpos ) = 0;

        virtual void changeLimits( const TVec2& limits ) = 0;

        TJoint* joint() { return m_jointPtr; }

    protected :

        TJoint* m_jointPtr;
    };

    /* dl-function to be loaded from the backend support */
    typedef TIJointAdapter* FcnCreateJointAdapter( TJoint* jointPtr );

}