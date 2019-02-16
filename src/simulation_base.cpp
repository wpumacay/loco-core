
#include <simulation_base.h>


namespace tysoc {


    TISimulation::TISimulation( TScenario* scenarioPtr )
    {
        m_runtimeType = "none";
        m_scenarioPtr = scenarioPtr;
    }

    TISimulation::~TISimulation()
    {
        m_scenarioPtr = NULL;
    }

    void TISimulation::initialize()
    {
        _initializeInternal();
    }

    void TISimulation::update()
    {
        _updateInternal();
    }

    void TISimulation::reset()
    {
        _resetInternal();
    }

    TScenario* TISimulation::scenario()
    {
        return m_scenarioPtr;
    }

    std::string TISimulation::type()
    {
        return m_runtimeType;
    }


}