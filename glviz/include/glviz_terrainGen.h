
#pragma once

#include <vector>
#include <iostream>
#include <string>

#include <LMeshBuilder.h>
#include <LScene.h>
#include <LAssetsManager.h>

#include <terrain/terrain_base.h>
// some commom functionality
#include <glviz_common.h>

#define VIZTERRAIN_AXES_DEFAULT_SIZE 0.1f

namespace tysoc {

    struct TGLVizTerrainPrimitive
    {
        engine::LModel*                 axesPtr;
        engine::LIRenderable*           meshPtr;
        terrain::TTerrainPrimitive*     terrainPrimitivePtr;
    };

    struct TGLVizTerrainDrawState
    {
        bool drawAsWireframe;
        bool showFrameAxes;
        bool showPrimitives;

        TGLVizTerrainDrawState()
        {
            drawAsWireframe     = false;
            showFrameAxes       = false;
            showPrimitives      = true;
        }
    };

    class TGLVizTerrainGenerator
    {

        private :

        engine::LScene*                 m_scenePtr;
        terrain::TITerrainGenerator*    m_terrainGeneratorPtr;
        std::string                     m_workingDir;

        std::vector< TGLVizTerrainPrimitive > m_vizTerrainPrimitives;

        void _collectTerrainPrimitives();

        engine::LIRenderable* _createMeshPrimitive( terrain::TTerrainPrimitive* terrainPrimitivePtr );
        engine::LIRenderable* _createMeshFromData( terrain::TTerrainPrimitive* terrainPrimitivePtr );

        void _resizeMesh( engine::LIRenderable* renderablePtr, terrain::TTerrainPrimitive* terrainPrimitivePtr );
        void _updateVizTerrainPrimitive( TGLVizTerrainPrimitive& vizTerrainPrimitivePtr );

        public :

        TGLVizTerrainGenerator( terrain::TITerrainGenerator* terrainGeneratorPtr,
                                    engine::LScene* scenePtr,
                                    const std::string& workingDir );
        ~TGLVizTerrainGenerator();

        TGLVizTerrainDrawState drawState;

        void update();
        terrain::TITerrainGenerator* getTerrainGeneratorPtr();

    };


}