
#include <pytysoc_simulation.h>

namespace py = pybind11;

namespace pytysoc
{

    PySimulation::PySimulation( tysoc::TISimulation* simulationPtr )
    {
        m_simulationPtr = simulationPtr;
    }

    PySimulation::~PySimulation()
    {
        m_simulationPtr = nullptr;
    }

    bool PySimulation::initialize()
    {
        if ( m_simulationPtr )
            return m_simulationPtr->initialize();

        TYSOC_CORE_ERROR( "PySimulation::initialize() >>> wrapped simulation-obj is null" );
        return false;
    }

    void PySimulation::step()
    {
        if ( !m_simulationPtr )
        {
            TYSOC_CORE_ERROR( "PySimulation::step() >>> wrapped simulation-obj is null" );
            return;
        }

        m_simulationPtr->step();
    }

    void PySimulation::reset()
    {
        if ( !m_simulationPtr )
        {
            TYSOC_CORE_ERROR( "PySimulation::initialize() >>> wrapped simulation-obj is null" );
            return;
        }

        m_simulationPtr->reset();
    }

    MeasurementDict PySimulation::getDictOfVectorizedSimData()
    {
        MeasurementDict _res;


        return _res;
    }

//    PyScenario* PySimulation::scenario()
//    {
//        if ( m_simulationPtr )
//        {
//            auto _scenarioPtr = m_simulationPtr->scenario();
//            if ( _scenarioPtr )
//                return new PyScenario( _scenarioPtr );
//        }
//
//        return nullptr;
//    }

    std::string PySimulation::type()
    {
        if ( m_simulationPtr )
            return m_simulationPtr->type();

        TYSOC_CORE_ERROR( "PySimulation::type() >>> wrapped simulation-obj is null" );
        return "undefined";
    }

}