#pragma once

#include <components/body.h>

namespace loco
{
    class TCollision;
    class TVisual;

    class TSingleBody : public TIBody
    {

    public :
        
        TSingleBody( const std::string& name,
                     const TBodyData& data,
                     const TVec3& position,
                     const TMat3& rotation );

        ~TSingleBody();

        void _preStepInternal() override;
        void _postStepInternal() override;
        void _resetInternal() override;

        void _setPositionInternal( const TVec3& position ) override;
        void _setRotationInternal( const TMat3& rotation ) override;
        void _setEulerInternal( const TVec3& euler ) override;
        void _setQuaternionInternal( const TVec4& quat ) override;
        void _setTransformInternal( const TMat4& transform ) override;

        void _setLocalPositionInternal( const TVec3& localPosition ) override;
        void _setLocalRotationInternal( const TMat3& localRotation ) override;
        void _setLocalEulerInternal( const TVec3& localEuler ) override;
        void _setLocalQuaternionInternal( const TVec4& localQuat ) override;
        void _setLocalTransformInternal( const TMat4& localTransform ) override;

    };
}
