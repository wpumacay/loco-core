
#pragma once

#include <tysoc_common.h>
#include <sandbox/body.h>

namespace tysoc {
namespace sandbox {

    class TIBody;

    class TIJoint
    {

        protected :

        std::string m_name;
        std::string m_type;

        TVec3 m_relpos;
        TMat3 m_relrot;
        TVec3 m_axis;
        TMat4 m_worldTransform;

        size_t m_ndof;
        TVec2 m_limits;

        TIBody* m_parentBodyPtr;

        public :

        TIJoint( const std::string& name,
                 const std::string& type,
                 const TVec3& relpos,
                 const TMat3& relrot,
                 const TVec3& axis,
                 const TVec2& limits = { 0, 0 } );
        virtual ~TIJoint();

        TVec3 relpos();
        TVec3 relRotEuler();
        TVec4 relRotQuat();
        TMat3 relRotMat();
        TVec3 axis();

        TVec3 worldPosition();
        TMat3 worldRotMat();
        TVec3 worldRotEuler();
        TVec4 worldRotQuat();
        TVec3 worldAxis();
        TMat4 worldTransform();

        std::string name();
        std::string type();

        size_t numDof();
        TVec2 limits();

        TIBody* parentBody();

    };


}}