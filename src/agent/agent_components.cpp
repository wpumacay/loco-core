
#include <agent/agent_components.h>

namespace tysoc {
namespace agent {

    size_t NUM_UNKNOWN_COLLISIONS   = 0;
    size_t NUM_UNKNOWN_VISUALS      = 0;
    size_t NUM_UNKNOWN_JOINTS       = 0;
    size_t NUM_UNKNOWN_BODIES       = 0;

    TKinTreeMeshAsset::TKinTreeMeshAsset()
    {
        name    = "undefined";
        file    = "";
        scale   = { 1.0, 1.0, 1.0 };
    }

    //------------------------TKinTreeCollision-------------------------------//
    TMaterial::TMaterial()
    {
        name        = "";
        texture     = "";
        diffuse     = TYSOC_DEFAULT_DIFFUSE_COLOR;
        specular    = TYSOC_DEFAULT_SPECULAR_COLOR;
    }

    //------------------------TKinTreeCollision-------------------------------//
    TKinTreeCollision::TKinTreeCollision()
    {
        name            = "undefined";
        parentBodyPtr   = NULL;
        friction        = { 3, { 1., 0.005, 0.0001 } };
        density         = TYSOC_DEFAULT_DENSITY;
        contype         = 1;
        conaffinity     = 1;
        condim          = 3;
    }

    //------------------------TKinTreeVisual----------------------------------//
    TKinTreeVisual::TKinTreeVisual()
    {
        name            = "undefined";
        parentBodyPtr   = NULL;
    }

    //------------------------TKinTreeSensor----------------------------------//
    TKinTreeSensor::TKinTreeSensor()
    {
        name        = "undefined";
        bodyPtr     = NULL;
        jointPtr    = NULL;
    }

    //------------------------TKinTreeJointSensor-----------------------------//
    TKinTreeJointSensor::TKinTreeJointSensor()
        : TKinTreeSensor()
    {
        type        = "joint";
        jointName   = "";
        theta       = 0;
        thetadot    = 0;
    }

    //------------------------TKinTreeJointSensor-----------------------------//
    TKinTreeBodySensor::TKinTreeBodySensor()
        : TKinTreeSensor()
    {
        type        = "body";
        bodyName    = "";
    }

    //------------------------TKinTreeBody------------------------------------//
    TKinTreeBody::TKinTreeBody()
    {
        name            = "undefined";
        parentBodyPtr   = NULL;
        inertiaPtr      = NULL;
    }

    //------------------------TKinTreeJoint-----------------------------------//
    TKinTreeJoint::TKinTreeJoint()
    {
        name            = "undefined";
        type            = "hinge";
        axis            = { 1.0, 0.0, 0.0 };
        parentBodyPtr   = NULL;
        lowerLimit      = 1.0;      // defaults to lower>upper <> free
        upperLimit      = -1.0;     // defaults to lower>upper <> free
        limited         = false;
        stiffness       = 0.0;
        armature        = 0.0;
        damping         = 0.0;
        ref             = 0.0;
        nqpos           = 0;
        nqvel           = 0;
        qpos            = { 0., 0., 0., 0., 0., 0., 0. };
        qvel            = { 0., 0., 0., 0., 0., 0. };
        qpos0           = { 0., 0., 0., 0., 0., 0., 0. };
        qvel0           = { 0., 0., 0., 0., 0., 0. };
    }

    //------------------------TKinTreeActuator--------------------------------//
    TKinTreeActuator::TKinTreeActuator()
    {
        name        = "undefined";
        type        = "motor";
        jointPtr    = NULL;
        minCtrl     = -1.0;
        maxCtrl     = 1.0;
        ctrlValue   = 0.0;
        gear        = { 1, { 1.0 } };
        kp          = 1.0;
        kv          = 1.0;
        clampCtrl   = true;
    }

    //------------------------to-string herlpers------------------------------//

