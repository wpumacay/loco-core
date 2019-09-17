#pragma once

#include <glviz_common.h>

#include <viz/drawable.h>

#include <graphics/CMeshBuilder.h>
#include <LScene.h>

#define GLVIZ_DEFAULT_HFIELD_BASE 0.1f

namespace tysoc {

    engine::LIRenderable* createRenderable( const TShapeData& data );

    class TGLDrawable : public TIDrawable
    {

    public :

        TGLDrawable( engine::LIRenderable* renderablePtr,
                     const TShapeData& shapeData );

        ~TGLDrawable();

        void setWorldTransform( const TMat4& transform ) override;

        void setColor( const TVec3& fullColor ) override;

        void setAmbientColor( const TVec3& ambientColor ) override;

        void setDiffuseColor( const TVec3& diffuseColor ) override;

        void setSpecularColor( const TVec3& specularColor ) override;

        void setShininess( const TScalar& shininess ) override;

        void changeSize( const tysoc::TVec3& newSize ) override;

        void changeElevationData( const std::vector< float >& heightData ) override;

        void show( bool visible ) override;

        void wireframe( bool wireframe ) override;

        bool isVisible() override;

        bool isWireframe() override;

        void recycle() override;

        void reuse( const TShapeData& data );

        engine::LIRenderable* renderable() { return m_renderablePtr; }

        static TGLDrawable* CreateDrawable( const TShapeData& data );

        static std::map< eShapeType, std::queue< TGLDrawable* > > m_shapesPool;
        static std::map< eShapeType, int > m_numShapes;

    private :

        engine::LIRenderable* m_renderablePtr;

        /* construction data of this drawable object */
        TShapeData m_data;
    };

    extern "C" TIDrawable* visualizer_createDrawable( const TShapeData& data );

}