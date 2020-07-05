
#include <glviz_common.h>

namespace tysoc {

    TMat4 toTMat4( const engine::CMat4& mat )
    {
        TMat4 _res;

        for ( size_t i = 0; i < 4; i++ )
            for ( size_t j = 0; j < 4; j++ )
                _res.buff[i + 4 * j] = mat.buff[i + 4 * j];

        return _res;
    }

    engine::CMat4 fromTMat4( const TMat4& mat )
    {
        engine::CMat4 _res;

        for ( size_t i = 0; i < 4; i++ )
            for ( size_t j = 0; j < 4; j++ )
                _res.buff[i + 4 * j] = mat.buff[i + 4 * j];

        return _res;
    }

    engine::CMat4 fromTMat3( const TMat3& mat )
    {
        engine::CMat4 _res;

        _res.buff[0] = mat.buff[0];
        _res.buff[1] = mat.buff[1];
        _res.buff[2] = mat.buff[2];

        _res.buff[4] = mat.buff[3];
        _res.buff[5] = mat.buff[4];
        _res.buff[6] = mat.buff[5];

        _res.buff[8]  = mat.buff[6];
        _res.buff[9]  = mat.buff[7];
        _res.buff[10] = mat.buff[8];

        return _res;
    }

    engine::CVec3 fromTVec3( const TVec3& vec )
    {
        return { vec.x, vec.y, vec.z };
    }

    engine::CVec4 fromTVec4( const TVec4& vec )
    {
        return { vec.x, vec.y, vec.z, vec.w };
    }

    engine::CIRenderable* createRenderable( const eShapeType& type,
                                            const TVec3& size,
                                            const std::string& filename )
    {
        engine::CIRenderable* _renderable = nullptr;

        if ( type == eShapeType::PLANE )
        {
            _renderable = engine::CMeshBuilder::createPlane( size.x, size.y );
        }
        else if ( type == eShapeType::BOX )
        {
            _renderable = engine::CMeshBuilder::createBox( size.x, size.y, size.z );
        }
        else if ( type == eShapeType::SPHERE )
        {
            _renderable = engine::CMeshBuilder::createSphere( size.x );
        }
        else if ( type == eShapeType::CAPSULE )
        {
            _renderable = engine::CMeshBuilder::createCapsule( size.x, size.y );
        }
        else if ( type == eShapeType::CYLINDER )
        {
            _renderable = engine::CMeshBuilder::createCylinder( size.x, size.y );
        }
        else if ( type == eShapeType::CONVEX_MESH )
        {
            _renderable = engine::CMeshBuilder::createModelFromFile( filename );
            _renderable->scale = { size.x, size.y, size.z };
        }

        if ( !_renderable )
            ENGINE_WARN( "Could not create renderable of type: {0}", tysoc::toString( type ) );

        return _renderable;
    }

    engine::CIRenderable* createRenderable( const TShapeData& data )
    {
        engine::CIRenderable* _renderable = nullptr;

        if ( data.type == eShapeType::PLANE )
        {
            _renderable = engine::CMeshBuilder::createPlane( data.size.x, data.size.y );
        }
        else if ( data.type == eShapeType::BOX )
        {
            _renderable = engine::CMeshBuilder::createBox( data.size.x,
                                                           data.size.y,
                                                           data.size.z );
        }
        else if ( data.type == eShapeType::SPHERE )
        {
            _renderable = engine::CMeshBuilder::createSphere( data.size.x );
        }
        else if ( data.type == eShapeType::CYLINDER )
        {
            _renderable = engine::CMeshBuilder::createCylinder( data.size.x,
                                                                   data.size.y );
        }
        else if ( data.type == eShapeType::CAPSULE )
        {
            _renderable = engine::CMeshBuilder::createCapsule( data.size.x,
                                                                  data.size.y );
        }
        else if ( data.type == eShapeType::CONVEX_MESH )
        {
            _renderable = engine::CMeshBuilder::createModelFromFile( data.filename );
            if ( _renderable )
                _renderable->scale = { data.size.x, data.size.y, data.size.z };
        }
        else if ( data.type == eShapeType::HEIGHTFIELD )
        {
            auto _heightDataScaled = data.hdata.heightData;
            for ( size_t i = 0; i < _heightDataScaled.size(); i++ )
                _heightDataScaled[i] *= data.size.z;

            _renderable = engine::CMeshBuilder::createHeightField( data.hdata.nWidthSamples,
                                                                      data.hdata.nDepthSamples,
                                                                      data.size.x,
                                                                      data.size.y,
                                                                      data.size.x / 2.0f, 
                                                                      data.size.y / 2.0f,
                                                                      _heightDataScaled,
                                                                      GLVIZ_DEFAULT_HFIELD_BASE );
        }

        if ( !_renderable )
            ENGINE_WARN( "Could not create renderable of type: {0}", tysoc::toString( data.type ) );

        return _renderable;
    }

