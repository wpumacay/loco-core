#pragma once

#include <loco_data.h>

namespace loco
{
    class TDrawable;

    class TIDrawableAdapter
    {
    public :

        TIDrawableAdapter( TObject* owner_ref )
            : m_OwnerRef( owner_ref ) {}

        virtual ~TIDrawableAdapter() { m_OwnerRef = nullptr; };

        virtual void SetTransform( const TMat4& transform ) = 0;

        virtual void SetColor( const TVec3& full_color ) = 0;

        virtual void SetAmbientColor( const TVec3& ambient_color ) = 0;

        virtual void SetDiffuseColor( const TVec3& diffuse_color ) = 0;

        virtual void SetSpecularColor( const TVec3& specular_color ) = 0;

        virtual void SetShininess( float shininess ) = 0;

        virtual void SetTexture( const std::string& texture ) = 0;

        virtual void ChangeSize( const TVec3& new_size ) = 0;

        virtual void ChangeElevationData( const std::vector< float >& heights ) = 0;

        virtual void SetVisible( bool visible ) = 0;

        virtual void SetWireframe( bool wireframe ) = 0;

        virtual void OnDetach() = 0;

        TObject* drawable() { return m_OwnerRef; }

        const TObject* drawable() const { return m_OwnerRef; }

        bool IsAwaitingDeletion() const { return m_AwaitingDeletion; }

    protected :

        // Current size of the drawable
        TVec3 m_Size;
        // Starting size of the drawable (when created)
        TVec3 m_Size0;
        // Scale-factor used to obtain the effect of changing the size
        TVec3 m_Scale;
        // Reference to the drawable owner
        TObject* m_OwnerRef;
        // Whether or not the adapter is ready to be freed
        bool m_AwaitingDeletion;
    };
}