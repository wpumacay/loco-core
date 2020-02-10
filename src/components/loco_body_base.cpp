
#include <components/loco_body_base.h>

namespace loco
{
    TIBody::TIBody( const std::string& name,
                    const TBodyData& data )
    {
        m_name = name;
        m_data = data;
        m_bodyImplRef = nullptr;
        m_classType = eBodyClassType::NONE;

        m_collision = std::make_unique<TCollision>( m_name + "_col", m_data.collision );
        m_collision->SetParentBody( this );

        m_visual = std::make_unique<TVisual>( m_name + "_viz", m_data.visual );
        m_visual->SetParentBody( this );
    }

    TIBody::~TIBody()
    {
        m_bodyImplRef = nullptr;
        m_collision = nullptr;
        m_visual = nullptr;
    }

    void TIBody::SetAdapter( TIBodyAdapter* bodyImplRef )
    {
        m_bodyImplRef = bodyImplRef;
    }

    void TIBody::SetCollision( std::unique_ptr< TCollision > collisionObj )
    {
        if ( !collisionObj )
            return;

        m_collision = std::move( collisionObj );
        m_collision->SetParentBody( this );
        m_data.collision = m_collision->data();
    }

    void TIBody::SetVisual( std::unique_ptr< TVisual > visualObj )
    {
        if ( !visualObj )
            return;

        m_visual = std::move( visualObj );
        m_visual->SetParentBody( this );
        m_data.visual = m_visual->data();
    }

    void TIBody::Initialize()
    {
        _InitializeInternal();
    }

    void TIBody::PreStep()
    {
        _PreStepInternal();
    }

    void TIBody::PostStep()
    {
        _PostStepInternal();
    }

    void TIBody::Reset()
    {
        _ResetInternal();
    }

    ////////////////////////////////////////////////////////////
    //                   World-space setters                  //
    ////////////////////////////////////////////////////////////

    void TIBody::SetPosition( const TVec3& position )
    {
        _SetPositionInternal( position );
    }

    void TIBody::SetRotation( const TMat3& rotation )
    {
        _SetRotationInternal( rotation );
    }

    void TIBody::SetEuler( const TVec3& euler )
    {
        _SetEulerInternal( euler );
    }

    void TIBody::SetQuaternion( const TVec4& quat )
    {
        _SetQuaternionInternal( quat );
    }

    void TIBody::SetTransform( const TMat4& transform )
    {
        _SetTransformInternal( transform );
    }

    ////////////////////////////////////////////////////////////
    //                   Local-space setters                  //
    ////////////////////////////////////////////////////////////

    void TIBody::SetLocalPosition( const TVec3& localPosition )
    {
        _SetLocalPositionInternal( localPosition );
    }

    void TIBody::SetLocalRotation( const TMat3& localRotation )
    {
        _SetLocalRotationInternal( localRotation );
    }

    void TIBody::SetLocalEuler( const TVec3& localEuler )
    {
        _SetLocalEulerInternal( localEuler );
    }

    void TIBody::SetLocalQuaternion( const TVec4& localQuat )
    {
        _SetLocalQuaternionInternal( localQuat );
    }

    void TIBody::SetLocalTransform( const TMat4& localTransform )
    {
        _SetLocalTransformInternal( localTransform );
    }
}