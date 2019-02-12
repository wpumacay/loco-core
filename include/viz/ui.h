
#pragma once

// scenario functionality
#include <scenario.h>

namespace tysoc {
namespace viz {


    struct TIWidget
    {
        std::string name;
        std::string type;
    };

    template<typename T>
    struct TISlider : public TIWidget
    {
        T value;
    };

    typedef TISlider<int> TISliderInt;
    typedef TISlider<float> TISliderFloat;
    typedef TISlider<double> TISliderDouble;

    // ...
    // @TODO: Make abstract widgets wrapping functionality

    class TIVisualizerUI
    {

        protected :

        TScenario* m_scenarioPtr;

        virtual void _initUIInternal() = 0;
        virtual void _renderUIInternal() = 0;

        public :

        TIVisualizerUI( TScenario* scenarioPtr );
        ~TIVisualizerUI();

        void initUI();
        void renderUI();

        // @TODO: add functionality to allow the user create ...
        // its own UI using this as a wrapper, and the python ...
        // binding as a programatical wrapper

    };


}}