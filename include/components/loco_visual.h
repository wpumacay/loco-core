#pragma once

#include <loco_common.h>

#include <components/loco_data.h>
#include <components/loco_body_base.h>
#include <viz/loco_drawable.h>

namespace loco
{
    class TIBody;

    class TVisual
    {
    public :

        TVisual( const std::string& name,
                 const TVisualData& visualData );

        ~TVisual();

        void SetParentBody( TIBody* parentBodyRef );

        void SetDrawable( TIDrawable* drawableImplRef );

        void SetVisible( bool visible );

        void SetWireframe( bool wireframe );

        void Initialize();

        void PreStep();

        void PostStep();

        void Reset();

        void SetLocalPosition( const TVec3& localPosition );

        void SetLocalRotation( const TMat3& localRotation );

        void SetLocalEuler( const TVec3& localEuler );

        void SetLocalQuat( const TVec4& localQuaternion );

        void SetLocalTransform( const TMat4& transform );

        void ChangeSize( const TVec3& newSize );

        void ChangeElevationData( const std::vector< float >& heightData );

        void ChangeColor( const TVec3& newFullColor );

        void ChangeAmbientColor( const TVec3& newAmbientColor );

        void ChangeDiffuseColor( const TVec3& newDiffuseColor );

        void ChangeSpecularColor( const TVec3& newSpecularColor );

        void ChangeShininess( const TScalar& shininess );

        std::string name() const { return m_name; }

        TVec3 pos() const { return TVec3( m_tf.col( 3 ) ); }

        TMat3 rot() const { return TMat3( m_tf ); }

        TVec3 euler() const { return tinymath::euler( m_tf ); }

        TVec4 quat() const { return tinymath::quaternion( m_tf ); }

        TMat4 tf() const { return m_tf; }

        TVec3 localPos() const { return TVec3( m_localTf.col( 3 ) ); }

        TMat3 localRot() const { return TMat3( m_localTf ); }

        TVec3 localEuler() const { return tinymath::euler( m_localTf ); }

        TVec4 localQuat() const { return tinymath::quaternion( m_localTf ); }

        TMat4 localTf() const { return m_localTf; }

        eShapeType shape() const { return m_data.type; }

        TVec3 size() const { return m_data.size; }

        TVec3 ambient() const { return m_data.ambient; }

        TVec3 diffuse() const { return m_data.diffuse; }

        TVec3 specular() const { return m_data.specular; }

        TScalar shininess() const { return m_data.shininess; }

        bool visible() const { return m_visible; }

        bool wireframe() const { return m_wireframe; }

        TVisualData data() const { return m_data; }

        TVisualData& dataRef() { return m_data; }

        const TVisualData& dataRef() const { return m_data; }

        TIBody* parent() { return m_parentBodyRef; }

        const TIBody* parent() const { return m_parentBodyRef; }

        TIDrawable* adapter() { return m_drawableImplRef; }

    protected :

        // Unique name identifier
        std::string m_name;
        // Transform in world-space
        TMat4 m_tf;
        // Relative transform w.r.t. parent body
        TMat4 m_localTf;
        // Reference to the parent-body of this collision object
        TIBody* m_parentBodyRef;
        // Construction data of this visual object
        TVisualData m_data;
        // Reference to the drawable resource used for visualization
        TIDrawable* m_drawableImplRef;
        // Flag that indicates visual's drawable visibility
        bool m_visible;
        // Flag that indicates visual's drawable wireframe-mode
        bool m_wireframe;
    };
}