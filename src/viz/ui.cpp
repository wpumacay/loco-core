
#include <viz/ui.h>


namespace tysoc {
namespace viz {


    TIVisualizerUI::TIVisualizerUI( TTysocCommonApi* tysocApiPtr )
    {
        m_tysocApiPtr = tysocApiPtr;
    }

    TIVisualizerUI::~TIVisualizerUI()
    {
        m_tysocApiPtr = NULL;
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