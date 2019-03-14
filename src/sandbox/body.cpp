
#include <sandbox/body.h>

namespace tysoc {
namespace sandbox {



    TIBody::TIBody( const std::string& name,
                    const std::string& type,
                    const TVec3& position,
                    const TMat3& rotation,
                    const TVec3& size,
                    const std::string& filename )
    {
        m_name = name;
        m_type = type;
        m_position = position;
        m_rotation = rotation;
        m_size = size;
        m_filename = filename;
    }

    TIBody::~TIBody()
    {
        for ( size_t i = 0; i < m_joints.size(); i++ )
        {
            delete m_joints[i];
        }
        m_joints.clear();
    }

    void TIBody::setPosition( const TVec3& position )
    {
        m_position = position;
        _changePosition();
    }

    void TIBody::setRotMat( const TMat3& rotmat )
    {
        m_rotation = rotmat;
        _changeRotation();
    }

    void TIBody::setRotEuler( const TVec3& euler )
    {
        m_rotation = TMat3::fromEuler( euler );
        _changeRotation();
    }

    void TIBody::setRotQuat( const TVec4& quat )
    {
        m_rotation = TMat3::fromQuaternion( quat );
        _changeRotation();
    }

    void TIBody::setSize( const TVec3& size )
    {
        m_size = size;
        _changeSize();
    }

    TVec3 TIBody::position()
    {
        return m_position;
    }

    TMat3 TIBody::rotMat()
    {
        return m_rotation;
    }

    TVec3 TIBody::rotEuler()
    {
        return TMat3::toEuler( m_rotation );
    }

    TVec4 TIBody::rotQuat()
    {
        return TMat3::toQuaternion( m_rotation );
    }

    TVec3 TIBody::size()
    {
        return m_size;
    }

    std::string TIBody::type()
    {
        return m_type;
    }

    std::string TIBody::name()
    {
        return m_name;
    }

    std::string TIBody::filename()
    {
        return m_filename;
    }

    TScalar TIBody::mass()
    {
        return m_mass;
    }

    TMat3 TIBody::inertia()
    {
        return m_inertia;
    }

    TVec3 TIBody::friction()
    {
        return m_friction;
    }

    void TIBody::update()
    {
        _updateInternal();
    }

    void TIBody::addJoint( TIJoint* jointPtr )
    {
        m_joints.push_back( jointPtr );
    }

    std::vector< TIJoint* > TIBody::joints()
    {
        return m_joints;
    }

}}