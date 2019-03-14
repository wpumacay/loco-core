
#include <sandbox/joint.h>

namespace tysoc {
namespace sandbox {

    TIJoint::TIJoint( const std::string& name,
                      const std::string& type,
                      const TVec3& relpos,
                      const TMat3& relrot,
                      const TVec3& axis,
                      const TVec2& limits )
    {
        m_name = name;
        m_type = type;
        m_relpos = relpos;
        m_relrot = relrot;
        m_axis = axis;
        m_limits = limits;
        m_worldTransform = TMat4();
        m_parentBodyPtr = NULL;

        if ( m_type == "free" )
            m_ndof = 6;
        else if ( m_type == "hinge" || m_type == "slide" )
            m_ndof = 1;
        else if ( m_type == "ball" )
            m_ndof = 3;
    }

    TIJoint::~TIJoint()
    {
        // No data allocated in heap here, so do nothing
    }

    TVec3 TIJoint::relpos()
    {
        return m_relpos;
    }

    TVec3 TIJoint::relRotEuler()
    {
        return TMat3::toEuler( m_relrot );
    }

    TVec4 TIJoint::relRotQuat()
    {
        return TMat3::toQuaternion( m_relrot );
    }

    TMat3 TIJoint::relRotMat()
    {
        return m_relrot;
    }

    TVec3 TIJoint::axis()
    {
        return m_axis;
    }

    TVec3 TIJoint::worldPosition()
    {
        return m_worldTransform.getPosition();
    }

    TMat3 TIJoint::worldRotMat()
    {
        return m_worldTransform.getRotation();
    }

    TVec3 TIJoint::worldRotEuler()
    {
        return TMat3::toEuler( m_worldTransform.getRotation() );
    }

    TVec4 TIJoint::worldRotQuat()
    {
        return TMat3::toQuaternion( m_worldTransform.getRotation() );
    }

    TVec3 TIJoint::worldAxis()
    {
        auto _rotmat = m_worldTransform.getRotation();

        return _rotmat * m_axis;
    }

    TMat4 TIJoint::worldTransform()
    {
        return m_worldTransform;
    }

    std::string TIJoint::name()
    {
        return m_name;
    }

    std::string TIJoint::type()
    {
        return m_type;
    }

    size_t TIJoint::numDof()
    {
        return m_ndof;
    }

    TVec2 TIJoint::limits()
    {
        return m_limits;
    }

    TIBody* TIJoint::parentBody()
    {
        return m_parentBodyPtr;
    }



}}