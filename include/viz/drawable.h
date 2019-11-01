#pragma once

#include <tysoc_common.h>
#include <components/data.h>

namespace tysoc {

    class TCollision;
    class TVisual;

    class TIDrawable
    {

    public :

        TIDrawable() {}

        virtual ~TIDrawable() {}

        virtual void setWorldTransform( const TMat4& transform ) = 0;

        virtual void setColor( const TVec3& fullColor ) = 0;

        virtual void setAmbientColor( const TVec3& ambientColor ) = 0;

        virtual void setDiffuseColor( const TVec3& diffuseColor ) = 0;

        virtual void setSpecularColor( const TVec3& specularColor ) = 0;

        virtual void setShininess( const TScalar& shininess ) = 0;

        virtual void changeSize( const TVec3& newSize ) = 0;

        virtual void changeElevationData( const std::vector< float >& heightData ) = 0;

        virtual void show( bool visible ) = 0;

        virtual void wireframe( bool wireframe ) = 0;

        virtual bool isVisible() = 0;

        virtual bool isWireframe() = 0;

    protected :

        /* current size of the drawable */
        TVec3 m_size;

        /* starting size of the drawable (when created) */
        TVec3 m_size0;

        /* scale-factor used to obtain the effect of changing the size */
        TVec3 m_scale;

        /* type of shape that this drawable represents */
        eShapeType  m_type;

    };

    /* dl-function to be loaded from the renderer support */
    typedef TIDrawable* FcnCreateVizDrawable( const TShapeData& data );

}