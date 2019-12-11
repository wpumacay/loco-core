#pragma once

#include <components/joint.h>

namespace tysoc {

    class TJoint;

    class TIJointAdapter
    {

    public :

        TIJointAdapter( TJoint* jointRef ) { m_jointRef = jointRef; }

        virtual ~TIJointAdapter() {}

        virtual void build() = 0;

        virtual void reset() = 0;

        virtual void update() = 0;

        // @todo: required for backend to handle dynamic deletion (when detached from a TJoint)
        virtual void detach() {}

        virtual void setLocalPosition( const TVec3& position ) = 0;

        virtual void setLocalRotation( const TMat3& rotation ) = 0;

        virtual void setLocalTransform( const TMat4& transform ) = 0;

        virtual void setQpos( const std::array< TScalar, TYSOC_MAX_NUM_QPOS >& qpos ) = 0;

        virtual void setQvel( const std::array< TScalar, TYSOC_MAX_NUM_QVEL >& qvel ) = 0;

        virtual void getQpos( std::array< TScalar, TYSOC_MAX_NUM_QPOS >& dstQpos ) = 0;

        virtual void getQvel( std::array< TScalar, TYSOC_MAS_NUM_QVEL >& dstQvel ) = 0;

        virtual void changeLimits( const TVec2& limits ) = 0;

        TJoint* joint() { return m_jointRef; }

    protected :

        TJoint* m_jointRef;
    };

    /* dl-function to be loaded from the backend support */
    typedef TIJointAdapter* FcnCreateJointAdapter( TJoint* jointRef );

}