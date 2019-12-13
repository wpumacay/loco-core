
#include <pytysoc_joint.h>

namespace py = pybind11;

namespace pytysoc
{

    PyJoint::PyJoint( const std::string& name,
                      const PyJointData& jointData )
    {
        m_ownerRef = nullptr;
        m_ownerParentRef = nullptr;

        m_joint = new tysoc::TJoint( name, toTJointData( jointData ) );
    }

    PyJoint::PyJoint( tysoc::TJoint* jointRef )
    {
        m_ownerRef = nullptr;
        m_ownerParentRef = nullptr;

        m_joint = jointRef;
    }

    PyJoint::~PyJoint()
    {
        m_ownerRef = nullptr;
        m_ownerParentRef = nullptr;
    }

    void PyJoint::setOwner( PyBody* ownerRef )
    {
        if ( !ownerRef )
        {
            TYSOC_CORE_ERROR( "PyJoint::setOwner >>> tried setting nullptr as owner of joint \"{0}\"", m_joint->name() );
            return;
        }

        if ( !ownerRef->ptr() )
        {
            TYSOC_CORE_ERROR( "PyJoint::setOwner >>> tried setting as owner of joint \"{0}\" an invalid (null-internal) body", m_joint->name() );
            return;
        }

        m_ownerRef = ownerRef;
        m_joint->setOwnerBody( m_ownerRef->ptr() );
    }

    void PyJoint::setOwnerParent( PyBody* ownerParentRef )
    {
        if ( !ownerParentRef )
        {
            TYSOC_CORE_ERROR( "PyJoint::setOwnerParent >>> tried setting nullptr as owner's parent of joint \"{0}\"", m_joint->name() );
            return;
        }

        if ( !ownerParentRef->ptr() )
        {
            TYSOC_CORE_ERROR( "PyJoint::setOwnerParent >>> tried setting as owner's parent of joint \"{0}\" an invalid (null-internal) body", m_joint->name() );
            return;
        }

        m_ownerParentRef = ownerParentRef;
        m_joint->setOwnerParent( m_ownerParentRef->ptr() );
    }

    void PyJoint::setLocalTransform( py::array_t<TScalar>& localTransform )
    {
        if ( !m_joint )
            return;

        m_joint->setLocalTransform( numpyToMat4( localTransform ) );
    }

    void PyJoint::setQpos( py::array_t<TScalar>& qpos )
    {
        if ( !m_joint )
            return;

        m_joint->setQpos( numpyToVecArray( qpos ) );
    }

    void PyJoint::setQvel( py::array_t<TScalar>& qvel )
    {
        if ( !m_joint )
            return;

        m_joint->setQvel( numpyToVecArray( qvel ) );
    }

    PyJointData PyJoint::data() const 
    {
        TYSOC_CORE_ASSERT( m_joint, "PyJoint::data() >>> wrapper contains null reference to a body" );

        return toPyJointData( m_joint->data() );
    }

}