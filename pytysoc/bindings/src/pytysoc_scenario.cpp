
#include <pytysoc_scenario.h>

namespace py = pybind11;

namespace pytysoc
{

    PyScenario::PyScenario()
    {
        m_scenarioPtr = new tysoc::TScenario();
    }

    PyScenario::~PyScenario()
    {
        m_pySingleBodies.clear();
        m_pySingleBodiesMap.clear();

        m_pyCompounds.clear();
        m_pyCompoundsMap.clear();

        m_pyCoreAgents.clear();
        m_pyCoreAgentsMap.clear();

        m_pyTerrainGens.clear();
        m_pyTerrainGensMap.clear();

        m_pySensors.clear();
        m_pySensorsMap.clear();

        if ( m_scenarioPtr )
        {
            delete m_scenarioPtr;
            m_scenarioPtr = nullptr;
        }
    }

    void PyScenario::addSingleBody( PySingleBody* pySingleBodyRef )
    {
        LOCO_CORE_ASSERT( m_scenarioPtr, "PyScenario::addSingleBody> there's no cpp-scenario yet" );
        LOCO_CORE_ASSERT( pySingleBodyRef->ptr(), "PyScenario::addSingleBody> single-body has no cpp-resource to add" );

        if ( m_pySingleBodiesMap.find( pySingleBodyRef->name() ) != m_pySingleBodiesMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::addSingleBody> tried to add an existing body with name: {0}", pySingleBodyRef->name() );
            return;
        }

        m_scenarioPtr->addSingleBody( dynamic_cast< tysoc::TSingleBody* >( pySingleBodyRef->ptr() ) );

        m_pySingleBodies.push_back( pySingleBodyRef );
        m_pySingleBodiesMap[ pySingleBodyRef->name() ] = pySingleBodyRef;
    }

    void PyScenario::addCompound( PyCompound* pyCompoundRef )
    {
        LOCO_CORE_ASSERT( m_scenarioPtr, "PyScenario::addCompound> there's no cpp-scenario yet" );
        LOCO_CORE_ASSERT( pyCompoundRef->ptr(), "PyScenario::addCompound> compound has no cpp-resource to add" );

        if ( m_pyCompoundsMap.find( pyCompoundRef->name() ) != m_pyCompoundsMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::addCompound> tried to add an existing body with name: {0}", pyCompoundRef->name() );
            return;
        }

        m_scenarioPtr->addCompound( dynamic_cast< tysoc::TCompound* >( pyCompoundRef->ptr() ) );

        m_pyCompounds.push_back( pyCompoundRef );
        m_pyCompoundsMap[pyCompoundRef->name()] = pyCompoundRef;
    }

    void PyScenario::addAgent( PyCoreAgent* pyCoreAgentPtr )
    {
        if ( !m_scenarioPtr )
            return;

        if ( pyCoreAgentPtr->name() == "undefined" )
        {
            LOCO_CORE_ERROR( "PyScenario::addAgent> tried to add a pyCoreAgent with no wrapped kintree agent" );
            return;
        }

        if ( m_pyCoreAgentsMap.find( pyCoreAgentPtr->name() ) != m_pyCoreAgentsMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::addAgent> tried to add an existing (same name) pyCoreAgent \"{0}\"", pyCoreAgentPtr->name() );
            return;
        }

        m_scenarioPtr->addAgent( pyCoreAgentPtr->ptr() );

