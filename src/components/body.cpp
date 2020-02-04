
#include <components/body.h>

namespace loco
{
    TIBody::TIBody( const std::string& name,
                    const TBodyData& data )
    {
        m_name = name;
        m_data = data;
        m_bodyImplRef = nullptr;
        m_classType = eBodyClassType::NONE;

        m_collision = std::unique_ptr< TCollision >( new TCollision( m_name + "_col", m_data.collision ) );
        m_collision->setParentBody( this );

        m_visual = std::unique_ptr< TVisual >( new TVisual( m_name + "_viz", m_data.visual ) );
        m_visual->setParentBody( this );
    }

    TIBody::~TIBody()
    {
        m_bodyImplRef = nullptr;
        m_collision = nullptr;
        m_visual = nullptr;
    }

    void TIBody::setAdapter( TIBodyAdapter* bodyImplRef )
    {
        m_bodyImplRef = bodyImplRef;
    }

    void TIBody::setCollision( std::unique_ptr< TCollision > collisionObj )
    {
        if ( !collisionObj )
            return;

        // Keep ownership of this collider, update collision-body references, and
        // update the body data (note: previous collider is deleted)
        m_collision = std::move( collisionObj );
        m_collision->setParentBody( this );
        m_data.collision = m_collision->data();
    }

    void TIBody::setVisual( std::unique_ptr< TVisual > visualObj )
    {
        if ( !visualObj )
            return;

        // Keep ownership of this visual, update collision-body references, and
        // update the body data (note: previous visual is deleted)
        m_visual = std::move( visualObj );
        m_visual->setParentBody( this );
        m_data.visual = m_visual->data();
    }

    void TIBody::preStep()
    {
        _preStepInternal();
    }

    void TIBody::postStep()
    {
        _postStepInternal();
    }

    void TIBody::reset()
    {
        _resetInternal();
    }

    ////////////////////////////////////////////////////////////
    //                   World-space setters                  //
    ////////////////////////////////////////////////////////////

    void TIBody::setPosition( const TVec3& position )
    {
        _setPositionInternal( position );
    }

    void TIBody::setRotation( const TMat3& rotation )
    {
        _setRotationInternal( rotation );
    }

    void TIBody::setEuler( const TVec3& euler )
    {
        _setEulerInternal( euler );
    }

    void TIBody::setQuaternion( const TVec4& quat )
    {
        _setQuaternionInternal( quat );
    }

    void TIBody::setTransform( const TMat4& transform )
    {
        _setTransformInternal( transform );
    }

    ////////////////////////////////////////////////////////////
    //                   Local-space setters                  //
    ////////////////////////////////////////////////////////////

    void TIBody::setLocalPosition( const TVec3& localPosition )
    {
        _setLocalPositionInternal( localPosition );
    }

    void TIBody::setLocalRotation( const TMat3& localRotation )
    {
        _setLocalRotationInternal( localRotation );
    }

    void TIBody::setLocalEuler( const TVec3& localEuler )
    {
        _setLocalEulerInternal( localEuler );
    }

    void TIBody::setLocalQuaternion( const TVec4& localQuat )
    {
        _setLocalQuaternionInternal( localQuat );
    }

    void TIBody::setLocalTransform( const TMat4& localTransform )
    {
        _setLocalTransformInternal( localTransform );
    }
}