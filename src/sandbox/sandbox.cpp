
#include <sandbox/sandbox.h>

namespace tysoc {
namespace sandbox {

    TBody::TBody()
    {
        name = "undefined";
        type = "box";
        filename = "";

        size = { 0.1, 0.1, 0.1 };
        color = { 0.25f, 1.0f, 0.25f };

        mass = 0.0f;
        friction = { 1.0, 0.005, 0.0001 };

        parentBodyPtr = NULL;

        vel = { 0, 0, 0 };
    }

    TBody::~TBody()
    {
        parentBodyPtr = NULL;

        for ( size_t q = 0; q < joints.size(); q++ )
        {
            delete joints[q];
            joints[q] = NULL;
        }
        joints.clear();

        for ( size_t q = 0; q < bodies.size(); q++ )
        {
            delete bodies[q];
            bodies[q] = NULL;
        }
        bodies.clear();
    }

    TJoint::TJoint()
    {
        name = "undefined";
        type = "hinge";

        axis = { 1, 0, 0 };

        parentBodyPtr = NULL;
    }

    TJoint::~TJoint()
    {
        parentBodyPtr = NULL;
    }

    TFreeBody::TFreeBody()
        : TBody()
    {
        auto _freejoint = new TJoint();
        _freejoint->name = "free";
        _freejoint->type = "free";
        _freejoint->parentBodyPtr = this;

        joints.push_back( _freejoint );
    }

}}