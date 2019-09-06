#pragma once

#include <tysoc_common.h>

#include <components/data.h>
#include <components/collisions.h>
#include <components/visuals.h>

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

        void addCollision( TCollision* collision );

        void addVisual( TVisual* visual );

        void update();

        void reset();

        void setPosition( const TVec3& position );

        void setRotation( const TMat3& rotation );

        void setTransform( const TMat4& transform );

        std::string name() { return m_name; }

        TVec3 pos() { return m_pos; }

        TMat3 rot() { return m_rot; }

        TVec4 quat() { return m_tf.getRotQuaternion(); }

        TMat4 tf() { return m_tf; }

        TVec3 pos0() { return m_pos0; }

        TMat3 rot0() { return m_rot0; }

        TVec4 quat0() { return m_tf0.getRotQuaternion(); }

        TMat4 tf0() { return m_tf0; }

        std::vector< TVisual* > visuals() { return m_visuals; }

        std::vector< TCollision* > collisions() { return m_collisions; }

        eDynamicsType dyntype() { return m_data.dyntype; }

        TBodyData data() { return m_data; }

        TIBodyAdapter* adapter() { return m_bodyImpl; }

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

        /* Adapter-object that gives access to the low-level API for a specific backend */
        TIBodyAdapter* m_bodyImpl;

        /* Vector-containers to store visuals and collisions owned by this body */
        std::vector< TVisual* >     m_visuals;
        std::vector< TCollision* >  m_collisions;

        /* Dictionary-containers to store visuals and collisions owned by this body */
        std::map< std::string, TVisual* >       m_visualsMap;
        std::map< std::string, TCollision* >    m_collisionsMap;
    };

}