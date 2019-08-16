
#include <viz/ui.h>


namespace tysoc {
namespace viz {


    TIVisualizerUI::TIVisualizerUI( TScenario* scenarioPtr )
    {
        m_scenarioPtr = scenarioPtr;
        m_simulationPtr = NULL;
    }

    TIVisualizerUI::~TIVisualizerUI()
    {
        m_scenarioPtr = NULL;
        m_simulationPtr = NULL;
    }

    void TIVisualizerUI::initUI()
    {
        _initUIInternal();
    }

    void TIVisualizerUI::renderUI()
    {
        _renderUIInternal();
    }

    void TIVisualizerUI::setSimulation( TISimulation* simulationPtr )
    {
        m_simulationPtr = simulationPtr;
    }

}}