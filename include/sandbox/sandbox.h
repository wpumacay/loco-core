
#pragma once

#include <tysoc_common.h>

namespace tysoc {
namespace sandbox {

    struct TJoint;

    struct TBody
    {

        std::string name;
        std::string type;
        std::string filename;

        TVec3 size;
        TMat4 relTransform;
        TMat4 worldTransform;
        TVec3 color;

        TScalar mass;
        TMat3   inertia;
        TVec3   friction;

        TBody* parentBodyPtr;
        std::vector< TJoint* > joints;
        std::vector< TBody* > bodies;

        TBody();
        ~TBody();
    };

    struct TJoint
    {
        std::string name;
        std::string type;

        TVec3 axis;
        TMat4 relTransform;
        TMat4 worldTransform;

        TVec2 limits;

        TBody* parentBodyPtr;

        TJoint();
        ~TJoint();
    };

    struct TFreeBody : public TBody
    {
        TFreeBody();
    };

}}