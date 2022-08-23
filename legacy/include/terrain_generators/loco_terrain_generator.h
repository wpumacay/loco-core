#pragma once

#include <loco_scenario.h>
#include <primitives/loco_single_body_primitives.h>

namespace loco {
    class TScenario;
}

namespace loco {
namespace terrain {

    class TITerrainGenerator : public TObject
    {
    public :

        TITerrainGenerator( const std::string& name, TScenario* scenario_ref );

        TITerrainGenerator( const TITerrainGenerator& other ) = delete;

        TITerrainGenerator& operator=( const TITerrainGenerator& other ) = delete;

        virtual ~TITerrainGenerator();

        void Reset();

        std::vector<primitives::TSingleBody*> primitives();

        std::vector<const primitives::TSingleBody*> primitives() const;

    protected :

        virtual void _ResetInternal() = 0;

        void _DetachSimInternal() override;

        void _DetachVizInternal() override;

        eObjectType _GetObjectTypeInternal() const { return eObjectType::TERRAIN_GENERATOR; }

    protected :

        /// Reference to the scenario this generator will create primitives in
        TScenario* m_ScenarioRef = nullptr;

        /// Container used to store references to the single-bodies that this generator creates
        std::vector<primitives::TSingleBody*> m_SingleBodiesRefs;
    };
}}