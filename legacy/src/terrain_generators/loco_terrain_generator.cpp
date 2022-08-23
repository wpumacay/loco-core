
#include <terrain_generators/loco_terrain_generator.h>

namespace loco {
namespace terrain {

    TITerrainGenerator::TITerrainGenerator( const std::string& name, TScenario* scenario_ref )
        : TObject( name )
    {
        m_ScenarioRef = scenario_ref;
    }

    TITerrainGenerator::~TITerrainGenerator()
    {
        m_ScenarioRef = nullptr;
        m_SingleBodiesRefs.clear();
    }

    void TITerrainGenerator::Reset()
    {
        _ResetInternal();
    }

    void TITerrainGenerator::_DetachSimInternal()
    {
        for ( auto primitive_ref : m_SingleBodiesRefs )
            primitive_ref->DetachSim();
    }

    void TITerrainGenerator::_DetachVizInternal()
    {
        for ( auto primitive_ref : m_SingleBodiesRefs )
            primitive_ref->DetachViz();
    }

    std::vector<primitives::TSingleBody*> TITerrainGenerator::primitives()
    {
        std::vector<primitives::TSingleBody*> vec_primitives;
        for ( auto primitive_ref : m_SingleBodiesRefs )
            vec_primitives.push_back( primitive_ref );
        return vec_primitives;
    }

    std::vector<const primitives::TSingleBody*> TITerrainGenerator::primitives() const
    {
        std::vector<const primitives::TSingleBody*> vec_primitives;
        for ( auto primitive_ref : m_SingleBodiesRefs )
            vec_primitives.push_back( primitive_ref );
        return vec_primitives;
    }
}}