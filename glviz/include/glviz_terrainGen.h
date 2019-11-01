#pragma once

#include <glviz_common.h>
#include <terrain/terrain_base.h>

// resources from rendering engine
#include <graphics/CScene.h>
#include <utils/CDebugDrawer.h>
#include <assets/CTextureManager.h>

using namespace tysoc::terrain;

namespace tysoc {

    struct TGLTerrainDrawable
    {
        TVec3                   size;
        TVec3                   size0;
        TTerrainPrimitive*      primitive;
        engine::CIRenderable*   renderable;
    };

    class TGLVizTerrainGenerator
    {

    public :

        TGLVizTerrainGenerator( terrain::TITerrainGenerator* terrainGeneratorPtr,
                                engine::CScene* scenePtr,
                                const std::string& workingDir );
        ~TGLVizTerrainGenerator();

        void update();

    private :

        void _collectTerrainPrimitives();
        void _updateTerrainDrawable( TGLTerrainDrawable terrainDrawable );

    private :

        engine::CScene*         m_scenePtr;
        TITerrainGenerator*     m_terrainGeneratorPtr;
        std::string             m_workingDir;

        std::vector< TGLTerrainDrawable > m_terrainDrawables;

    };

}