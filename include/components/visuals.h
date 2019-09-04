#pragma once

#include <tysoc_common.h>
#include <components/data.h>

namespace tysoc {

    class TVisual
    {

        public :

        TVisual( const std::string& name,
                 const TVisualData& shapeData );

        ~TVisual();

        void build();

        void update();

        void reset();

        std::string name() { return m_name; }

        TVec3 position() { return m_position; }

        TMat3 orientation() { return m_orientation; }

        TMat4 worldTransform() { return m_worldTransform; }

        TVisualData data() { return m_data; }

        protected :

        std::string m_name;

        TVec3 m_position;
        TMat3 m_orientation;
        TMat4 m_worldTransform;
        TMat4 m_localTransform;

        TVisualData m_data;
    };



}