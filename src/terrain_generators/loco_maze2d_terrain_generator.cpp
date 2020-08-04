
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
        m_NumTotalCells = maze_count_x * maze_count_y;
        m_CellSize = maze_cell_size;
        m_BottomLeftPos = bottom_left_pos;

        m_Layout2d = std::unique_ptr<char[]>( new char[maze_count_x * maze_count_y] );
        // Construct a default layout
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
        if ( str_layout2d.size() != m_NumTotalCells )
        {
            LOCO_CORE_ERROR( "TMaze2dTerrainGenerator::GenerateLayout >>> mismatch between the total number "
                             "of cells of the given string layout and the dimensions of the maze. Expected "
                             "total={0} but got total={1}. Error found while processing terrain-generator {2}",
                             m_NumTotalCells, str_layout2d.size(), m_name );
            return;
        }
        for ( ssize_t i = 0; i < m_NumTotalCells; i++ )
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
        const ssize_t m_NumTotalCells = m_CountX * m_CountY;
        if ( vec_layout2d.size() != m_NumTotalCells )
        {
            LOCO_CORE_ERROR( "TMaze2dTerrainGenerator::GenerateLayout >>> mismatch between the total number "
                             "of cells of the given std::vector layout and the dimensions of the maze. Expected "
                             "total={0} but got total={1}. Error found while processing terrain-generator {2}",
                             m_NumTotalCells, vec_layout2d.size(), m_name );
            return;
        }
        for ( ssize_t i = 0; i < m_NumTotalCells; i++ )
            m_Layout2d[i] = vec_layout2d[i];
        _UpdateMaze();
    }

    void TMaze2dTerrainGenerator::GenerateRandomLayout()
    {
        // @todo: Implement functionality for random-maze generation
        _UpdateMaze();
    }

    void TMaze2dTerrainGenerator::_UpdateMaze()
    {
        if ( m_BlocksInUse.empty() && m_BlocksAvailable.empty() )
            return; // Terrain-generator might not have been initialized yet

        while ( !m_BlocksInUse.empty() )
        {
            m_BlocksAvailable.push( m_BlocksInUse.front() );
            m_BlocksInUse.pop();
        }
        LOCO_CORE_ASSERT( m_BlocksAvailable.size() == m_SingleBodiesRefs.size(), "TMaze2dTerrainGenerator::_UpdateMaze >>> "
                          "terrain generator {0} doesn't have the required number of available blocks to work with", m_name );

        // Send all cells to their inactive position before constructing the layout
        for ( ssize_t ix = 0; ix < m_CountX; ix++ )
        {
            for ( ssize_t iy = 0; iy < m_CountY; iy++ )
            {
                const ssize_t cell_index = ix + iy * m_CountX;
                const auto cell_pos = TVec3( ( ix + 0.5f ) * m_CellSize.x() + m_BottomLeftPos.x(),
                                             ( iy + 0.5f ) * m_CellSize.y() + m_BottomLeftPos.y(),
                                             0.5f * m_CellSize.z() + 100.0f ); // Send cells to its rest|inactive position
                m_SingleBodiesRefs[cell_index]->SetPosition( cell_pos );
            }
        }
        // Construct the required layout
        for ( ssize_t ix = 0; ix < m_CountX; ix++ )
        {
            for ( ssize_t iy = 0; iy < m_CountY; iy++ )
            {
                const ssize_t cell_index = ix + iy * m_CountX;
                if ( m_Layout2d[cell_index] == CELL_BLOCKED )
                {
                    const auto cell_pos = TVec3( ( ix + 0.5f ) * m_CellSize.x() + m_BottomLeftPos.x(),
                                                 ( iy + 0.5f ) * m_CellSize.y() + m_BottomLeftPos.y(),
                                                 0.5f * m_CellSize.z() );
                    auto cell = m_BlocksAvailable.front();
                    cell->SetPosition( cell_pos );
                    m_BlocksAvailable.pop();
                    m_BlocksInUse.push( cell );
                }
            }
        }
    }

    void TMaze2dTerrainGenerator::_InitializeInternal()
    {
        for ( ssize_t ix = 0; ix < m_CountX; ix++ )
        {
            for ( ssize_t iy = 0; iy < m_CountY; iy++ )
            {
                const auto cell_name = m_name + "_cell_" + std::to_string( ix ) + "_" + std::to_string( iy );
                const auto cell_pos = TVec3( ( ix + 0.5f ) * m_CellSize.x() + m_BottomLeftPos.x(),
                                             ( iy + 0.5f ) * m_CellSize.y() + m_BottomLeftPos.y(),
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
        _UpdateMaze();
    }

    void TMaze2dTerrainGenerator::_PreStepInternal()
    {
        // Nothing to setup prior to a simulation step
    }

    void TMaze2dTerrainGenerator::_PostStepInternal()
    {
        // Nothing to update after a simulation step
    }

    void TMaze2dTerrainGenerator::_SetTransformInternal( const TMat4& tf )
    {
        const auto position = TVec3( tf.col( 3 ) );
        m_BottomLeftPos.x() = position.x();
        m_BottomLeftPos.y() = position.y();
        for ( ssize_t ix = 0; ix < m_CountX; ix++ )
        {
            for ( ssize_t iy = 0; iy < m_CountY; iy++ )
            {
                const ssize_t cell_index = ix + iy * m_CountX;
                if ( m_Layout2d[cell_index] == CELL_BLOCKED )
                    m_SingleBodiesRefs[cell_index]->SetPosition( { ( ix + 0.5f ) * m_CellSize.x() + m_BottomLeftPos.x(), 
                                                                   ( iy + 0.5f ) * m_CellSize.y() + m_BottomLeftPos.y(), 
                                                                   0.5f * m_CellSize.z() } );
                else
                    m_SingleBodiesRefs[cell_index]->SetPosition( { ( ix + 0.5f ) * m_CellSize.x() + m_BottomLeftPos.x(),
                                                                   ( iy + 0.5f ) * m_CellSize.y() + m_BottomLeftPos.y(),
                                                                   0.5f * m_CellSize.z() + 100.0f } );
            }
        }
    }

    std::string TMaze2dTerrainGenerator::ToString() const
    {
        std::string strrep = "Maze2dTerrainGenerator(\n";
        strrep += "name             : " + m_name + "\n";
        strrep += "cpp-address      : " + tinyutils::PointerToHexAddress( this ) + "\n";
        strrep += "count-x          : " + std::to_string( m_CountX ) + "\n";
        strrep += "count-y          : " + std::to_string( m_CountY ) + "\n";
        strrep += "cell-size        : " + loco::ToString( m_CellSize ) + "\n";
        strrep += "cells-in-use     : " + std::to_string( m_BlocksInUse.size() ) + "\n";
        strrep += "cells-available  : " + std::to_string( m_BlocksAvailable.size() ) + "\n";
        strrep += "layout           :\n";
            for ( ssize_t iy = m_CountY - 1; iy >= 0; iy-- )
            {
                strrep += "\t";
                for ( ssize_t ix = 0; ix < m_CountX; ix++ )
                {
                    const ssize_t cell_index = ix + iy * m_CountX;
                    strrep += std::to_string( m_Layout2d[cell_index] );
                }
                strrep += "\n";
            }
        strrep += ")";
        return strrep;
    }

    std::string TMaze2dTerrainGenerator::str_layout2d() const
    {
        std::string strrep;
        for ( ssize_t i = 0; i < m_NumTotalCells; i++ )
            strrep.push_back( m_Layout2d[i] );
        return strrep;
    }

    std::vector<char> TMaze2dTerrainGenerator::vec_layout2d() const
    {
        std::vector<char> vecrep;
        for ( ssize_t i = 0; i < m_NumTotalCells; i++ )
            vecrep.push_back( m_Layout2d[i] );
        return vecrep;
    }
}}