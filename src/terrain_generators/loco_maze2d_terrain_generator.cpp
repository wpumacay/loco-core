
#include <terrain_generators/loco_maze2d_terrain_generator.h>

namespace loco {
namespace terrain {

    TMaze2dTerrainGenerator::TMaze2dTerrainGenerator( const std::string& name, TScenario* scenario_ref,
                                                      const ssize_t& maze_count_x, const ssize_t& maze_count_y,
                                                      const TVec3& maze_cell_size, const TVec2& bottom_left_pos )
        : TITerrainGenerator( name, scenario_ref )
    {
        m_CountX = maze_count_x;
        m_CountY = maze_count_y;
        m_CellSize = maze_cell_size;
        m_BottomLeftPos = bottom_left_pos;

        m_Layout2d = std::unique_ptr<char[]>( new char[maze_count_x * maze_count_y] );
        // Construct the default layout
        for ( ssize_t ix = 0; ix < m_CountX; ix++ )
        {
            for ( ssize_t iy = 0; iy < m_CountY; iy++ )
            {
                const ssize_t cell_index = ix + iy * m_CountX;
                const bool cell_is_blocked = ( ix % 2 == 0 ) && ( iy % 2 == 0 );
                m_Layout2d[cell_index] = ( cell_is_blocked ? CELL_BLOCKED : CELL_EMPTY );
            }
        }
    }

    void TMaze2dTerrainGenerator::GenerateLayout( const std::string& str_layout2d )
    {
        // Layouts in string format are grids of countX by countY characters, as shown below :
        //
        // std::string layout =  "*xx**"     * x x * *
        //                       "x**x*"     x * * x *   where:
        //                       "**x**" <=> * * x * *     *: empty
        //                       "*x***"     * x * * *     x: blocked
        //                       "xxxx*";    x x x x *
        const ssize_t num_total_cells = m_CountX * m_CountY;
        if ( str_layout2d.size() != num_total_cells )
        {
            LOCO_CORE_ERROR( "TMaze2dTerrainGenerator::GenerateLayout >>> mismatch between the total number "
                             "of cells of the given string layout and the dimensions of the maze. Expected "
                             "total={0} but got total={1}. Error found while processing terrain-generator {2}",
                             num_total_cells, str_layout2d.size(), m_name );
            return;
        }
        for ( ssize_t i = 0; i < num_total_cells; i++ )
            m_Layout2d[i] = str_layout2d[i];
        _UpdateMaze();
    }

    void TMaze2dTerrainGenerator::GenerateLayout( const std::vector<char>& vec_layout2d )
    {
        // Layouts om vector format are grids of countX by countY characters stored in row-major order, as shown below :
        //
        // std::vector<char> layout = {'*','x','x','*','*',     * x x * *
        //                             'x','*','*','x','*',     x * * x *   where:
        //                             '*','*','x','*','*', <=> * * x * *     *: empty
        //                             '*','x','*','*','*',     * x * * *     x: blocked
        //                             'x','x','x','x','*'};    x x x x *
        const ssize_t num_total_cells = m_CountX * m_CountY;
        if ( vec_layout2d.size() != num_total_cells )
        {
            LOCO_CORE_ERROR( "TMaze2dTerrainGenerator::GenerateLayout >>> mismatch between the total number "
                             "of cells of the given std::vector layout and the dimensions of the maze. Expected "
                             "total={0} but got total={1}. Error found while processing terrain-generator {2}",
                             num_total_cells, vec_layout2d.size(), m_name );
            return;
        }
        for ( ssize_t i = 0; i < num_total_cells; i++ )
            m_Layout2d[i] = vec_layout2d[i];
        _UpdateMaze();
    }

    void TMaze2dTerrainGenerator::GenerateRandomLayout()
    {

        _UpdateMaze();
    }

    void TMaze2dTerrainGenerator::_UpdateMaze()
    {
        if ( m_BlocksInUse.empty() && m_BlocksAvailable.empty() )
            return; // Terrain-generator might not have been initialized yet
    }

    void TMaze2dTerrainGenerator::_InitializeInternal()
    {
        for ( ssize_t ix = 0; ix < m_CountX; ix++ )
        {
            for ( ssize_t iy = 0; iy < m_CountY; iy++ )
            {
                const auto cell_name = m_name + "_cell_" + std::to_string( ix ) + "_" + std::to_string( iy );
                const auto cell_pos = TVec3( ( ix + 0.5f ) * m_CellSize.x() + ix * 0.2f + m_BottomLeftPos.x(),
                                             ( iy + 0.5f ) * m_CellSize.y() + iy * 0.2f + m_BottomLeftPos.y(),
                                             0.5f * m_CellSize.z() + 100.0f ); // Send cells to its rest|inactive position
                const auto cell_rot = TMat3();
                auto cell = m_ScenarioRef->AddSingleBody( std::make_unique<primitives::TBox>( 
                                                            cell_name, m_CellSize, cell_pos, cell_rot, eDynamicsType::STATIC ) );
                m_SingleBodiesRefs.push_back( cell );
                m_BlocksAvailable.push( cell );
            }
        }
        _UpdateMaze();
    }

    void TMaze2dTerrainGenerator::_ResetInternal()
    {

    }

    void TMaze2dTerrainGenerator::_PreStepInternal()
    {

    }

    void TMaze2dTerrainGenerator::_PostStepInternal()
    {

    }

    void TMaze2dTerrainGenerator::_SetTransformInternal( const TMat4& tf )
    {

    }

    std::string TMaze2dTerrainGenerator::str_layout2d() const
    {
        std::string strrep;

        return strrep;
    }

    std::vector<char> TMaze2dTerrainGenerator::vec_layout2d() const
    {
        std::vector<char> vecrep;

        return vecrep;
    }

}}