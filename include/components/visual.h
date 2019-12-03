#pragma once

#include <tysoc_common.h>

#include <components/data.h>
#include <components/body.h>

#include <viz/drawable.h>

namespace tysoc {

    class TBody;

    class TVisual
    {

        public :

        TVisual( const std::string& name,
                 const TVisualData& visualData );

        ~TVisual();

        void setParentBody( TBody* parentBodyPtr );

        void setDrawable( TIDrawable* drawablePtr );

        void show( bool visible );

        void wireframe( bool wireframe );

        void update();

        void reset();

        void setLocalPosition( const TVec3& localPosition );

        void setLocalRotation( const TMat3& rotation );

        void setLocalQuat( const TVec4& localQuaternion );

        void setLocalTransform( const TMat4& transform );

        void changeSize( const TVec3& newSize );

        void changeElevationData( const std::vector< float >& heightData );

        void changeColor( const TVec3& newFullColor );

        void changeAmbientColor( const TVec3& newAmbientColor );

        void changeDiffuseColor( const TVec3& newDiffuseColor );

        void changeSpecularColor( const TVec3& newSpecularColor );

        void changeShininess( const TScalar& shininess );

        std::string name() { return m_name; }

        bool isVisible();

        bool isWireframe();

        TVec3 pos() { return m_pos; }

        TMat3 rot() { return m_rot; }

        TVec3 euler() { return m_tf.getRotEuler(); }

        TVec4 quat() { return m_tf.getRotQuaternion(); }

        TMat4 tf() { return m_tf; }

        TVec3 localPos() { return m_localPos; }

        TMat3 localRot() { return m_localRot; }

        TVec3 localEuler() { return m_localTf.getRotEuler(); }

        TVec4 localQuat() { return m_localTf.getRotQuaternion(); }

        TMat4 localTf() { return m_localTf; }

        eShapeType shape() { return m_data.type; }

        TVec3 size() { return m_data.size; }

        TVisualData data() { return m_data; }

        protected :

        /* unique name identifier */
        std::string m_name;

        /* relative position w.r.t. parent body */
        TVec3 m_localPos;
        /* relative rotation w.r.t. parent body */
        TMat3 m_localRot;
        /* relative transform w.r.t. parent body */
        TMat4 m_localTf;

        /* position in world-space */
        TVec3 m_pos;
        /* rotation in world-space */
        TMat3 m_rot;
        /* transform in world-space */
        TMat4 m_tf;

        /* reference to the parent-body of this collision object */
        TBody* m_parentBodyPtr;

        /* construction data of this visual object */
        TVisualData m_data;

        /* reference to the drawable resource used for visualization */
        TIDrawable* m_drawableImplPtr;
    };

}