    std::string toString( TKinTreeBody* kinTreeBodyPtr )
    {
        if ( !kinTreeBodyPtr )
            return std::string( "\tbodyName: NULL\n\r" );

        std::string _res;

        _res += std::string( "\tbodyName: " ) + 
                kinTreeBodyPtr->name + 
                std::string( "\n\r" );

        _res += std::string( "\tbodyRelTransform: " ) +
                TMat4::toString( kinTreeBodyPtr->relTransform ) +
                std::string( "\n\r" );

        if ( kinTreeBodyPtr->inertiaPtr )
        {
            _res += std::string( "\tbodyMass: " ) + 
                    std::to_string( kinTreeBodyPtr->inertiaPtr->mass ) +
                    std::string( "\n\r" );

            auto _inertiaMat = TMat3( kinTreeBodyPtr->inertiaPtr->ixx,
                                      kinTreeBodyPtr->inertiaPtr->ixy,
                                      kinTreeBodyPtr->inertiaPtr->ixz,
                                      kinTreeBodyPtr->inertiaPtr->ixy,
                                      kinTreeBodyPtr->inertiaPtr->iyy,
                                      kinTreeBodyPtr->inertiaPtr->iyz,
                                      kinTreeBodyPtr->inertiaPtr->ixz,
                                      kinTreeBodyPtr->inertiaPtr->iyz,
                                      kinTreeBodyPtr->inertiaPtr->izz );

            _res += std::string( "\tbodyInertia: " ) + 
                    TMat3::toString( _inertiaMat ) +
                    std::string( "\n\r" );

            _res += std::string( "\tbodyInertiaRelFrame: " ) +
                    TMat4::toString( kinTreeBodyPtr->inertiaPtr->relTransform ) +
                    std::string( "\n\r" );
        }

        if ( kinTreeBodyPtr->parentBodyPtr )
        {
            _res += std::string( "\tbodyParentBody: " ) +
                    kinTreeBodyPtr->parentBodyPtr->name +
                    std::string( "\n\r" );
        }
        else
        {
            _res += std::string( "\tbodyParentBody: NULL\n\r" );
        }

        _res += std::string( "\tbodyChildVisuals: " );
        for ( size_t j = 0; j < kinTreeBodyPtr->childVisuals.size(); j++ )
        {
            _res += kinTreeBodyPtr->childVisuals[j]->name + 
                    ( ( j != ( kinTreeBodyPtr->childVisuals.size() - 1 ) ) ? 
                                std::string( ", " ) : std::string( "" ) );
        }
        _res += std::string( "\n\r" );

        _res += std::string( "\tbodyChildCollisions: " );
        for ( size_t j = 0; j < kinTreeBodyPtr->childCollisions.size(); j++ )
        {
            _res += kinTreeBodyPtr->childCollisions[j]->name + 
                    ( ( j != ( kinTreeBodyPtr->childCollisions.size() - 1 ) ) ? 
                                std::string( ", " ) : std::string( "" ) );
        }
        _res += std::string( "\n\r" );

        _res += std::string( "\tbodyChildJoints: " );
        for ( size_t j = 0; j < kinTreeBodyPtr->childJoints.size(); j++ )
        {
            _res += kinTreeBodyPtr->childJoints[j]->name + 
                    ( ( j != ( kinTreeBodyPtr->childJoints.size() - 1 ) ) ? 
                                std::string( ", " ) : std::string( "" ) );
        }
        _res += std::string( "\n\r" );

        return _res;
    }

    std::string toString( TKinTreeJoint* kinTreeJointPtr )
    {
        if ( !kinTreeJointPtr )
            return std::string( "\tjoint.name: NULL\n\r" );

        std::string _res;

        _res += std::string( "\tjoint.Name: " ) +
                kinTreeJointPtr->name + 
                std::string( "\n\r" );

        _res += std::string( "\tjoint.Type: " ) +
                kinTreeJointPtr->type + 
                std::string( "\n\r" );

        _res += std::string( "\tjoint.Axis: " ) +
                TVec3::toString( kinTreeJointPtr->axis ) + 
                std::string( "\n\r" );

        _res += std::string( "\tjoint.RelTransform: " ) +
                TMat4::toString( kinTreeJointPtr->relTransform ) + 
                std::string( "\n\r" );

        if ( kinTreeJointPtr->parentBodyPtr )
        {
            _res += std::string( "\tjoint.ParentBody: " ) +
                    kinTreeJointPtr->parentBodyPtr->name +
                    std::string( "\n\r" );
        }
        else
        {
            _res += std::string( "\tjoint.ParentBody: NULL\n\r" );
        }

        _res += std::string( "\tjoint.lowerLimit: " ) +
                std::to_string( kinTreeJointPtr->lowerLimit ) +
                std::string( "\n\r" );

        _res += std::string( "\tjoint.upperLimit: " ) +
                std::to_string( kinTreeJointPtr->upperLimit ) +
                std::string( "\n\r" );

        _res += std::string( "\tjoint.limited: " ) +
                std::string( ( kinTreeJointPtr->limited ) ? "true" : "false" ) +
                std::string( "\n\r" );

        return _res;
    }

