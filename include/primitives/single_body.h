#pragma once

#include <components/body.h>

namespace tysoc {

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

        void setLocalPosition( const TVec3& localPosition ) override;

        void setLocalRotation( const TMat3& localRotation ) override;

        void setLocalEuler( const TVec3& localEuler ) override;

        void setLocalQuaternion( const TVec4& localQuat ) override;

        void setLocalTransform( const TMat4& localTransform ) override;

    };

}
