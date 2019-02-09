
#pragma once

#include <tysoc_common.h>
#include <utils/parsers/parsers_xml.h>

namespace tysoc {
namespace urdf {

    // @CHECK|@REFACTOR: Check if needs to create data in heap, or could just ...
    //                   create the data into the stack, by replacing pointers ...
    //                   by the structs itself (like the urdfgeometry. It could ...
    //                   just be a plain variable and not a pointer)

    /*************************************************************
    *   NATERIAL AND INERTIA INFORMATION
    *************************************************************/

    struct UrdfMaterial
    {
        static std::map< std::string, UrdfMaterial* > MATERIALS;

        std::string     name;
        std::string     filename;
        TVec4           color;

        UrdfMaterial()
        {
            name        = "undefined";
            filename    = "";
            color       = TVec4( 0.8, 0.8, 0.8, 1.0 );
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement );
    };

    struct UrdfInertia
    {
        TMat4       localTransform;
        double      mass;
        double      ixx; 
        double      ixy;
        double      ixz;
        double      iyy;
        double      iyz;
        double      izz;

        UrdfInertia()
        {
            mass    = 1.0f;
            ixx     = 1.0f;
            iyy     = 1.0f;
            izz     = 1.0f;
            ixy     = 0.0f;
            ixz     = 0.0f;
            iyz     = 0.0f;
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement );
    };

    /*************************************************************
    *   GEOEMTRY TYPES INFORMATION
    *************************************************************/

    struct UrdfGeometry
    {
        std::string     type;
        std::string     filename;
        TVec3           size;

        UrdfGeometry()        
        {
            type        = "undefined";
            filename    = "";
            size        = TVec3( 0, 0, 0 );
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement );
    };

    struct UrdfShape
    {
        TMat4           localTransform;
        UrdfGeometry*   geometry;

        UrdfShape()
        {
            geometry = new UrdfGeometry();
        }

        virtual void collectAttribs( tinyxml2::XMLElement* xmlElement );
    };

    struct UrdfVisual : UrdfShape
    {
        std::string     name;
        UrdfMaterial*   material;

        UrdfVisual() : UrdfShape()
        {
            name        = "undefined";
            material    = new UrdfMaterial();
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement ) override;
    };

    struct UrdfCollision : UrdfShape
    {
        std::string name;

        UrdfCollision() : UrdfShape()
        {
            name = "undefined";
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement ) override;
    };

    /**********************************************************************
    *   JOINT AND LINKS DATA HOLDERS
    **********************************************************************/

    struct UrdfJoint;

    struct UrdfLink
    {
        std::string                         name;
        UrdfInertia*                        inertia;
        std::vector< UrdfVisual* >          visuals;
        std::vector< UrdfCollision* >       collisions;
        UrdfLink*                           parentLink;
        UrdfJoint*                          parentJoint;
        std::vector< UrdfJoint* >           childJoints;
        std::vector< UrdfLink* >            childLinks;

        UrdfLink()
        {
            inertia         = NULL;
            parentLink      = NULL;
            parentJoint     = NULL;
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement );
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
            lowerLimit    = 0;
            upperLimit    = 0;
            effortLimit   = 0;
            velocityLimit = 0;
            jointDamping  = 0;
            jointFriction = 0;
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement );
    };


    /**********************************************************************
    *   URDF MODEL DATA STRUCTURE
    **********************************************************************/

    struct UrdfModel
    {
        std::string                             name;
        std::map< std::string, UrdfMaterial*>   materials;
        std::map< std::string, UrdfLink*>       links;
        std::map< std::string, UrdfJoint*>      joints;
        std::vector< UrdfLink* >                rootLinks;

        UrdfModel(){}
        ~UrdfModel(){}// @TODO: Check what to release
    };

}}