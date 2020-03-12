#pragma once

#include <components/loco_body_base.h>

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

    protected:

        void _InitializeInternal() override;
        void _PreStepInternal() override;
        void _PostStepInternal() override;
        void _ResetInternal() override;
        void _DetachInternal() override;

        void _SetPositionInternal( const TVec3& position ) override;
        void _SetRotationInternal( const TMat3& rotation ) override;
        void _SetEulerInternal( const TVec3& euler ) override;
        void _SetQuaternionInternal( const TVec4& quat ) override;
        void _SetTransformInternal( const TMat4& transform ) override;

        void _SetInitialPositionInternal( const TVec3& position ) override;
        void _SetInitialRotationInternal( const TMat3& rotation ) override;
        void _SetInitialEulerInternal( const TVec3& euler ) override;
        void _SetInitialQuaternionInternal( const TVec4& quat ) override;
        void _SetInitialTransformInternal( const TMat4& transform ) override;

        void _SetLocalPositionInternal( const TVec3& localPosition ) override;
        void _SetLocalRotationInternal( const TMat3& localRotation ) override;
        void _SetLocalEulerInternal( const TVec3& localEuler ) override;
        void _SetLocalQuaternionInternal( const TVec4& localQuat ) override;
        void _SetLocalTransformInternal( const TMat4& localTransform ) override;

        void _SetInitialLocalPositionInternal( const TVec3& localPosition ) override;
        void _SetInitialLocalRotationInternal( const TMat3& localRotation ) override;
        void _SetInitialLocalEulerInternal( const TVec3& localEuler ) override;
        void _SetInitialLocalQuaternionInternal( const TVec4& localQuat ) override;
        void _SetInitialLocalTransformInternal( const TMat4& localTransform ) override;
    };
}
