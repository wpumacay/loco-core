#pragma once

#include <loco_common.h>
#include <visualizer/loco_visualizer_light.h>

namespace loco {
namespace visualizer {

    class TVizLight;

    class TVizLightAdapter
    {
    public :

        TVizLightAdapter( TVizLight* lightRef ) : m_lightRef( lightRef ) {}
        virtual ~TVizLightAdapter() {}

        virtual void Build() = 0;
        virtual void Detach() = 0;
        virtual void Reset() = 0;

        virtual void ChangeAmbientColor( const TVec3& ambientColor ) = 0;
        virtual void ChangeDiffuseColor( const TVec3& diffuseColor ) = 0;
        virtual void ChangeSpecularColor( const TVec3& specularColor ) = 0;
        virtual void ChangeIntensity( float intensity ) = 0;

        virtual void SetPosition( const TVec3& position ) = 0;
        virtual void SetDirection( const TVec3& direction ) = 0;
        virtual void SetCastShadows( bool castShadows ) = 0;

        virtual void ChangeAttenuationK0( float attnk0 ) = 0;
        virtual void ChangeAttenuationK1( float attnk1 ) = 0;
        virtual void ChangeAttenuationK2( float attnk2 ) = 0;
        virtual void ChangeInnerCutoff( float innerCutoff ) = 0;
        virtual void ChangeOuterCutoff( float outerCutoff ) = 0;

        bool IsAwaitingDeletion() const { return m_awaitingDeletion; }

    protected :

        // Handle to the light-object the user is exposed to
        TVizLight* m_lightRef;
        // Whether or not the adapter is ready to be freed
        bool m_awaitingDeletion;
    };
}}