        m_pyCoreAgents.push_back( pyCoreAgentPtr );
        m_pyCoreAgentsMap[pyCoreAgentPtr->name()] = pyCoreAgentPtr;
    }

    void PyScenario::addTerrainGen( PyTerrainGen* pyTerrainGenPtr )
    {
        if ( !m_scenarioPtr )
            return;

        if ( pyTerrainGenPtr->name() == "undefined" )
        {
            LOCO_CORE_ERROR( "PyScenario::addTerrainGen> tried to add a pyTerrainGen with no wrapped terrainGen" );
            return;
        }

        if ( m_pyTerrainGensMap.find( pyTerrainGenPtr->name() ) != m_pyTerrainGensMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::addTerrainGen> tried to add an existing (same name) pyTerrainGen called \"{0}\"", pyTerrainGenPtr->name() );
            return;
        }

        m_scenarioPtr->addTerrainGenerator( pyTerrainGenPtr->ptr() );

        m_pyTerrainGens.push_back( pyTerrainGenPtr );
        m_pyTerrainGensMap[pyTerrainGenPtr->name()] = pyTerrainGenPtr;
    }

    void PyScenario::addSensor( PySensor* pySensorPtr )
    {
        if ( !m_scenarioPtr )
            return;

        if ( pySensorPtr->name() == "undefined" )
        {
            LOCO_CORE_ERROR( "PyScenario::addSensor> tried to add a pySensor with no wrapped sensor" );
            return;
        }

        if ( m_pySensorsMap.find( pySensorPtr->name() ) != m_pySensorsMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::addSensor> tried to add an existing (same name) pySensor called \"{0}\"", pySensorPtr->name() );
            return;
        }

        m_scenarioPtr->addSensor( pySensorPtr->ptr() );

        m_pySensors.push_back( pySensorPtr );
        m_pySensorsMap[pySensorPtr->name()] = pySensorPtr;
    }

    PySingleBody* PyScenario::getSingleBodyByName( const std::string& name )
    {
        if ( m_pySingleBodiesMap.find( name ) == m_pySingleBodiesMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::getSingleBodyByName> single-body \"{0}\" not found in scenario", name );
            return nullptr;
        }

        return m_pySingleBodiesMap[name];
    }

    PyCompound* PyScenario::getCompoundByName( const std::string& name )
    {
        if ( m_pyCompoundsMap.find( name ) == m_pyCompoundsMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::getCompoundByName> compound \"{0}\" not found in scenario", name );
            return nullptr;
        }

        return m_pyCompoundsMap[name];
    }

    PyCoreAgent* PyScenario::getAgentByName( const std::string& name )
    {
        if ( m_pyCoreAgentsMap.find( name ) == m_pyCoreAgentsMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::getAgentByName> agent \"{0}\"  not found in scenario", name );
            return nullptr;
        }

        return m_pyCoreAgentsMap[name];
    }

    PyTerrainGen* PyScenario::getTerrainGenByName( const std::string& name )
    {
        if ( m_pyTerrainGensMap.find( name ) == m_pyTerrainGensMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::getTerrainGenByName> terrainGen \"{0}\" not found in scenario", name );
            return nullptr;
        }

        return m_pyTerrainGensMap[name];
    }

    PySensor* PyScenario::getSensorByName( const std::string& name )
    {
        if ( m_pySensorsMap.find( name ) == m_pySensorsMap.end() )
        {
            LOCO_CORE_WARN( "PyScenario::getSensorByName> sensor \"{0}\"  not found in scenario", name );
            return nullptr;
        }

        return m_pySensorsMap[name];
    }

    std::vector< PySingleBody* > PyScenario::getSingleBodies()
    {
        return m_pySingleBodies;
    }

    std::vector< PyCompound* > PyScenario::getCompounds()
    {
        return m_pyCompounds;
    }

    std::vector< PyCoreAgent* > PyScenario::getAgents()
    {
        return m_pyCoreAgents;
    }

    std::vector< PyTerrainGen* > PyScenario::getTerrainGens()
    {
        return m_pyTerrainGens;
    }

    std::vector< PySensor* > PyScenario::getSensors()
    {
        return m_pySensors;
    }

    std::map< std::string, PySingleBody* > PyScenario::getSingleBodiesMap()
    {
        return m_pySingleBodiesMap;
    }

    std::map< std::string, PyCompound* > PyScenario::getCompoundsMap()
    {
        return m_pyCompoundsMap;
    }

    std::map< std::string, PyCoreAgent* > PyScenario::getAgentsMap()
    {
        return m_pyCoreAgentsMap;
    }

    std::map< std::string, PyTerrainGen* > PyScenario::getTerrainGensMap()
    {
        return m_pyTerrainGensMap;
    }

    std::map< std::string, PySensor* > PyScenario::getSensorsMap()
    {
        return m_pySensorsMap;
    }

    tysoc::TScenario* PyScenario::ptr()
    {
        return m_scenarioPtr;
    }

}