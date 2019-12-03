#pragma once

#include <tysoc_common.h>

#include <components/data.h>
#include <components/collision.h>
#include <components/visual.h>

#include <adapters/body_adapter.h>

namespace tysoc {

    class TCollision;
    class TVisual;
    class TIBodyAdapter;

    class TBody
    {

    public :
        
        TBody( const std::string& name,
               const TBodyData& data,
               const TVec3& position,
               const TMat3& rotation );

        virtual ~TBody();

        void setAdapter( TIBodyAdapter* bodyImpl );

        void setCollision( std::unique_ptr< TCollision > collisionObj );

        void setVisual( std::unique_ptr< TVisual > visualObj );

        void update();

        void reset();

        void setPosition( const TVec3& position );

        void setRotation( const TMat3& rotation );

        void setEuler( const TVec3& euler );

        void setQuaternion( const TVec4& quat );

        void setTransform( const TMat4& transform );

        std::string name() { return m_name; }

        TVec3 pos() { return m_pos; }

        TMat3 rot() { return m_rot; }

        TVec4 quat() { return m_tf.getRotQuaternion(); }

        TVec3 euler() { return m_tf.getRotEuler(); }

        TMat4 tf() { return m_tf; }

        TVec3 pos0() { return m_pos0; }

        TMat3 rot0() { return m_rot0; }

        TVec4 quat0() { return m_tf0.getRotQuaternion(); }

        TVec3 euler0() { return m_tf0.getRotEuler(); }

        TMat4 tf0() { return m_tf0; }

        TVisual* visual() { return m_visual.get(); }

        TCollision* collision() { return m_collision.get(); }

        eDynamicsType dyntype() { return m_data.dyntype; }

        TBodyData data() { return m_data; }

        TIBodyAdapter* adapter() { return m_bodyImplRef; }

    protected :

        /* unique name identifier */
        std::string m_name;

        /* position in world space */
        TVec3 m_pos;
        /* orientation in world space */
        TMat3 m_rot; 
        /* transform in world space */
        TMat4 m_tf;

        /* starting position of the body in world-space */
        TVec3 m_pos0;
        /* starting orientation of the body in world-space */
        TMat3 m_rot0;
        /* starting transform of the body in world-space */
        TMat4 m_tf0;

        /* data of this body object */
        TBodyData m_data;

        /* Adapter-object that gives access to the low-level API for a specific backend (reference only, as simulation owns it) */
        TIBodyAdapter* m_bodyImplRef;

        /* Single collider (collision object) of this body */
        std::unique_ptr< TCollision > m_collision;

        /* Single visual (viewer object) of this body */
        std::unique_ptr< TVisual > m_visual;
    };

}
