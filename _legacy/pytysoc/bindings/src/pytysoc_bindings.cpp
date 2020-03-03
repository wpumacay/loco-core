
#include <pytysoc_bindings.h>

namespace py = pybind11;

// @todo: REMOVE usage of double wrapping, as we're currently creating wrappers for c++ resources,
//        but pybind11 already wraps the classes we make as wrappers (double wrapping). Instead, 
//        just wrap directly, as in the following example:
//
//  current:
//          /* define some wrapping class */
//          class PyBody{}
//          /* define wrappers using pybind for this """wrapping""" class */
//          py::class_<pytysoc::PyBody>(m,"PyBody") \...
//
//  desired:
//          /* just wrap directly the tysoc::TIBody object */
//          py::class_<tysoc::TIBody>(m,"TIBody") \...
//
// for this to work, we need to expose every element to pybind, and this means EVERY elements on
// the c++ side: enums (e.g. eJointType), structs (e.g. TJointData), math-objects (e.g. TMat4), 
// core tysoc objects (e.g. TIBody, TCompound), etc.

namespace pytysoc
{
    void initializeMjcfSchemaPath( const std::string& schemaPath )
    {
        tysoc::mjcf::MJCF_SCHEMA_PATH = schemaPath;
        // std::cout << "DEBUG> set schema path to: " << schemaPath << std::endl;
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
    // Enums and Data bindings
    PYTYSOC_ENUM_BINDINGS( m )
    PYTYSOC_DATA_BINDINGS( m )
    // Core collision bindings
    PYTYSOC_COLLISION_BINDINGS( m )
    // Core visual bindings
    PYTYSOC_VISUAL_BINDINGS( m )
    // Core joint bindings
    PYTYSOC_JOINT_BINDINGS( m )
    // Core body bindings
    PYTYSOC_INTERFACE_BODY_BINDINGS( m )
    PYTYSOC_SINGLE_BODY_BINDINGS( m )
    PYTYSOC_COMPOUND_BODY_BINDINGS( m )
    // Core compound bindings
    PYTYSOC_COMPOUND_BINDINGS( m )
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
    PYTYSOC_VISUALIZER_KEYCODES( m )
    // simulation bindings
    PYTYSOC_SIMULATION_BINDINGS( m )
    // runtime bindings
    PYTYSOC_RUNTIME_BINDINGS( m )

    m.attr( "__version__" ) = "dev";
}