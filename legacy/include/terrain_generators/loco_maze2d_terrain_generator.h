#pragma once

#include <terrain_generators/loco_terrain_generator.h>

namespace loco {
namespace terrain {

    class TMaze2dTerrainGenerator : public TITerrainGenerator
    {
    public :

        static constexpr char CELL_EMPTY = '*';
        static constexpr char CELL_BLOCKED = 'x';

        TMaze2dTerrainGenerator( const std::string& name, TScenario* scenario_ref,
                                 const ssize_t& maze_count_x, const ssize_t& maze_count_y,
                                 const TVec3& maze_cell_size, const TVec2& bottom_left_pos = { 0.0f, 0.0f } );

        TMaze2dTerrainGenerator( const TMaze2dTerrainGenerator& other ) = delete;

        TMaze2dTerrainGenerator& operator=( const TMaze2dTerrainGenerator& other ) = delete;

        ~TMaze2dTerrainGenerator() = default;

        void GenerateLayout( const std::string& str_layout2d );

        void GenerateLayout( const std::vector<char>& vec_layout2d );

        void GenerateRandomLayout();

        ssize_t count_x() const { return m_CountX; }

        ssize_t count_y() const { return m_CountY; }

        TVec3 cell_size() const { return m_CellSize; }

        TScalar cell_size_x() const { return m_CellSize.x(); }

        TScalar cell_size_y() const { return m_CellSize.y(); }

        TScalar cell_size_z() const { return m_CellSize.z(); }

        TVec3 maze_size() const { return TVec3( m_CountX * m_CellSize.x(), m_CountY * m_CellSize.y(), m_CellSize.z() ); }

        TScalar maze_size_x() const { return m_CountX * m_CellSize.x(); }

        TScalar maze_size_y() const { return m_CountY * m_CellSize.y(); }

        TScalar maze_size_z() const { return m_CellSize.z(); }

        TVec2 bottom_left_pos() const { return m_BottomLeftPos; }

        std::string ToString() const;

        std::string str_layout2d() const;

        std::vector<char> vec_layout2d() const;

    protected :

        void _InitializeInternal() override;

        void _ResetInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _SetTransformInternal( const TMat4& tf ) override;

    private :

        void _UpdateMaze();

    private :

        ssize_t m_CountX;

        ssize_t m_CountY;

        ssize_t m_NumTotalCells;

        TVec3 m_CellSize;

        TVec2 m_BottomLeftPos;

        std::unique_ptr<char[]> m_Layout2d;

        std::queue<primitives::TSingleBody*> m_BlocksInUse;

        std::queue<primitives::TSingleBody*> m_BlocksAvailable;
    };
}}