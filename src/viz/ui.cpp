
#include <viz/ui.h>


namespace tysoc {
namespace viz {


    TIVisualizerUI::TIVisualizerUI( TScenario* scenarioPtr )
    {
        m_scenarioPtr = scenarioPtr;
    }

    TIVisualizerUI::~TIVisualizerUI()
    {
        m_scenarioPtr = NULL;
    }

    void TIVisualizerUI::initUI()
    {
        _initUIInternal();
    }

    void TIVisualizerUI::renderUI()
    {
        _renderUIInternal();
    }


}}