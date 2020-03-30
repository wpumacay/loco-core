#pragma once

#include <loco_data.h>
#include <visualizer/loco_visualizer_drawable_adapter.h>

namespace loco
{
    class TDrawable : public TObject
    {
    public :

        TDrawable( const std::string& name,
                   const TVisualData& visual_data );

        ~TDrawable();

        void SetParentObject( TObject* parent_ref );

        void SetDrawableAdapter( TIDrawableAdapter* drawable_adapter_ref );

        void SetVisible( bool visible );

        void SetWireframe( bool wireframe );

        void SetLocalPosition( const TVec3& local_pos );

        void SetLocalRotation( const TMat3& local_rot );

        void SetLocalEuler( const TVec3& local_euler );

        void SetLocalQuat( const TVec4& local_quat );

        void SetLocalTransform( const TMat4& transform );

        void ChangeSize( const TVec3& new_size );

        void ChangeElevationData( const std::vector< float >& heights );

        void ChangeColor( const TVec3& new_full_color );

        void ChangeAmbientColor( const TVec3& new_ambient_color );

        void ChangeDiffuseColor( const TVec3& new_diffuse_color );

        void ChangeSpecularColor( const TVec3& new_specular_color );

        void ChangeShininess( const TScalar& shininess );

        void ChangeTexture( const std::string& texture );

        TVec3 local_pos() const { return TVec3( m_LocalTf.col( 3 ) ); }

        TMat3 local_rot() const { return TMat3( m_LocalTf ); }

        TVec3 local_euler() const { return tinymath::euler( m_LocalTf ); }

        TVec4 local_quat() const { return tinymath::quaternion( m_LocalTf ); }

        TMat4 local_tf() const { return m_LocalTf; }

        eShapeType shape() const { return m_Data.type; }

        TVec3 size() const { return m_Data.size; }

        TVec3 ambient() const { return m_Data.ambient; }

        TVec3 diffuse() const { return m_Data.diffuse; }

        TVec3 specular() const { return m_Data.specular; }

        TScalar shininess() const { return m_Data.shininess; }

        bool visible() const { return m_Visible; }

        bool wireframe() const { return m_Wireframe; }

        static eObjectType GetStaticType() { return eObjectType::DRAWABLE; }

        TVisualData& data() { return m_Data; }

        const TVisualData& data() const { return m_Data; }

        TObject* parent() { return m_Parent; }

        const TObject* parent() const { return m_Parent; }

        TIDrawableAdapter* adapter() { return m_DrawableAdapter; }

        const TIDrawableAdapter* adapter() const { return m_DrawableAdapter; }

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;

        void _DetachSimInternal() override;

        void _DetachVizInternal() override;

        void _SetTransformInternal( const TMat4& transform ) override;

        void _UpdateTransform();

        eObjectType _GetObjectTypeInternal() const override { return eObjectType::DRAWABLE; }

    protected :

        // Relative transform w.r.t. parent body
        TMat4 m_LocalTf;
        // Reference to the parent-body of this drawable object
        TObject* m_Parent;
        // Construction data of this drawable object
        TVisualData m_Data;
        // Reference to the drawable resource used for visualization
        TIDrawableAdapter* m_DrawableAdapter;
        // Flag that indicates drawable's visibility
        bool m_Visible;
        // Flag that indicates drawable's wireframe-mode
        bool m_Wireframe;
    };
}