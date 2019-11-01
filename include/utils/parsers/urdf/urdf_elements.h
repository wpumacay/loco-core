
#pragma once

#include <tysoc_common.h>
#include <utils/parsers/parsers_xml.h>

namespace tysoc {
namespace urdf {

    /*************************************************************
    *   NATERIAL AND INERTIA INFORMATION
    *************************************************************/

    struct UrdfMaterial
    {
        static std::map< std::string, UrdfMaterial > MATERIALS;

        std::string     name;
        std::string     filename;
        TVec4           color;

        UrdfMaterial()
        {
            name        = "undefined";
            filename    = "";
            color       = TYSOC_DEFAULT_RGBA_COLOR;
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement );
    };

    struct UrdfInertia
    {
        TMat4 localTransform;
        float mass;
        float ixx; 
        float ixy;
        float ixz;
        float iyy;
        float iyz;
        float izz;

        UrdfInertia()
        {
            mass = 0.0f;
            ixx = 0.0f;
            iyy = 0.0f;
            izz = 0.0f;
            ixy = 0.0f;
            ixz = 0.0f;
            iyz = 0.0f;
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement,
                             const std::string& worldUp );
    };

    /*************************************************************
    *   GEOEMTRY TYPES INFORMATION
    *************************************************************/

    struct UrdfShape
    {
        std::string     type;
        TVec3           size;
        std::string     filename;
        TMat4           localTransform;

        UrdfShape()
        {
            type        = "sphere";
            size        = { 0.1f, 0.0f, 0.0f };
            filename    = "";
        }

        virtual void collectAttribs( tinyxml2::XMLElement* xmlElement, 
                                     const std::string& worldUp );
    };

    struct UrdfVisual : UrdfShape
    {
        std::string     name;
        UrdfMaterial    material;

        UrdfVisual() : UrdfShape(), name( "undefined" ) {}

        void collectAttribs( tinyxml2::XMLElement* xmlElement, 
                             const std::string& worldUp ) override;
    };

    struct UrdfCollision : UrdfShape
    {
        std::string name;

        UrdfCollision() : UrdfShape(), name( "undefined" ) {}

        void collectAttribs( tinyxml2::XMLElement* xmlElement,
                             const std::string& worldUp ) override;
    };

    /**********************************************************************
    *   JOINT AND LINKS DATA HOLDERS
    **********************************************************************/

    struct UrdfJoint;

    struct UrdfLink
    {
        std::string                         name;
        UrdfInertia                         inertia;
        std::vector< UrdfVisual >           visuals;
        std::vector< UrdfCollision >        collisions;
        UrdfLink*                           parentLink;
        UrdfJoint*                          parentJoint;
        std::vector< UrdfJoint* >           joints;
        std::vector< UrdfLink* >            children;

        UrdfLink()
        {
            parentLink      = nullptr;
            parentJoint     = nullptr;
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement, 
                             const std::string& worldUp );
    };

    struct UrdfJoint
    {
        std::string     name;
        std::string     type;
        std::string     parentLinkName;
        std::string     childLinkName;
        TMat4           parentLinkToJointTransform;
        TVec3           localJointAxis;
        TScalar         lowerLimit;
        TScalar         upperLimit;
        TScalar         effortLimit;
        TScalar         velocityLimit;
        TScalar         jointDamping;
        TScalar         jointFriction;

        UrdfJoint()
        {
            lowerLimit    = 1.0;
            upperLimit    = -1.0;
            effortLimit   = 0;
            velocityLimit = 0;
            jointDamping  = 0;
            jointFriction = 0;
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement,
                             const std::string& worldUp );
    };


    /**********************************************************************
    *   URDF MODEL DATA STRUCTURE
    **********************************************************************/

    struct UrdfModel
    {
        std::string                             name;
        std::map< std::string, UrdfMaterial>    materials;
        std::map< std::string, UrdfLink*>       links;
        std::map< std::string, UrdfJoint*>      joints;
        std::vector< UrdfLink* >                rootLinks;

        TMat4 zeroCompensation;

        std::vector< std::pair< std::string, std::string > > exclusionPairs;

        UrdfModel(){}
        ~UrdfModel(){}// @TODO: Check what to release
    };

}}