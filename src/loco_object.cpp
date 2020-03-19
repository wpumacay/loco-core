
#include <loco_object.h>

namespace loco
{
    std::string ToString( const eObjectType& obj_type )
    {
        if ( obj_type == eObjectType::BASE ) return "base";
        if ( obj_type == eObjectType::DRAWABLE ) return "drawable";
        if ( obj_type == eObjectType::SINGLE_BODY ) return "single_body";
        if ( obj_type == eObjectType::SINGLE_BODY_COLLIDER ) return "single_body_collider";

        LOCO_CORE_ERROR( "ToString(eObjectType) >>> undefined object type" );
        return "undefined";
    }

    void TObject::Initialize()
    {
        _InitializeInternal();
    }

    void TObject::PreStep()
    {
        _PreStepInternal();
    }

    void TObject::PostStep()
    {
        _PostStepInternal();
    }

    void TObject::Reset()
    {
        _ResetInternal();
    }

    void TObject::DetachSim()
    {
        _DetachSimInternal();
    }

    void TObject::DetachViz()
    {
        _DetachVizInternal();
    }

    void TObject::SetPosition( const TVec3& position )
    {
        m_tf.set( position, 3 );
        _SetTransformInternal( m_tf );
    }

    void TObject::SetRotation( const TMat3& rotation )
    {
        m_tf.set( rotation );
        _SetTransformInternal( m_tf );
    }

    void TObject::SetEuler( const TVec3& euler )
    {
        m_tf.set( tinymath::rotation( euler ) );
        _SetTransformInternal( m_tf );
    }

    void TObject::SetQuaternion( const TVec4& quaternion )
    {
        m_tf.set( tinymath::rotation( quaternion ) );
        _SetTransformInternal( m_tf );
    }

    void TObject::SetTransform( const TMat4& transform )
    {
        m_tf = transform;
        _SetTransformInternal( m_tf );
    }
}