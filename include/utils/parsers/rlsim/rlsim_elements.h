
#pragma once

#include <tysoc_common.h>
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
            parentJointId = -1;
        }

        void collectAttribs( const nlohmann::json& jsonElement );
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
            collGroup   = -1;
            parentJointId = -1;
        }

        void collectAttribs( const nlohmann::json& jsonElement );
    };

    struct RlsimJoint
    {
        std::string                 name;
        std::string                 type;
        int                         parentJointId;
        TVec3                       localPos;
        TVec3                       localEuler;
        std::vector< TVec2 >        limitsPerDof;
        TScalar                     torqueLimit;
        std::vector< RlsimJoint* >  childJoints;
        std::vector< RlsimBody* >   childBodies;
        std::vector< RlsimVisual* > childVisuals;

        RlsimJoint()
        {
            parentJointId = -1;
        }

        void collectAttribs( const nlohmann::json& jsonElement );
    };


    struct RlsimModel
    {
        std::string                             name;
        std::vector< RlsimVisual* >             visuals;
        std::vector< RlsimJoint* >              joints;
        std::vector< RlsimBody* >               bodies;
        RlsimJoint*                             rootJoint;

        RlsimModel() 
        {
            rootJoint = NULL;
        }

        ~RlsimModel() {} // @TODO: Check what to release
    };

}}