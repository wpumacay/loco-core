
#pragma once

#include <tysoc_common.h>

namespace tysoc {
namespace urdf {

    /*************************************************************
    *   NATERIAL AND INERTIA INFORMATION
    *************************************************************/

    struct UrdfMaterial
    {
        std::string     name;
        std::string     textureFilename;
        TVec4           rgbaColor;
        TVec3           specularColor;

        UrdfMaterial()
        {
            rgbaColor     = TVec4( 0.8, 0.8, 0.8, 1.0 );
            specularColor = TVec3( 0.4, 0.4, 0.4 );
        }

        void collectAttribs( tinyxml2::XMLElement* xmlElement );
    };

    struct UrdfInertia
    {
        TMat4       linkLocalFrame;
        bool        hasLinkLocalFrame;
        double      mass;
        double      ixx; 
        double      ixy;
        double      ixz;
        double      iyy;
        double      iyz;
        double      izz;

        UrdfInertia()
        {
            hasLinkLocalFrame   = false;
            mass                = 0.f;
            ixx                 = 0.0f;
            ixy                 = 0.0f;
            ixz                 = 0.0f;
            iyy                 = 0.0f;
            iyz                 = 0.0f;
            izz                 = 0.0f;
        }
    };

    /*************************************************************
    *   GEOEMTRY TYPES INFORMATION
    *************************************************************/

    struct UrdfGeometry
    {
        std::string type;
        double      sphereRadius;
        TVec3       boxSize;
        double      capsuleRadius;
        double      capsuleHeight;
        bool        hasFromTo;
        TVec3       capsuleFrom;
        TVec3       capsuleTo;

        TVec3 planeNormal;

        enum
        {
            FILE_STL = 1,
            FILE_COLLADA = 2,
            FILE_OBJ = 3,
            FILE_CDF = 4,
            MEMORY_VERTICES = 5,

        };
        int meshFileType;
        std::string meshFileName;
        TVec3 meshScale;

        UrdfMaterial localMaterial;
        bool hasLocalMaterial;

        UrdfGeometry()        
        {
            type              = "undefined";
            sphereRadius      = 1;
            boxSize           = TVec3( 1, 1, 1 );
            capsuleRadius     = 1;
            capsuleHeight     = 1;
            hasFromTo         = false;
            capsuleFrom       = TVec3( 0, 1, 0 );
            capsuleTo         = TVec3( 1, 0, 0 );
            planeNormal       = TVec3( 0, 0, 1 );
            meshFileType      = 0;
            meshScale         = TVec3( 1, 1, 1 );
            hasLocalMaterial  = false;
        }
    };

    struct UrdfShape
    {
        std::string     name;
        TMat4           linkLocalFrame;
        UrdfGeometry    geometry;
    };

    struct UrdfVisual : UrdfShape
    {
        std::string materialName;
    };

    struct UrdfCollision : UrdfShape
    {
        int flags;
        int collisionGroup;
        int collisionMask;

        UrdfCollision()
        {
            flags = 0;
        }
    };

    /*************************************************************
    *   JOINT TYPES INFORMATION
    *************************************************************/
    enum UrdfJointTypes
    {
        URDFRevoluteJoint = 1,
        URDFPrismaticJoint,
        URDFContinuousJoint,
        URDFFloatingJoint,
        URDFPlanarJoint,
        URDFFixedJoint,
        URDFSphericalJoint,

    };

    enum URDF_LinkContactFlags
    {
        URDF_CONTACT_HAS_LATERAL_FRICTION = 1,
        URDF_CONTACT_HAS_INERTIA_SCALING = 2,
        URDF_CONTACT_HAS_CONTACT_CFM = 4,
        URDF_CONTACT_HAS_CONTACT_ERP = 8,
        URDF_CONTACT_HAS_STIFFNESS_DAMPING = 16,
        URDF_CONTACT_HAS_ROLLING_FRICTION = 32,
        URDF_CONTACT_HAS_SPINNING_FRICTION = 64,
        URDF_CONTACT_HAS_RESTITUTION = 128,
        URDF_CONTACT_HAS_FRICTION_ANCHOR = 256,

    };

    enum UrdfCollisionFlags
    {
        URDF_FORCE_CONCAVE_TRIMESH = 1,
        URDF_HAS_COLLISION_GROUP = 2,
        URDF_HAS_COLLISION_MASK = 4,
    };

    struct URDFLinkContactInfo
    {
        TScalar lateralFriction;
        TScalar rollingFriction;
        TScalar spinningFriction;
        TScalar restitution;
        TScalar inertiaScaling;
        TScalar contactCfm;
        TScalar contactErp;
        TScalar contactStiffness;
        TScalar contactDamping;

        int flags;

        URDFLinkContactInfo()
        {
            lateralFriction   = 0.5;
            rollingFriction   = 0;
            spinningFriction  = 0;
            restitution       = 0;
            inertiaScaling    = 1;
            contactCfm        = 0;
            contactErp        = 0;
            contactStiffness  = 1e4;
            contactDamping    = 1;

            flags = URDF_CONTACT_HAS_LATERAL_FRICTION;
        }
    };

    /**********************************************************************
    *   JOINT AND LINKS DATA HOLDERS
    **********************************************************************/

    struct UrdfJoint;

    struct UrdfLink
    {
        std::string                         name;
        UrdfInertia                         inertia;
        TMat4                               linkTransformInWorld;
        std::vector<UrdfVisual>             visualArray;
        std::vector<UrdfCollision>          collisionArray;
        UrdfLink*                           parentLink;
        UrdfJoint*                          parentJoint;
        std::vector<UrdfJoint*>             childJoints;
        std::vector<UrdfLink*>              childLinks;
        int                                 linkIndex;
        URDFLinkContactInfo                 contactInfo;

        UrdfLink()
        {
            parentLink    = 0;
            parentJoint   = 0;
            linkIndex     = -2;
        }
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
            upperLimit    = -1;
            effortLimit   = 0;
            velocityLimit = 0;
            jointDamping  = 0;
            jointFriction = 0;
        }
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

        UrdfModel()
        {

        }

        ~UrdfModel()
        {
            for (int i = 0; i < materials.size(); i++)
            {
                UrdfMaterial** ptr = materials.getAtIndex(i);
                if (ptr)
                {
                    UrdfMaterial* t = *ptr;
                    delete t;
                }
            }
            for (int i = 0; i < links.size(); i++)
            {
                UrdfLink** ptr = links.getAtIndex(i);
                if (ptr)
                {
                    UrdfLink* t = *ptr;
                    delete t;
                }
            }
            for (int i = 0; i < joints.size(); i++)
            {
                UrdfJoint** ptr = joints.getAtIndex(i);
                if (ptr)
                {
                    UrdfJoint* t = *ptr;
                    delete t;
                }
            }
        }
    };

}}