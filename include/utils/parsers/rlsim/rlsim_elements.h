
#pragma once

#include <loco_common.h>
#include <utils/parsers/parsers_json.h>

namespace tysoc {
namespace rlsim {

    struct RlsimVisual
    {
        std::string     name;
        std::string     shape;
        TVec3           size;
        TVec3           localPos;
        TVec3           localEuler;
        TVec4           rgba;
        int             parentJointId;

        RlsimVisual() 
        {
            name    = "undefined";
            shape   = "sphere";
            size    = { 0.1f, 0.0f, 0.0f };
            rgba    = tysoc::DEFAULT_RGBA_COLOR;

            parentJointId = -1;
        }

        void collectAttribs( const nlohmann::json& jsonElement, const std::string& worldUp );
    };

    struct RlsimBody
    {
        std::string     name;
        std::string     shape;
        TScalar         mass;
        TVec3           size;
        TVec3           localPos;
        TVec3           localEuler;
        TVec4           rgba;
        int             collGroup;
        int             parentJointId;

        RlsimBody()
        {
            name    = "undefined";
            shape   = "sphere";
            mass    = 0.0f;
            size    = { 0.1f, 0.0f, 0.0f };
            rgba    = tysoc::DEFAULT_RGBA_COLOR;

            collGroup = -1;
            parentJointId = -1;
        }

        void collectAttribs( const nlohmann::json& jsonElement, const std::string& worldUp );
    };

    struct RlsimJoint
    {
        std::string                 name;
        std::string                 type;
        TVec3                       axis;
        int                         parentJointId;
        TVec3                       localPos;
        TVec3                       localEuler;
        std::vector< TVec2 >        limitsPerDof;
        TScalar                     torqueLimit;
        std::vector< RlsimJoint* >  childJoints;
        std::vector< RlsimBody* >   childBodies;
        std::vector< RlsimVisual >  childVisuals;

        RlsimJoint()
        {
            name = "undefined";
            type = "revolute";
            axis = { 1.0f, 0.0f, 0.0f };

            parentJointId = -1;
        }

        void collectAttribs( const nlohmann::json& jsonElement, const std::string& worldUp );
    };


    struct RlsimModel
    {
        std::string                     name;
        std::vector< RlsimVisual >      visuals;
        std::vector< RlsimJoint* >      joints;
        std::vector< RlsimBody* >       bodies;
        RlsimJoint*                     rootJoint;
        std::string                     worldUp;
        TScalar                         worldScale;

        RlsimModel() 
        {
            name        = "undefined";
            rootJoint   = nullptr;
            worldUp     = "y";
            worldScale  = 1.0;
        }

        ~RlsimModel()
        {
            for ( auto _joint : joints )
                delete _joint;
            for ( auto _body : bodies )
                delete _body;

            rootJoint = nullptr;
            joints.clear();
            bodies.clear();
            visuals.clear();
        }
    };

}}