    void setRenderableColor( engine::CIRenderable* renderable,
                             const TVec3& cAmbient,
                             const TVec3& cDiffuse,
                             const TVec3& cSpecular )
    {
        if ( renderable->type() == engine::eRenderableType::MESH )
        {
            renderable->material()->ambient   = { cAmbient.x, cAmbient.y, cAmbient.z };
            renderable->material()->diffuse   = { cDiffuse.x, cDiffuse.y, cDiffuse.z };
            renderable->material()->specular  = { cSpecular.x, cSpecular.y, cSpecular.z };
        }
        else if ( renderable->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( renderable )->meshes();
            for ( auto _submesh : _submeshes )
            {
                _submesh->material()->ambient   = { cAmbient.x, cAmbient.y, cAmbient.z };
                _submesh->material()->diffuse   = { cDiffuse.x, cDiffuse.y, cDiffuse.z };
                _submesh->material()->specular  = { cSpecular.x, cSpecular.y, cSpecular.z };
            }
        }
    }

    void setRenderableTexture( engine::CIRenderable* renderable,
                               engine::CTexture* albedoTex )
    {
        if ( !renderable )
            return;

        if ( renderable->type() == engine::eRenderableType::MESH )
        {
            renderable->material()->setAlbedoMap( albedoTex );
        }
        else if ( renderable->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( renderable )->meshes();
            for ( auto _submesh : _submeshes )
                _submesh->material()->setAlbedoMap( albedoTex );
        }
    }

    void resizeRenderable( engine::CIRenderable* renderable,
                           const eShapeType& type,
                           const TVec3& baseSize,
                           const TVec3& newSize )
    {
        if ( !renderable )
            return;

        if ( type == eShapeType::PLANE )
        {
            renderable->scale.x = newSize.x / baseSize.x;
            renderable->scale.y = newSize.y / baseSize.y;
        }
        else if ( type == eShapeType::BOX )
        {
            renderable->scale = { newSize.x / baseSize.x,
                                  newSize.y / baseSize.y,
                                  newSize.z / baseSize.z };
        }
        else if ( type == eShapeType::SPHERE )
        {
            // scale according to radius
            renderable->scale = { newSize.x / baseSize.x, 
                                  newSize.x / baseSize.x, 
                                  newSize.x / baseSize.x };
        }
        else if ( type == eShapeType::CYLINDER )
        {
            // scale according to radius
            renderable->scale.x = newSize.x / baseSize.x;
            renderable->scale.y = newSize.x / baseSize.x;
            // scale according to height
            renderable->scale.z = newSize.y / baseSize.y;
        }
        else if ( type == eShapeType::CAPSULE )
        {
            // scale according to radius
            renderable->scale.x = newSize.x / baseSize.x;
            renderable->scale.y = newSize.x / baseSize.x;
            // scale according to height
            renderable->scale.z = newSize.y / baseSize.y;
        }
        else if ( type == eShapeType::CONVEX_MESH )
        {
            // scale every dimension
            renderable->scale.x = newSize.x / baseSize.x;
            renderable->scale.y = newSize.y / baseSize.y;
            renderable->scale.z = newSize.z / baseSize.z;
        }
        else if ( type == eShapeType::HEIGHTFIELD )
        {
            LOCO_CORE_WARN( "Hfield shapes don't support changing the scale. Change the elevation data instead" );
        }
    }

}