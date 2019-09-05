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
               const TBodyData& data );

        virtual ~TBody();

        void setAdapter( adapters::TIBodyAdapter* bodyImpl );

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

        std::vector< TVisual* > visuals() { return m_visuals; }

        std::vector< TCollision* > collisions() { return m_collisions; }

        TBodyData data() { return m_data; }

    protected :

        /* unique name identifier */
        std::string m_name;

        /* position in world space */
        TVec3 m_pos;
        /* orientation in world space */
        TMat3 m_rot; 
        /* transform in world space */
        TMat4 m_tf;

        /* data of this body object */
        TBodyData m_data;

        /* Adapter-object that gives access to the low-level API for a specific backend */
        adapters::TIBodyAdapter* m_bodyImpl;

        /* Vector-containers to store visuals and collisions owned by this body */
        std::vector< TVisual* >     m_visuals;
        std::vector< TCollision* >  m_collisions;

        /* Dictionary-containers to store visuals and collisions owned by this body */
        std::map< std::string, TVisual* >       m_visualsMap;
        std::map< std::string, TCollision* >    m_collisionsMap;
    };



}