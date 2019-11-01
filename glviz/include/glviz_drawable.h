#pragma once

#include <glviz_common.h>
#include <viz/drawable.h>
#include <graphics/CMeshBuilder.h>

namespace tysoc {

    class TGLDrawable : public TIDrawable
    {

    public :

        TGLDrawable( const TShapeData& shapeData );

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

        engine::CIRenderable* renderable() { return m_renderablePtr; }

    private :

        /* reference to the created renderable (not owned, scene owns it) */
        engine::CIRenderable* m_renderablePtr;

        /* construction data of this drawable object */
        TShapeData m_data;
    };

    extern "C" TIDrawable* visualizer_createDrawable( const TShapeData& data );

}