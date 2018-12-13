
#pragma once

#include <terrain/terrain_common.h>

namespace tysoc {
namespace terrain {

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
    
}}