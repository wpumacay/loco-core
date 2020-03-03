
#pragma once

#include <terrain/types/sections/terrain_sections_base.h>
#include <terrain/types/sections/terrain_sections_connectedpath_profiles.h>

namespace tysoc {

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
        TVec3 p1;
        TVec3 p2;
        TVec3 p3;
        TVec3 p4;
        TVec3 normal;

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

}