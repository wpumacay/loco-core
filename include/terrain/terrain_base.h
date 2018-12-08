
#pragma once

#include <terrain/terrain_common.h>

#define PROCEDURAL_TERRAIN_POOL_SIZE 100

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
        struct { float r; float g; float b; }   color;
        struct { float x; float y; float z; }   size;
        struct { float x; float y; float z; }   pos;
        float                                   rotmat[9];
        float                                   rbound;
        bool                                    inUse;
        bool                                    useCustomColor;
    };

    class TTerrainGenerator
    {
        protected :

        // this will hold all primitives created in the generator lifetime
        std::vector< TTerrainPrimitive* > m_primitives;
        // this are working structures, and contain references for reuse
        std::queue< TTerrainPrimitive* > m_pool;
        std::queue< TTerrainPrimitive* > m_justCreated;
        std::queue< TTerrainPrimitive* > m_fixed;

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
        void flushFixedQueue();
        std::queue< TTerrainPrimitive* > getFixed() { return m_fixed; }
        std::queue< TTerrainPrimitive* > getJustCreated() { return m_justCreated; }
        std::vector< TTerrainPrimitive* > getPrimitives() { return m_primitives; }
    };

}