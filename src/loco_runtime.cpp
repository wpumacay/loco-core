
#include <loco_runtime.h>

namespace loco
{
    TRuntime::TRuntime( const std::string& dlpathSim,
                        const std::string& dlpathViz )
    {
        m_fcnCreateSimulation = nullptr;
        m_fcnCreateVisualizer = nullptr;

        m_libHandleSimulation = nullptr;
        m_libHandleVisualizer = nullptr;

        m_visualizer = nullptr;
        m_simulation = nullptr;

        _LoadLibraryFunctions( dlpathSim, dlpathViz );

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TRuntime @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TRuntime @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TRuntime::~TRuntime()
    {
        m_fcnCreateSimulation = nullptr;
        m_fcnCreateVisualizer = nullptr;

        m_simulation = nullptr;
        m_visualizer = nullptr;

        if ( m_libHandleSimulation )
            dlclose( m_libHandleSimulation );
        m_libHandleSimulation = nullptr;

        if ( m_libHandleVisualizer )
            dlclose( m_libHandleVisualizer );
        m_libHandleSimulation = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TRuntime @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TRuntime @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TRuntime::_LoadLibraryFunctions( const std::string& dlpathSim,
                                          const std::string& dlpathViz )
    {
        m_dlpathSimulation = dlpathSim;
        m_dlpathVisualizer = dlpathViz;

        if ( m_dlpathSimulation != "" && m_dlpathSimulation != config::physics::NONE )
        {
            m_libHandleSimulation = dlopen( m_dlpathSimulation.c_str(), RTLD_NOW );
            if ( m_libHandleSimulation )
            {
                m_fcnCreateSimulation = (FcnCreateSim*)_LoadFcnFromSimulationLibrary( "simulation_create" );
            }
            else
            {
                LOCO_CORE_ERROR( "TRuntime::_LoadLibraryFcns >>> error while loading simulation dllibrary \"{0}\". \
                                 Error message: {1}", m_dlpathSimulation, dlerror() );
            }
        }

        if ( m_dlpathVisualizer != "" && m_dlpathVisualizer != config::rendering::NONE )
        {
            m_libHandleVisualizer = dlopen( m_dlpathVisualizer.c_str(), RTLD_NOW );
            if ( m_libHandleVisualizer )
            {
                m_fcnCreateVisualizer = (FcnCreateViz*)_LoadFcnFromVisualizerLibrary( "visualizer_create" );
            }
            else
            {
                LOCO_CORE_ERROR( "TRuntime::_LoadLibraryFcns >>> error while loading visualizer dllibrary \"{0}\". \
                                 Error message: {1}", m_dlpathVisualizer, dlerror() );
            }
        }
    }

    void* TRuntime::_LoadFcnFromSimulationLibrary( const std::string& fcnName )
    {
        LOCO_CORE_ASSERT( m_libHandleSimulation, "TRuntime::_LoadFcnFromSimulationLibrary >>> sim-library \
                          hasn't been loaded yet. (Tried to load symbol {0})", fcnName );

        void* _fcnPtr = dlsym( m_libHandleSimulation, fcnName.c_str() );
        if ( !_fcnPtr )
        {
            LOCO_CORE_ERROR( "TRuntime::_LoadFcnFromSimulationLibrary >>> Couldn't load simulation-symbol: {0}. \
                             Error message: {1}", fcnName, dlerror() );
            return nullptr;
        }

        LOCO_CORE_INFO( "TRuntime::_LoadFcnFromSimulationLibrary >>> Successfully loaded simulation-symbol: {0}", fcnName );
        return _fcnPtr;
    }

    void* TRuntime::_LoadFcnFromVisualizerLibrary( const std::string& fcnName )
    {
        LOCO_CORE_ASSERT( m_libHandleVisualizer, "TRuntime::_LoadFcnFromVisualizationLibrary >>> viz-library \
                          hasn't been loaded yet. (Tried to load symbol {0})", fcnName );

        void* _fcnPtr = dlsym( m_libHandleVisualizer, fcnName.c_str() );
        if ( !_fcnPtr )
        {
            LOCO_CORE_ERROR( "TRuntime::_LoadFcnFromVisualizationLibrary >>> Couldn't load visualization-symbol: {0}. \
                             Error message: {1}", fcnName, dlerror() );
            return nullptr;
        }

        LOCO_CORE_INFO( "TRuntime::_LoadFcnFromVisualizationLibrary >>> Successfully loaded visualizer-symbol: {0}", fcnName );
        return _fcnPtr;
    }

    TISimulation* TRuntime::CreateSimulation( TScenario* scenarioRef )
    {
        if ( !m_fcnCreateSimulation )
        {
            if ( m_dlpathSimulation != config::physics::NONE )
                LOCO_CORE_ERROR( "TRuntime::CreateSimulation >>> tried creating a simulation without proper \
                                 backend-fcnPointer. Creating dummy simulation instead." );

            m_simulation = std::make_unique<TNullSimulation>( scenarioRef );
        }
        else
        {
            if ( m_simulation )
            {
                LOCO_CORE_WARN( "TRuntime::CreateSimulation >>> there's already a simulation. Deleting and creating a new one" );
                m_simulation = nullptr;
            }

            m_simulation = std::unique_ptr<TISimulation>( m_fcnCreateSimulation( scenarioRef ) );
        }

        return m_simulation.get();
    }

    TIVisualizer* TRuntime::CreateVisualizer( TScenario* scenarioRef,
                                              size_t windowWidth, size_t windowHeight,
                                              bool windowResizable, bool renderOffscreen )
    {
        if ( !m_fcnCreateVisualizer )
        {
            if ( m_dlpathVisualizer != config::physics::NONE )
                LOCO_CORE_ERROR( "TRuntime::CreateVisualizer >>> tried creating a visualizer without proper \
                                 backend-fcnPointer. Creating dummy visualizer instead." );

            m_visualizer = std::make_unique<TNullVisualizer>( scenarioRef,
                                                              windowWidth, windowHeight,
                                                              windowResizable, renderOffscreen );
        }
        else
        {
            if ( m_visualizer )
            {
                LOCO_CORE_WARN( "TRuntime::CreateVisualizer >>> there's already a visualizer. Deleting and creating a new one." );
                m_visualizer = nullptr;
            }

            m_visualizer = std::unique_ptr<TIVisualizer>( m_fcnCreateVisualizer( scenarioRef,
                                                                                 windowWidth, windowHeight,
                                                                                 windowResizable, renderOffscreen ) );
        }

        return m_visualizer.get();
    }

    void TRuntime::DestroySimulation()
    {
        m_simulation = nullptr;
    }

    void TRuntime::DestroyVisualizer()
    {
        m_visualizer = nullptr;
    }

}