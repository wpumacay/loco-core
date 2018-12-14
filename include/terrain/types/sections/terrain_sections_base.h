
#pragma once

#include <terrain/terrain_base.h>

namespace tysoc {
namespace terrain {


    class TSectionsTerrainGenerator : public TITerrainGenerator
    {

        protected :

        float m_startX;
        float m_startY;
        float m_startZ;

        float m_sectionDepth;

        virtual void _initializeSection() = 0;
        virtual void _updateSection() = 0;
        virtual float _computeProfile1D( float x, float y ) = 0;

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
        float getProfile1D( float x, float y );

        float getSectionDepth() { return m_sectionDepth; }
    };


}}