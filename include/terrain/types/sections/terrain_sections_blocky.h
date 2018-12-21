
#pragma once

#include <terrain/types/sections/terrain_sections_base.h>

#define DEFAULT_SECTION_BASE_HEIGHT 0.25f

namespace tysoc {
namespace terrain {


    /**
     *   The type of scenarios created here are of the form :
     *
     *                                                          ______________________________________________________
     *                                              ___________/___________                                          /
     *                                             /                      /|            _____________________       /
     *                                            /                      / |           /                    /|     /
     *                                           /______________________/  |          / floating platforms //     /
     *                                          |                      |   |         /        (5)         //.    /
     *                                          |                      |   |        /___________________ // .   /
     *                                          |   _                  |   |        |___________________|/  .  /
     *                                          |                      |   |        .  .                .   . /
     *                                          |  (4)   walls         |   |        .  .                .   ./
     *                                          |                      |   |        .  .                .   /
     *                                          |                      |   |        .  .                .  /
     *                                          |                      |  /         .                   . /
     *                                          |                      | /          .                   ./
     *                                          |______________________|/                               /
     *                                          /                                                      /
     *                                         /                                                      /
     *                                        /______________________________________________________/
     *                                                                                                                                                     
     *                                     _______________________________________________________
     *                                    /                                                      /
     *                                   /                       gaps                           /
     *                                  /                                                      /
     *                                 /______________________________________________________/
     *                                _______________________________________________________
     *                               /                                                      /
     *                              /                                                      /
     *                             /                                                      /
     *                            /             (3)        gaps                          /
     *                           /                                                      /
     *                          /                                                      /
     *                         /______________________________________________________/
     *                        _______________________________________________________
     *                       /                                                      /
     *                      /                         gaps                         /
     *                     /                                                      /
     *                    /______________________________________________________/
     *                    ______________________________________________________
     *                ___/__________                                           /
     *               /             /|                                         /
     *              /             / |                                        /
     *             /____________ /  |                                       /
     *            |             |   |                                      /
     *            |(2)obstacles |  /                                      /
     *            |             | /                                      /
     *            |_____________|/                                      /
     *       ____/__________________________________________________   /
     *     /                                                       /| /
     *    /______________________________________________________ / |/          
     *    |                                                      |  /             | height
     *    |         (1)         hurdles                          | /              |  
     *    |______________________________________________________|/               |  /
     +    /                                                      /                | / width
     *   /                                                      /                 |/
     *  /______________________________________________________/      ------------
     *                                                                   depth             
     *  
     *  
     *  (1) hurdles :
     *      * spawnSide        -> N/A
     *      * percentDepth     -> [dmin, dmax] = [1.0,1.0]
     *      * baseHeight       -> bheight = given by the user
     *      * percentHeight    -> [hmin, hmax] = given by the user
     *      * baseWidth        -> bwidth = given by the user
     *      * percentWidth     -> [wmin, hmax] = given by the user
     *      * baseSpacingX     -> bspacingX = given by the user
     *      * percentSpacingX  -> [psmin, psmax] = given by the user
     *      * baseOffsetZ      -> boffsetZ = N/A
     *      * percentOffsetZ   -> [pozmin, pozmax] = N/A
     *      * usesBase         -> {true} to include the base (should be fixed non-removable)
     *  
     *  (2) obstacles :
     *      * spawnSide        -> {-1, +1} <> { left, right } random
     *      * percentDepth     -> [dmin, dmax] = given by the user
     *      * baseHeight       -> bheight = given by the user
     *      * percentHeight    -> [hmin, hmax] = given by the user
     *      * baseWidth        -> bwidth = given by the user
     *      * percentWidth     -> [wmin, hmax] = given by the user
     *      * baseSpacingX     -> bspacingX = given by the user
     *      * percentSpacingX  -> [psmin, psmax] = given by the user
     *      * baseOffsetZ      -> boffsetZ = N/A
     *      * percentOffsetZ   -> [pozmin, pozmax] = N/A
     *      * usesBase         -> {true} to include the base (should be fixed non-removable)

     *  (3) gaps :
     *      * spawnSide        -> N/A
     *      * percentDepth     -> [dmin, dmax] = [1.0,1.0]
     *      * baseHeight       -> bheight = a tuned constant
     *      * percentHeight    -> [hmin, hmax] = [1.0,1.0] or could be given by the user (to allow a bit more challenge)
     *      * baseWidth        -> bwidth = given by the user
     *      * percentWidth     -> [wmin, hmax] = given by the user
     *      * baseSpacingX     -> bspacingX = given by the user
     *      * percentSpacingX  -> [psmin, psmax] = given by the user
     *      * baseOffsetZ      -> boffsetZ = given by the user
     *      * percentOffsetZ   -> [pozmin, pozmax] = given by the user
     *      * usesBase         -> {false}
     *
     *  (4) walls :
     *      * spawnSide        -> {-1, +1} <> { left, right }
     *      * percentDepth     -> [dmin, dmax] = given by the user
     *      * baseHeight       -> bheight = a tuned constant
     *      * percentHeight    -> [hmin, hmax] = [1.0,1.0]
     *      * baseWidth        -> bwidth = given by the user
     *      * percentWidth     -> [wmin, hmax] = given by the user
     *      * baseSpacingX     -> bspacingX = given by the user
     *      * percentSpacingX  -> [psmin, psmax] = given by the user
     *      * baseOffsetZ      -> boffsetZ = N/A
     *      * percentOffsetZ   -> [pozmin, pozmax] = N/A
     *      * usesBase         -> {true} to include the base (should be fixed non-removable)
     *                                                     
     *  (5) platforms :
     *      * spawnSide        -> {-1, +1} <> { left, right }
     *      * percentDepth     -> [dmin, dmax] = given by the user
     *      * baseHeight       -> bheight = a tuned constant
     *      * percentHeight    -> [hmin, hmax] = [1.0,1.0]
     *      * baseWidth        -> bwidth = given by the user
     *      * percentWidth     -> [wmin, hmax] = given by the user                                                        
     *      * baseSpacingX     -> bspacingX = given by the user
     *      * percentSpacingX  -> [psmin, psmax] = given by the user
     *      * baseOffsetZ      -> boffsetZ = given by the user
     *      * percentOffsetZ   -> [pozmin, pozmax] = given by the user
     *      * usesBase         -> {true} to include the base (should be fixed non-removable)
     */


