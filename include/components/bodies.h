#pragma once

#include <tysoc_common.h>

#include <components/data.h>
#include <components/collisions.h>
#include <components/visuals.h>

namespace tysoc {

    class TCollision;
    class TVisual;

    class TBody
    {
        public :
        
        TBody();

        virtual ~TBody();

        void build();

        void update();

        void reset();

        void addCollision( TCollision* collision );

        void addVisual( TVisual* visual );

        void setPosition( const TVec3& position );

        void setOrientation( const TMat3& orientation );

        TVec3 position() { return m_position; }

        TMat3 orientation() { return m_orientation; }

        TMat4 worldTransform() { return m_worldTransform; }

        std::vector< TCollision* > collisions();

        std::vector< TVisual* > visuals();

        protected :

        TVec3 m_position;
        TMat3 m_orientation;
        TMat4 m_worldTransform;

        std::vector< TCollision* > m_collisions;
        std::vector< TVisual* > m_visuals;

        std::map< std::string, TCollision* > m_collisionsMap;
        std::map< std::string, TVisual* > m_visualsMap;
    };



}