
#include <glviz_terrainGen.h>

namespace tysoc {

    TGLVizTerrainGenerator::TGLVizTerrainGenerator( TITerrainGenerator* terrainGeneratorPtr,
                                                    engine::CScene* scenePtr )
    {
        m_scenePtr              = scenePtr;
        m_terrainGeneratorPtr   = terrainGeneratorPtr;

        _collectTerrainPrimitives();
    }

    TGLVizTerrainGenerator::~TGLVizTerrainGenerator()
    {
        m_scenePtr              = nullptr;
        m_terrainGeneratorPtr   = nullptr;

        m_terrainDrawables.clear();
    }

    void TGLVizTerrainGenerator::update()
    {
        for ( auto _terrainDrawable : m_terrainDrawables )
            _updateTerrainDrawable( _terrainDrawable );
    }

    void TGLVizTerrainGenerator::_collectTerrainPrimitives()
    {
        auto _terrainPrimitives = m_terrainGeneratorPtr->getPrimitives();
        for ( auto _primitive : _terrainPrimitives )
        {
            TGLTerrainDrawable _terrainDrawable;

            _terrainDrawable.size       = _primitive->size;
            _terrainDrawable.size0      = _primitive->size;
            _terrainDrawable.primitive  = _primitive;
            // @todo: move enumerators from components to tysoc-common (or similar)
            _terrainDrawable.renderable = tysoc::createRenderable( toEnumShape( _primitive->geomType ),
                                                                   _primitive->size,
                                                                   _primitive->filename ) ;
            m_scenePtr->addRenderable( std::unique_ptr< engine::CIRenderable >( _terrainDrawable.renderable ) );

            if ( engine::CTextureManager::HasCachedTexture( _primitive->texturename ) )
                tysoc::setRenderableTexture( _terrainDrawable.renderable, 
                                             engine::CTextureManager::GetCachedTexture( _primitive->texturename ) );

            m_terrainDrawables.push_back( _terrainDrawable );
        }
    }

    void TGLVizTerrainGenerator::_updateTerrainDrawable( TGLTerrainDrawable terrainDrawable )
    {
        auto _renderable = terrainDrawable.renderable;
        auto _primitive = terrainDrawable.primitive;
        auto _oldSize = terrainDrawable.size0;
        auto _newSize = terrainDrawable.size = terrainDrawable.primitive->size;

        // update renderable according to the current information hold by the primitive (@todo: do it just once)
        tysoc::resizeRenderable( _renderable, toEnumShape( _primitive->geomType ), _oldSize, _newSize );
        tysoc::setRenderableColor( _renderable, _primitive->color, _primitive->color, _primitive->color );

        // finally update the world-transform
        _renderable->position = fromTVec3( _primitive->pos );
        _renderable->rotation = fromTMat3( _primitive->rotmat );
    }

}