
#include <pytysoc_runtime.h>

namespace py = pybind11;

namespace pytysoc
{

    PyRuntime::PyRuntime( const std::string& dlpathSim,
                          const std::string& dlpathViz )
    {
        m_runtimePtr = new tysoc::TRuntime( dlpathSim, dlpathViz );
    }

    PyRuntime::~PyRuntime()
    {
        if ( m_runtimePtr )
        {
            delete m_runtimePtr;
            m_runtimePtr = nullptr;
        }
    }

    PySimulation* PyRuntime::createSimulation( PyScenario* pyScenarioPtr )
    {
        auto _scenarioPtr = pyScenarioPtr->ptr();
        auto _simulationPtr = m_runtimePtr->createSimulation( _scenarioPtr );
        if ( _simulationPtr )
            return new PySimulation( _simulationPtr );
        else
            return nullptr;
    }

    PyVisualizer* PyRuntime::createVisualizer( PyScenario* pyScenarioPtr )
    {
        auto _scenarioPtr = pyScenarioPtr->ptr();
        auto _visualizerPtr = m_runtimePtr->createVisualizer( _scenarioPtr );
        if ( _visualizerPtr )
            return new PyVisualizer( _visualizerPtr );
        else
            return nullptr;
    }

}