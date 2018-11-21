
#pragma once

// just in case
#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <iostream>
#include <queue>
#include <vector>


namespace tysocterrain
{

    struct TGeneratorInfo
    {
        // some time information
        float time;
        float deltatime;
        // position to use for scenario calculations
        struct { float x; float y; float z ;} trackingpoint;
        // camera frustum to cull things
        float camfrustum[9];
    };


    struct TTerrainPrimitive
    {
        std::string                             type;
        std::string                             geomType;
        struct { float x; float y; float z; }   size;
        struct { float x; float y; float z; }   pos;
        float                                   rotmat[9];
        float                                   rbound;
        bool                                    inUse;
    };

    class TTerrainGenerator
    {
        protected :

        std::queue< TTerrainPrimitive* > m_pool;
        std::queue< TTerrainPrimitive* > m_justCreated;

        TGeneratorInfo* m_generatorInfo;

        std::string m_name;

        public :

        TTerrainGenerator( const std::string& name );
        ~TTerrainGenerator();

        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void recycle( TTerrainPrimitive* pPrimitive );

        std::string name() { return m_name; }
        TGeneratorInfo* generatorInfo() { return m_generatorInfo; }

        void flushJustCreatedQueue();
        std::queue< TTerrainPrimitive* > getJustCreated() { return m_justCreated; }
    };

}