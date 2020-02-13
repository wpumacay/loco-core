
#include <visualizer/loco_visualizer.h>

namespace loco
{
    TIVisualizer::TIVisualizer( TScenario* scenarioRef )
    {
        m_scenarioRef = scenarioRef;
        m_currentCameraIndex = -1;
        m_currentLightIndex = -1;
    }

    TIVisualizer::~TIVisualizer()
    {
        m_vizCameras.clear();
        m_vizLights.clear();
        m_vizCameraAdapters.clear();
        m_vizLightAdapters.clear();
        m_vizDrawableAdapters.clear();
        m_scenarioRef   = nullptr;
    }

    void TIVisualizer::ChangeScenario( TScenario* scenarioRef )
    {
        m_scenarioRef = scenarioRef;
        _ChangeScenarioInternal();
    }

    void TIVisualizer::Initialize()
    {
        for ( auto& cameraAdapter : m_vizCameraAdapters )
            cameraAdapter->Build();

        for ( auto& lightAdapter : m_vizLightAdapters )
            lightAdapter->Build();

        _InitializeInternal();
    }

    void TIVisualizer::Update()
    {
        for ( auto& camera : m_vizCameras )
            camera->Update();

        for ( auto& light : m_vizLights )
            light->Update();

        _UpdateInternal();
    }

    void TIVisualizer::Reset()
    {
        for ( auto& camera : m_vizCameras )
            camera->Reset();

        for ( auto& light : m_vizLights )
            light->Reset();

        _ResetInternal();
    }

    TVizCamera* TIVisualizer::CreateCamera( const std::string& name,
                                            const eVizCameraType& type,
                                            const TVec3& position,
                                            const TVec3& target )
    {
        auto camera = std::make_unique<TVizCamera>( name, type, position, target );
        // Construct backend-specific adapters here
        _CreateCameraInternal( camera.get() );

        m_camerasMap[camera->name()] = m_vizCameras.size();
        m_vizCameras.push_back( std::move( camera ) );
        if ( m_currentCameraIndex < 0 )
            m_currentCameraIndex = m_vizCameras.size() - 1;

        return m_vizCameras.back().get();
    }

    TVizLight* TIVisualizer::CreateLight( const std::string& name,
                                          const eVizLightType& type,
                                          const TVec3& ambient,
                                          const TVec3& diffuse,
                                          const TVec3& specular )
    {
        auto light = std::make_unique<TVizLight>( name, type, ambient, diffuse, specular );
        // Construct backend-specific adapters here
        _CreateLightInternal( light.get() );

        m_lightsMap[light->name()] = m_vizLights.size();
        m_vizLights.push_back( std::move( light ) );
        if ( m_currentLightIndex < 0 )
            m_currentLightIndex = m_vizLights.size() - 1;

        return m_vizLights.back().get();
    }


    TVizCamera* TIVisualizer::GetCurrentCamera()
    {
        if ( m_currentCameraIndex < 0 )
        {
            LOCO_CORE_ERROR( "TIVisualizer::GetCurrentCamera >>> there's no current camera in the visualizer" );
            return nullptr;
        }

        return get_mutable_camera( m_currentCameraIndex );
    }

    TVizLight* TIVisualizer::GetCurrentLight()
    {
        if ( m_currentLightIndex < 0 )
        {
            LOCO_CORE_ERROR( "TIVisualizer::GetCurrentLight >>> there's no current light in the visualizer" );
            return nullptr;
        }

        return get_mutable_light( m_currentLightIndex );
    }


    const TVizCamera* TIVisualizer::GetCurrentCamera() const
    {
        if ( m_currentCameraIndex < 0 )
        {
            LOCO_CORE_ERROR( "TIVisualizer::GetCurrentCamera >>> there's no current camera in the visualizer" );
            return nullptr;
        }

        return get_camera( m_currentCameraIndex );
    }

    const TVizLight* TIVisualizer::GetCurrentLight() const
    {
        if ( m_currentLightIndex < 0 )
        {
            LOCO_CORE_ERROR( "TIVisualizer::GetCurrentLight >>> there's no current light in the visualizer" );
            return nullptr;
        }

        return get_light( m_currentLightIndex );
    }

    void TIVisualizer::ChangeCurrentCamera( const std::string& name )
    {
        if ( !HasCameraNamed( name ) )
        {
            LOCO_CORE_ERROR( "TIVisualizer::ChangeCurrentCamera >>> camera named '{0}' not found", name );
            return;
        }

        _ChangeCurrentCameraInternal( get_mutable_camera( m_camerasMap.at( name ) ) );
    }

    void TIVisualizer::ChangeCurrentLight( const std::string& name )
    {
        if ( !HasLightNamed( name ) )
        {
            LOCO_CORE_ERROR( "TIVisualizer::ChangeCurrentLight >>> light named '{0}' not found", name );
            return;
        }

        _ChangeCurrentLightInternal( get_mutable_light( m_lightsMap.at( name ) ) );
    }

    bool TIVisualizer::HasCameraNamed( const std::string& name ) const
    {
        return m_camerasMap.find( name ) != m_camerasMap.end();
    }

