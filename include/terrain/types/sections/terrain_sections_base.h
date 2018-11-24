
#pragma once

#include <terrain/terrain_base.h>

namespace tysocterrain
{


    class TSectionsTerrainGenerator : public TTerrainGenerator
    {

        protected :

        float m_startX;
        float m_startY;
        float m_startZ;

        float m_sectionDepth;

        virtual void _initializeSection() = 0;
        virtual void _updateSection() = 0;
        virtual float _computeProfile1D( float x ) = 0;

        public :

        TSectionsTerrainGenerator( const std::string& name,
                                   float startX,
                                   float startY,
                                   float startZ,
                                   float sectionDepth );
        ~TSectionsTerrainGenerator();

        void setStart( float x, float y, float z );
        void initialize() override;
        void update() override;

        // Should be implemented in order to be used by the terrainsensor
        float getProfile1D( float x );
    };


}