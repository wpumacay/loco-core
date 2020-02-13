#pragma once

#include <loco_common.h>

namespace loco
{
    enum class eVizLightType : uint8_t
    {
        DIRECTIONAL = 0,
        POINT,
        SPOT
    };

    std::string ToString( const eVizLightType& type );

    class TVizLightAdapter;

    class TVizLight
    {
    public :

        TVizLight( const std::string& name,
                   const eVizLightType& type,
                   const TVec3& ambientColor,
                   const TVec3& diffuseColor,
                   const TVec3& specularColor );
        ~TVizLight();

        void Update();
        void Reset();

        void SetAdapter( TVizLightAdapter* adapterRef );

        void SetPosition( const TVec3& position );
        void SetDirection( const TVec3& direction );

        void ChangeAmbientColor( const TVec3& ambientColor );
        void ChangeDiffuseColor( const TVec3& diffuseColor );
        void ChangeSpecularColor( const TVec3& specularColor );
        void ChangeIntensity( float intensity );
        void SetCastShadows( bool castShadows );
        void ChangeInnerCutoff( float innerCutoff );
        void ChangeOuterCutoff( float outerCutoff );

        TVec3 position() const { return m_position; }
        TVec3 direction() const { return m_direction; }

        TVec3 ambient() const { return m_ambientColor; }
        TVec3 diffuse() const { return m_diffuseColor; }
        TVec3 specular() const { return m_specularColor; }
        float intensity() const { return m_intensity; }
        bool castShadows() const { return m_castShadows; }
        float innerCutoff() const { return m_innerCutoff; }
        float outerCutoff() const { return m_outerCutoff; }

        std::string name() const { return m_name; }
        eVizLightType type() const { return m_type; }

    private :

        // Unique identifier of this light on the visualizer
        std::string m_name;
        // Type of light used (check enum above for supported types)
        eVizLightType m_type;
        // Position of the light on the scenario (if applicable: point|spot)
        TVec3 m_position;
        // Initial position of the light on the scenario (if applicable: point|spot)
        TVec3 m_position0;
        // Direction of the light on the scenario (if applicable: directional|spot)
        TVec3 m_direction;
        // Initial direction of the light on the scenario (if applicable: directional|spot)
        TVec3 m_direction0;
        // Color-components of the light
        TVec3 m_ambientColor;
        TVec3 m_diffuseColor;
        TVec3 m_specularColor;
        float m_intensity;
        // Whether or not the light casts shadows
        bool m_castShadows;
        // Cutoff angles (if applicable: spot)
        float m_innerCutoff;
        float m_outerCutoff;
        // Adapter for backend-specific light functionality
        TVizLightAdapter* m_adapterRef;
    };

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

    protected :

        // Handle to the light-object the user is exposed to
        TVizLight* m_lightRef;
    };
}