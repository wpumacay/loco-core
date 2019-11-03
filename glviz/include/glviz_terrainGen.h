#pragma once

#include <glviz_common.h>
#include <terrain/terrain_base.h>

// resources from rendering engine
#include <graphics/CScene.h>
#include <utils/CDebugDrawer.h>
#include <assets/CTextureManager.h>

using namespace engine;

namespace tysoc {

    struct TGLTerrainDrawable
    {
        TVec3               size;
        TVec3               size0;
        TTerrainPrimitive*  primitive;
        CIRenderable*       renderable;
    };

    class TGLVizTerrainGenerator
    {

    public :

        TGLVizTerrainGenerator( TITerrainGenerator* terrainGeneratorPtr,
                                CScene* scenePtr );
        ~TGLVizTerrainGenerator();

        void update();

    private :

        void _collectTerrainPrimitives();
        void _updateTerrainDrawable( TGLTerrainDrawable terrainDrawable );

    private :

        CScene*             m_scenePtr;
        TITerrainGenerator* m_terrainGeneratorPtr;

        std::vector< TGLTerrainDrawable > m_terrainDrawables;

    };

}