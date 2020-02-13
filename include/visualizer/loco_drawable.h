#pragma once

#include <loco_common.h>
#include <components/loco_data.h>

namespace loco
{
    class TCollision;
    class TVisual;

    class TIDrawable
    {
    public :

        TIDrawable() {}

        virtual ~TIDrawable() {}

        virtual void SetWorldTransform( const TMat4& transform ) = 0;

        virtual void SetColor( const TVec3& fullColor ) = 0;

        virtual void SetAmbientColor( const TVec3& ambientColor ) = 0;

        virtual void SetDiffuseColor( const TVec3& diffuseColor ) = 0;

        virtual void SetSpecularColor( const TVec3& specularColor ) = 0;

        virtual void SetShininess( const TScalar& shininess ) = 0;

        virtual void SetTexture( const std::string& textureName ) = 0;

        virtual void ChangeSize( const TVec3& newSize ) = 0;

        virtual void ChangeElevationData( const std::vector< float >& heightData ) = 0;

        virtual void SetVisible( bool visible ) = 0;

        virtual void SetWireframe( bool wireframe ) = 0;

        // @todo: required for backend to handle dynamic deletion (when detached from a TVisual)
        virtual void Detach() {}

        virtual bool visible() = 0;

        virtual bool wireframe() = 0;

    protected :

        // Current size of the drawable
        TVec3 m_size;

        // Starting size of the drawable (when created)
        TVec3 m_size0;

        // Scale-factor used to obtain the effect of changing the size
        TVec3 m_scale;

        // Type of shape that this drawable represents
        eShapeType  m_type;
    };

    /* dl-function to be loaded from the renderer support */
    typedef TIDrawable* FcnCreateVizDrawable( const TShapeData& data );
}