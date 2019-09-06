
#pragma once

#include <scenario.h>
#include <simulation_base.h>

namespace tysoc {

    class TISimulation;

    class TIVisualizerUI
    {

    public :

        TIVisualizerUI( TScenario* scenarioPtr );
        virtual ~TIVisualizerUI();

        void initUI();
        void renderUI();
        void setSimulation( TISimulation* simulationPtr );

    protected :

        virtual void _initUIInternal() = 0;
        virtual void _renderUIInternal() = 0;

        TScenario* m_scenarioPtr;
        TISimulation* m_simulationPtr;

    };

}