    bool TIVisualizer::HasLightNamed( const std::string& name ) const
    {
        return m_lightsMap.find( name ) != m_lightsMap.end();
    }

    size_t TIVisualizer::GetNumCameras() const
    {
        return m_vizCameras.size();
    }

    size_t TIVisualizer::GetNumLights() const
    {
        return m_vizLights.size();
    }

    const TVizCamera* TIVisualizer::GetCameraByName( const std::string& name ) const
    {
        if ( !HasCameraNamed( name ) )
        {
            LOCO_CORE_ERROR( "TIVisualizer::GetCameraByName >>> camera named '{0}' not found", name );
            return nullptr;
        }

        return get_camera( m_camerasMap.at( name ) );
    }

    const TVizLight* TIVisualizer::GetLightByName( const std::string& name ) const
    {
        if ( !HasLightNamed( name ) )
        {
            LOCO_CORE_ERROR( "TIVisualizer::GetLightByName >>> light names '{0}' not found", name );
            return nullptr;
        }

        return get_light( m_lightsMap.at( name ) );
    }

    TVizCamera* TIVisualizer::GetCameraByName( const std::string& name )
    {
        if ( !HasCameraNamed( name ) )
        {
            LOCO_CORE_ERROR( "TIVisualizer::GetCameraByName >>> camera named '{0}' not found", name );
            return nullptr;
        }

        return get_mutable_camera( m_camerasMap.at( name ) );
    }

    TVizLight* TIVisualizer::GetLightByName( const std::string& name )
    {
        if ( !HasLightNamed( name ) )
        {
            LOCO_CORE_ERROR( "TIVisualizer::GetLightByName >>> light names '{0}' not found", name );
            return nullptr;
        }

        return get_mutable_light( m_lightsMap.at( name ) );
    }

    bool TIVisualizer::IsActive() const
    {
        return _IsActiveInternal();
    }

    bool TIVisualizer::IsKeyDown( int key ) const
    {
        return _IsKeyDownInternal( key );
    }

    bool TIVisualizer::CheckSingleKeyPress( int key ) const
    {
        return _CheckSingleKeyPressInternal( key );
    }

    bool TIVisualizer::IsMouseDown( int button ) const
    {
        return _IsMouseDownInternal( button );
    }

    TVec2 TIVisualizer::GetCursorPosition() const
    {
        return _GetCursorPositionInternal();
    }

    const TVizCamera* TIVisualizer::get_camera( ssize_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_vizCameras.size() ), "TIVisualizer::get_camera >>> index {0} \
                          out of range [0,...{1}]", index, m_vizCameras.size() );

        return m_vizCameras[index].get();
    }

    const TVizLight* TIVisualizer::get_light( ssize_t index ) const
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_vizLights.size() ), "TIVisualizer::get_light >>> index {0} \
                          out of range [0,...{1}]", index, m_vizLights.size() );

        return m_vizLights[index].get();
    }

    TVizCamera* TIVisualizer::get_mutable_camera( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_vizCameras.size() ), "TIVisualizer::get_mutable_camera >>> index {0} \
                          out of range [0,...{1}]", index, m_vizCameras.size() );

        return m_vizCameras[index].get();
    }

    TVizLight* TIVisualizer::get_mutable_light( ssize_t index )
    {
        LOCO_CORE_ASSERT( ( index >= 0 && index < m_vizLights.size() ), "TIVisualizer::get_mutable_light >>> index {0} \
                          out of range [0,...{1}]", index, m_vizLights.size() );

        return m_vizLights[index].get();
    }

    void TIVisualizer::DrawLine( const TVec3& start, const TVec3& end, const TVec3& color )
    {
        _DrawLineInternal( start, end, color );
    }

    void TIVisualizer::DrawAABB( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color )
    {
        _DrawAABBInternal( aabbMin, aabbMax, aabbWorldTransform, color );
    }

    void TIVisualizer::DrawSolidCylinderX( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        _DrawSolidCylinderInternalX( radius, height, transform, color );
    }

    void TIVisualizer::DrawSolidCylinderY( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        _DrawSolidCylinderInternalY( radius, height, transform, color );
    }

    void TIVisualizer::DrawSolidCylinderZ( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        _DrawSolidCylinderInternalZ( radius, height, transform, color );
    }

    void TIVisualizer::DrawSolidArrowX( float length, const TMat4& transform, const TVec4& color)
    {
        _DrawSolidArrowInternalX( length, transform, color );
    }

    void TIVisualizer::DrawSolidArrowY( float length, const TMat4& transform, const TVec4& color)
    {
        _DrawSolidArrowInternalY( length, transform, color );
    }

    void TIVisualizer::DrawSolidArrowZ( float length, const TMat4& transform, const TVec4& color)
    {
        _DrawSolidArrowInternalZ( length, transform, color );
    }

    /********************************************************************************
    *                               Null-visualizer impl                            *
    ********************************************************************************/

    TNullVisualizer::TNullVisualizer( TScenario* scenarioRef )
        : TIVisualizer( scenarioRef )
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TNullVisualizer @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TNullVisualizer @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TNullVisualizer::~TNullVisualizer()
    {
    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TNullVisualizer @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TNullVisualizer @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }
}