    struct TBlockyParams
    {
        bool usesBase;
        bool usesSides;

        float sectionLength;

        float baseDepth;
        float baseHeight;
        float baseWidth;
        float baseSpacingX;
        float baseOffsetZ;

        struct { float min; float max; } percentDepth;
        struct { float min; float max; } percentHeight;
        struct { float min; float max; } percentWidth;
        struct { float min; float max; } percentSpacingX;
        struct { float min; float max; } percentOffsetZ;
    };

    struct TBlockyTerrainPrimitive : public TTerrainPrimitive
    {
        struct { float p1x; float p1y; float p1z;
                 float p2x; float p2y; float p2z; } AABB;

        TBlockyTerrainPrimitive() 
        { 
            type     = "blocky"; 
            geomType = "box";
        }
    };


    class TBlockyTerrainGenerator : public TSectionsTerrainGenerator
    {

        private :

        // lastSpawnX is used for the spawn condition
        float m_lastSpawnX;
        // currentSpawnX looks ahead one spaceX
        float m_currentSpawnX;

        TBlockyTerrainPrimitive* m_basePrimitivePtr;
        TBlockyParams m_params;

        void _initializeSection() override;
        void _updateSection() override;
        float _computeProfile1D( float x, float y ) override;

        void _createBase();
        void _createBlock();

        public :

        TBlockyTerrainGenerator( const std::string& name,
                                 float startX,
                                 float startY,
                                 float startZ,
                                 const TBlockyParams& params );

        ~TBlockyTerrainGenerator();

        void setParams( const TBlockyParams& params ) { m_params = params; }
        TBlockyParams getParams() const { return m_params; }

    };


}}