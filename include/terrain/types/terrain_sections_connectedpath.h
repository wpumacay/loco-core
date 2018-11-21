
#pragma once

#include <terrain/terrain_base.h>


namespace tysocterrain
{


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

    struct TProfileGenerator
    {
        virtual float generate( float x ) = 0;
    };

    struct TSineProfileGenerator : public TProfileGenerator
    {
        private :

        float m_period;
        float m_phase;
        float m_ampl;

        public :

        TSineProfileGenerator( float ampl, float period, float phase );
        float generate( float x ) override;
    };

    class TPathTerrainGenerator : public TTerrainGenerator
    {

        private :

        float m_currentX; // current position in the axis of generation
        float m_baseY;
        float m_baseZ;
        float m_sectionsDepth;
        float m_sectionsTickness;
        float m_sectionsDeltaX;

        TProfileGenerator* m_generator;

        void _updateSections( TGeneratorInfo* generatorInfo );
        void _createSection();

        public :

        TPathTerrainGenerator( const std::string& name,
                               TProfileGenerator* generator,
                               float sectionDeltaX, 
                               float sectionsDepth, 
                               float sectionTickness,
                               float baseY = 0.0f,
                               float baseZ = 0.0f );
        ~TPathTerrainGenerator();

        void initialize() override;
        void update() override;
        float getProfile1D( float x );
    };


}