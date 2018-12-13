
#pragma once

#include <terrain/types/sections/terrain_sections_base.h>
#include <terrain/types/sections/terrain_sections_connectedpath_profiles.h>

namespace tysoc {
namespace terrain {

    /**
     *   The type of scenarios created here are of the form :
     * 
     * 
     *      __________
     *    /|          |\                     ______
     *   / |          | \                  /|      |\
     *  /  |          |  \                / |      | \          
     *     |__________|   \ _____________/  |      |  \        ______________________
     *     /          \    |            |   |      |   |      |   \|    |/
     *    /            \   |            |   |______|   |      |    |    |
     *   /              \  |            |  /        \  |      |    |    |               ... 
     *                   \ |            | /          \ |      |    |    |
     *                    \|____________|/            \|      |___/|____|\___________
     *                      
     */


    struct TPathTerrainPrimitive : public TTerrainPrimitive
    {
        struct { float x; float y; float z; }   p1;
        struct { float x; float y; float z; }   p2;
        struct { float x; float y; float z; }   p3;
        struct { float x; float y; float z; }   p4;
        struct { float x; float y; float z; }   normal;

        TPathTerrainPrimitive() 
        { 
            type     = "path"; 
            geomType = "box";
        }
    };

    class TPathTerrainGenerator : public TSectionsTerrainGenerator
    {

        private :

        float m_currentX; // current position in the axis of generation
        float m_componentsThickness;
        float m_componentsSpacingX;

        TProfileGenerator* m_generator;

        void _initializeSection() override;
        void _updateSection() override;
        float _computeProfile1D( float x, float y ) override;

        void _createPathComponent();

        public :

        TPathTerrainGenerator( const std::string& name,
                               float startX,
                               float startY,
                               float startZ,
                               float sectionDepth,
                               float componentsSpacingX, 
                               float componentsThickness,
                               TProfileGenerator* generator );
        ~TPathTerrainGenerator();
    };

}}