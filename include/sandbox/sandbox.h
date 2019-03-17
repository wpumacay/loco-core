
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
        TMat4 worldTransform;

        TScalar mass;
        TMat3   inertia;
        TVec3   friction;

        std::vector< TJoint* > joints;
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
    };




}}