    std::string toString( TKinTreeCollision* kinTreeCollisionPtr )
    {
        if ( !kinTreeCollisionPtr )
            return std::string( "\tcollision.Name: NULL\n\r" );

        std::string _res;

        _res += std::string( "\tcollision.Name: " ) +
                kinTreeCollisionPtr->name + 
                std::string( "\n\r" );

        _res += std::string( "\tcollision.Type: " ) +
                kinTreeCollisionPtr->geometry.type + 
                std::string( "\n\r" );

        _res += std::string( "\tcollision.MeshId: " ) +
                kinTreeCollisionPtr->geometry.meshId + 
                std::string( "\n\r" );

        _res += std::string( "\tcollision.Filename: " ) +
                kinTreeCollisionPtr->geometry.filename + 
                std::string( "\n\r" );

        _res += std::string( "\tcollision.Size: " ) +
                TVec3::toString( kinTreeCollisionPtr->geometry.size ) + 
                std::string( "\n\r" );

        _res += std::string( "\tcollision.RelTransform: " ) +
                TMat4::toString( kinTreeCollisionPtr->relTransform ) + 
                std::string( "\n\r" );

        if ( kinTreeCollisionPtr->parentBodyPtr )
        {
            _res += std::string( "\tcollision.ParentBody: " ) +
                    kinTreeCollisionPtr->parentBodyPtr->name +
                    std::string( "\n\r" );
        }
        else
        {
            _res += std::string( "\tcollision.ParentBody: NULL\n\r" );
        }

        return _res;
    }

    std::string toString( TKinTreeVisual* kinTreeVisualPtr )
    {
        if ( !kinTreeVisualPtr )
            return std::string( "\tvisual.name: NULL\n\r" );

        std::string _res;

        _res += std::string( "\tvisual.Name: " ) +
                kinTreeVisualPtr->name + 
                std::string( "\n\r" );

        _res += std::string( "\tvisual.Type: " ) +
                kinTreeVisualPtr->geometry.type + 
                std::string( "\n\r" );

        _res += std::string( "\tvisual.MeshId: " ) +
                kinTreeVisualPtr->geometry.meshId + 
                std::string( "\n\r" );

        _res += std::string( "\tvisual.Filename: " ) +
                kinTreeVisualPtr->geometry.filename + 
                std::string( "\n\r" );

        _res += std::string( "\tvisual.Size: " ) +
                TVec3::toString( kinTreeVisualPtr->geometry.size ) + 
                std::string( "\n\r" );

        _res += std::string( "\tvisual.RelTransform: " ) +
                TMat4::toString( kinTreeVisualPtr->relTransform ) + 
                std::string( "\n\r" );

        if ( kinTreeVisualPtr->parentBodyPtr )
        {
            _res += std::string( "\tvisual.ParentBody: " ) +
                    kinTreeVisualPtr->parentBodyPtr->name +
                    std::string( "\n\r" );
        }
        else
        {
            _res += std::string( "\tvisual.ParentBody: NULL\n\r" );
        }

        return _res;
    }

    std::string toString( TKinTreeSensor* kinTreeSensorPtr )
    {
        if ( !kinTreeSensorPtr )
            return std::string( "\tsensor.Name: NULL\n\r" );

        std::string _res;

        _res += std::string( "\tsensor.Name: " ) +
                kinTreeSensorPtr->name + 
                std::string( "\n\r" );

        _res += std::string( "\tsensor.Type: " ) +
                kinTreeSensorPtr->type + 
                std::string( "\n\r" );

        if ( kinTreeSensorPtr->bodyPtr )
        {
            _res += std::string( "\tsensor.ParentBody: " ) +
                    kinTreeSensorPtr->bodyPtr->name +
                    std::string( "\n\r" );
        }
        else
        {
            _res += std::string( "\tsensor.ParentBody: NULL\n\r" );
        }

        if ( kinTreeSensorPtr->jointPtr )
        {
            _res += std::string( "\tsensor.ChildBody: " ) +
                    kinTreeSensorPtr->jointPtr->name +
                    std::string( "\n\r" );
        }
        else
        {
            _res += std::string( "\tsensor.ChildBody: NULL\n\r" );
        }

        return _res;
    }

    std::string toString( TKinTreeActuator* kinTreeActuatorPtr )
    {
        if ( !kinTreeActuatorPtr )
            return std::string( "\tactuator.Name: NULL\n\r" );

        std::string _res;

        _res += std::string( "\tactuator.Name: " ) +
                kinTreeActuatorPtr->name + 
                std::string( "\n\r" );

        _res += std::string( "\tactuator.Type: " ) +
                kinTreeActuatorPtr->type + 
                std::string( "\n\r" );

        _res += std::string( "\tactuator.RelTransform: " ) +
                TMat4::toString( kinTreeActuatorPtr->relTransform ) + 
                std::string( "\n\r" );

        _res += std::string( "\tactuator.minCtrl: " ) +
                std::to_string( kinTreeActuatorPtr->minCtrl ) +
                std::string( "\n\r" );

        _res += std::string( "\tactuator.maxCtrl: " ) +
                std::to_string( kinTreeActuatorPtr->maxCtrl ) +
                std::string( "\n\r" );

        if ( kinTreeActuatorPtr->jointPtr )
        {
            _res += std::string( "\tactuator.ChildBody: " ) +
                    kinTreeActuatorPtr->jointPtr->name +
                    std::string( "\n\r" );
        }
        else
        {
            _res += std::string( "\tactuator.ChildBody: NULL\n\r" );
        }

        return _res;
    }

}}