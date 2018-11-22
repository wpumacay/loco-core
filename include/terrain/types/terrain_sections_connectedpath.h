
#pragma once

#include <terrain/terrain_base.h>

#define _RANDOM( a, b ) ( a + ( b - a ) * ( std::rand() / ( float )RAND_MAX ) )

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

    struct TPerlin1DProfileGenerator : public TProfileGenerator
    {
        private :

        struct Vec2
        {
            float x;
            float y;

            Vec2() { x = 0.0f; y = 0.0f; }
            Vec2( float x, float y ) { this->x = x; this->y = y; }
        };

        std::vector< int > m_permutations;
        std::vector< Vec2 > m_octaveOffsets;

        int     m_octaves;
        float   m_persistance;
        float   m_lacunarity;
        float   m_noiseScale;

        float _lerp( float a, float b, float t );
        float _fade( float t );
        float _dotgrad( int hash, float xf, float yf );
        float _perlin2d( float x, float y );

        public :

        TPerlin1DProfileGenerator();
        TPerlin1DProfileGenerator( int octaves,
                                   float persistance, 
                                   float lacunarity, 
                                   float noiseScale );
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
                               float baseX = 0.0f,
                               float baseY = 0.0f,
                               float baseZ = 0.0f );
        ~TPathTerrainGenerator();

        void initialize() override;
        void update() override;
        float getProfile1D( float x );
    };


}