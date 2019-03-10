
#include <pytysoc_bindings.h>

namespace py = pybind11;

namespace pytysoc
{
    void initializeMjcfSchemaPath( const std::string& schemaPath )
    {
        tysoc::mjcf::MJCF_SCHEMA_PATH = schemaPath;
        std::cout << "DEBUG> set schema path to: " << schemaPath << std::endl;
    }

    void initializeLoader( const std::string& pathTemplatesMjcf,
                           const std::string& pathTemplatesUrdf,
                           const std::string& pathTemplatesRlsim )
    {
        tysoc::TModelLoader::Create( pathTemplatesMjcf,
                                     pathTemplatesUrdf,
                                     pathTemplatesRlsim );
    }
}

PYBIND11_MODULE( tysoc_bindings, m )
{
    // Some bindings for initialization
    m.def( "initializeMjcfSchemaPath", &pytysoc::initializeMjcfSchemaPath );
    m.def( "initializeLoader", &pytysoc::initializeLoader );

    // Common bindings
    PYTYSOC_COMMON_BINDINGS( m )
    // Tests bindings
    PYTYSOC_TESTS_BINDINGS( m )
    // Core agent bindings
    PYTYSOC_CORE_AGENT_BINDINGS( m )
    // Core terrainGen bindings
    PYTYSOC_CORE_TERRAINGEN_BINDINGS( m )
    // Sensor bindings
    PYTYSOC_SENSOR_BINDINGS( m )
    // Scenario bindings
    PYTYSOC_SCENARIO_BINDINGS( m )
    // visualizer bindings
    PYTYSOC_VISUALIZER_BINDINGS( m )
    // simulation bindings
    PYTYSOC_SIMULATION_BINDINGS( m )
    // runtime bindings
    PYTYSOC_RUNTIME_BINDINGS( m )

    m.attr( "__version__" ) = "dev";
}