
#pragma once

#include <terrain/terrain_common.h>

#define PROCEDURAL_TERRAIN_POOL_SIZE 100

namespace tysoc {
namespace terrain {

    const std::string TERRAIN_TYPE_STATIC                       = "static";
    const std::string TERRAIN_TYPE_PROCEDURAL_SECTIONS_PATH     = "proc_sect_path";
    const std::string TERRAIN_TYPE_PROCEDURAL_SECTIONS_BLOCKS   = "proc_sect_blocks";

    struct TGeneratorInfo
    {
        // some time information
        float time;
        float deltatime;
        // position to use for scenario calculations
        struct { float x; float y; float z; } trackingpoint;
        // camera frustum to cull things
        float camfrustum[9];
    };


    struct TTerrainPrimitive
    {
        std::string                             type;
        std::string                             geomType;
        std::string                             filename;
        std::string                             texturename;
        struct { float r; float g; float b; }   color;
        struct { float x; float y; float z; }   size;
        struct { float x; float y; float z; }   pos;
        float                                   rotmat[9];
        float                                   rbound;
        bool                                    inUse;
        bool                                    useCustomColor;
    };

    class TITerrainGenerator
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
        std::string m_type;

        public :

        TITerrainGenerator( const std::string& name );
        ~TITerrainGenerator();

        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void recycle( TTerrainPrimitive* pPrimitive );

        std::string name() { return m_name; }
        std::string type() { return m_type; }
        TGeneratorInfo* generatorInfo() { return m_generatorInfo; }

        void flushJustCreatedQueue();
        void flushFixedQueue();
        std::queue< TTerrainPrimitive* > getFixed() { return m_fixed; }
        std::queue< TTerrainPrimitive* > getJustCreated() { return m_justCreated; }
        std::vector< TTerrainPrimitive* > getPrimitives() { return m_primitives; }
    };

}}