
#include <visualizer/loco_visualizer_light.h>

namespace loco
{
    std::string ToString( const eVizLightType& type )
    {
        if ( type == eVizLightType::DIRECTIONAL ) return "directional";
        if ( type == eVizLightType::POINT ) return "point";
        if ( type == eVizLightType::SPOT ) return "spot";

        return "undefined";
    }

    TVizLight::TVizLight( const std::string& name,
                          const eVizLightType& type,
                          const TVec3& ambientColor,
                          const TVec3& diffuseColor,
                          const TVec3& specularColor )
    {
        m_name = name;
        m_type = type;
        m_ambientColor = ambientColor;
        m_diffuseColor = diffuseColor;
        m_specularColor = specularColor;
        m_castShadows = true;
        m_intensity = 1.0f;
        m_innerCutoff = loco::PI / 4.0f;
        m_outerCutoff = loco::PI / 3.0f;
        m_position0 = { 0.0f, 0.0f, 1.0f };
        m_position = m_position0;
        m_direction0 = { 0.0f, 0.0f, -1.0f };
        m_direction = m_direction0;
        m_adapterRef = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TVizLight {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TVizLight " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TVizLight::~TVizLight()
    {
        if ( m_adapterRef )
            m_adapterRef->Detach();

        m_adapterRef = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TVizLight {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TVizLight " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TVizLight::Update()
    {
        // nothing to update (unlike cameras, lights shouldn't change internally on their own)
    }

    void TVizLight::Reset()
    {
        m_position = m_position0;
        m_direction = m_direction0;

        if ( m_adapterRef )
            m_adapterRef->Reset();
    }

    void TVizLight::SetAdapter( TVizLightAdapter* adapterRef )
    {
        m_adapterRef = adapterRef;
    }

    void TVizLight::SetPosition( const TVec3& position )
    {
        m_position = position;

        if ( m_adapterRef )
            m_adapterRef->SetPosition( position );
    }

    void TVizLight::SetDirection( const TVec3& direction )
    {
        m_direction = direction;

        if ( m_adapterRef )
            m_adapterRef->SetDirection( direction );
    }

    void TVizLight::ChangeAmbientColor( const TVec3& ambientColor )
    {
        m_ambientColor = ambientColor;

        if ( m_adapterRef )
            m_adapterRef->ChangeAmbientColor( ambientColor );
    }

    void TVizLight::ChangeDiffuseColor( const TVec3& diffuseColor )
    {
        m_diffuseColor = diffuseColor;

        if ( m_adapterRef )
            m_adapterRef->ChangeDiffuseColor( diffuseColor );
    }

    void TVizLight::ChangeSpecularColor( const TVec3& specularColor )
    {
        m_specularColor = specularColor;

        if ( m_adapterRef )
            m_adapterRef->ChangeSpecularColor( specularColor );
    }

    void TVizLight::ChangeIntensity( float intensity )
    {
        m_intensity = intensity;

        if ( m_adapterRef )
            m_adapterRef->ChangeIntensity( intensity );
    }

    void TVizLight::SetCastShadows( bool castShadows )
    {
        m_castShadows = castShadows;

        if ( m_adapterRef )
            m_adapterRef->SetCastShadows( castShadows );
    }

    void TVizLight::ChangeInnerCutoff( float innerCutoff )
    {
        m_innerCutoff = innerCutoff;

        if ( m_adapterRef )
            m_adapterRef->ChangeInnerCutoff( innerCutoff );
    }

    void TVizLight::ChangeOuterCutoff( float outerCutoff )
    {
        m_outerCutoff = outerCutoff;

        if ( m_adapterRef )
            m_adapterRef->ChangeOuterCutoff( outerCutoff );
    }
}