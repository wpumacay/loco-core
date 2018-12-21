
#pragma once

// tysocBaseApi functionality
#include <api_adapter.h>

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
    // @TODO: Make an abstract for the widgets wrapping functionality

    class TIVisualizerUI
    {

        protected :

        TTysocCommonApi* m_tysocApiPtr;

        virtual void _initUIInternal() = 0;
        virtual void _renderUIInternal() = 0;

        public :

        TIVisualizerUI( TTysocCommonApi* tysocApiPtr );
        ~TIVisualizerUI();

        void initUI();
        void renderUI();

        // @TODO: add functionality to allow the user create ...
        // its own UI using this as a wrapper, and the python ...
        // binding as a programatical wrapper

    };


}}