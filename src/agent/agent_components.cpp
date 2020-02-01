
#include <agent/agent_components.h>

namespace tysoc {

    size_t NUM_UNNAMED_COLLISIONS   = 0;
    size_t NUM_UNNAMED_VISUALS      = 0;
    size_t NUM_UNNAMED_JOINTS       = 0;
    size_t NUM_UNNAMED_BODIES       = 0;

    //------------------------TKinTreeCollision-------------------------------//
    TKinTreeCollision::TKinTreeCollision()
    {
        name                = "undefined";
        data.type           = eShapeType::SPHERE;
        data.size           = { 0.1f, 0.0f, 0.0f };
        data.friction       = { 1., 0.005, 0.0001 };
        data.density        = TYSOC_DEFAULT_DENSITY;
        data.collisionGroup = 1;
        data.collisionMask  = 1;
        parentBodyPtr       = nullptr;
    }

    //------------------------TKinTreeVisual----------------------------------//
    TKinTreeVisual::TKinTreeVisual()
    {
        name                        = "undefined";
        data.type                   = eShapeType::SPHERE;
        data.size                   = { 0.1f, 0.0f, 0.0f };
        data.ambient                = tysoc::DEFAULT_AMBIENT_COLOR;
        data.diffuse                = tysoc::DEFAULT_DIFFUSE_COLOR;
        data.specular               = tysoc::DEFAULT_SPECULAR_COLOR;
        data.shininess              = tysoc::DEFAULT_SHININESS;
        data.texture                = "";
        data.usesMaterialFromMesh   = true;
        parentBodyPtr               = nullptr;
    }

    //------------------------TKinTreeSensor----------------------------------//
    TKinTreeSensor::TKinTreeSensor()
    {
        name        = "undefined";
        data.type   = eSensorType::NONE;
        bodyName    = "";
        jointName   = "";
        bodyPtr     = nullptr;
        jointPtr    = nullptr;
    }

    TKinTreeSensor::~TKinTreeSensor()
    {
        bodyPtr     = nullptr;
        jointPtr    = nullptr;
    }

    //------------------------TKinTreeJointSensor-----------------------------//
    TKinTreeJointSensor::TKinTreeJointSensor()
        : TKinTreeSensor()
    {
        data.type   = eSensorType::PROP_JOINT;
        theta       = 0.0f;
        thetadot    = 0.0f;
    }

    //------------------------TKinTreeJointSensor-----------------------------//
    TKinTreeBodySensor::TKinTreeBodySensor()
        : TKinTreeSensor()
    {
        data.type       = eSensorType::PROP_BODY;
        linVelocity     = { 0.0f, 0.0f, 0.0f };
        linAcceleration = { 0.0f, 0.0f, 0.0f };
        comForce        = { 0.0f, 0.0f, 0.0f };
        comTorque       = { 0.0f, 0.0f, 0.0f };
    }

    //------------------------TKinTreeBody------------------------------------//
    TKinTreeBody::TKinTreeBody()
    {
        name                = "undefined";
        inertialData.mass   = 0.0f; // 0.0 => compute from colliders
        inertialData.ixx    = 0.0f;
        inertialData.iyy    = 0.0f;
        inertialData.izz    = 0.0f;
        inertialData.ixy    = 0.0f;
        inertialData.ixz    = 0.0f;
        inertialData.iyz    = 0.0f;
        parentBodyPtr       = nullptr;
    }

    //------------------------TKinTreeJoint-----------------------------------//
    TKinTreeJoint::TKinTreeJoint( const eJointType& type )
    {
        name            = "undefined";
        data.type       = type;
        data.axis       = { 1.0f, 0.0f, 0.0f };
        data.limits     = { 1.0f, -1.0f }; // defaults to lower > upper <> free
        data.stiffness  = 0.0f;
        data.armature   = 0.0f;
        data.damping    = 0.0f;
        data.ref        = 0.0f;
        data.nqpos      = 0;
        data.nqvel      = 0;
        parentBodyPtr   = nullptr;
        userControlled  = false; // by default the simulation handles its qvalues
        qpos = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        qvel = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

        if ( type == eJointType::REVOLUTE )
        {
            // revolute joints have just 1dof, and 1qpos for this dof
            data.nqpos = 1;
            data.nqvel = 1;
            // qpos and qvel set to zero by default
            qpos0[0] = 0.0f;
            qvel0[0] = 0.0f;
        }
        else if ( type == eJointType::PRISMATIC )
        {
            // prismatic joints have just 1dof, and 1qpos for this dof
            data.nqpos = 1;
            data.nqvel = 1;
            // qpos and qvel set to zero by default
            qpos0[0] = 0.0f;
            qvel0[0] = 0.0f;
        }
        else if ( type == eJointType::SPHERICAL )
        {
            // spherical joints have 3dofs, and 4qpos (quaternion) representing it
            data.nqpos = 4;
            data.nqvel = 3;
            // qpos and qvel set to "zero" by default
            //       qx  qy  qz  qw   --unused--
            qpos0 = { 0., 0., 0., 1. }; // zero-quaternion
            //       ex  ey  ez  ---unused---
            qvel0 = { 0., 0., 0. }; // zero velocity
        }
        else if ( type == eJointType::FREE )
        {
            // free joints have 6dofs, and 7qpos (quat + xyz) representing it
            data.nqpos = 7;
            data.nqvel = 6;
            // qpos and qvel set to "zero" by default
            //        x   y   z  qx  qy  qz  qw
            qpos0 = { 0., 0., 0., 0., 0., 0., 1. }; // zero-xyz + zero-quat
            //        x   y   z   ex  ey  ez
            qvel0 = { 0., 0., 0., 0., 0., 0. }; // zero-xyz + zero-euler? velocity
        }
        else if ( type == eJointType::FIXED )
        {
            // fixed joints have 0dofs, as expected
            data.nqpos = 0;
            data.nqvel = 0;
        }
        else
        {
            TYSOC_CORE_ERROR( "Joint \"{0}\" has unsupported type \"{1}\"", name, tysoc::toString( type ) );
        }
    }

    //------------------------TKinTreeActuator--------------------------------//
    TKinTreeActuator::TKinTreeActuator()
    {
        name        = "undefined";
        data.type   = eActuatorType::TORQUE;
        data.limits = { -1.0, 1.0f };
        data.gear   = { 1, { 1.0 } };
        data.kp     = 1.0f;
        data.kv     = 1.0f;
        ctrlValue   = 0.0f;
        jointPtr    = nullptr;
    }

    //------------------------to-string helpers-------------------------------//

    std::string toString( TKinTreeBody* kinTreeBodyPtr )
    {
        if ( !kinTreeBodyPtr )
            return std::string( "\tBody.name             : nullptr\n\r" );

        std::string _strRep;

        _strRep += std::string( "\tBody.name                : " ) + kinTreeBodyPtr->name + "\n\r";
        _strRep += std::string( "\tBody.mass                : " ) + std::to_string( kinTreeBodyPtr->inertialData.mass ) + "\n\r";
        _strRep += std::string( "\tBody.ixx                 : " ) + std::to_string( kinTreeBodyPtr->inertialData.ixx ) + "\n\r";
        _strRep += std::string( "\tBody.ixy                 : " ) + std::to_string( kinTreeBodyPtr->inertialData.ixy ) + "\n\r";
        _strRep += std::string( "\tBody.ixz                 : " ) + std::to_string( kinTreeBodyPtr->inertialData.ixz ) + "\n\r";
        _strRep += std::string( "\tBody.iyy                 : " ) + std::to_string( kinTreeBodyPtr->inertialData.iyy ) + "\n\r";
        _strRep += std::string( "\tBody.iyz                 : " ) + std::to_string( kinTreeBodyPtr->inertialData.iyz ) + "\n\r";
        _strRep += std::string( "\tBody.izz                 : " ) + std::to_string( kinTreeBodyPtr->inertialData.izz ) + "\n\r";
        _strRep += std::string( "\tBody.inertiaFrame        : " ) + tysoc::toString( kinTreeBodyPtr->inertialData.localTransform ) + "\n\r";

        if ( kinTreeBodyPtr->parentBodyPtr )
            _strRep += std::string( "\tBody.parentBody          : " ) + kinTreeBodyPtr->parentBodyPtr->name + "\n\r";
        else
            _strRep += std::string( "\tBody.parentBody          : nullptr\n\r" );

        _strRep += std::string( "\tVisuals                  : " );
        for ( auto _visual : kinTreeBodyPtr->visuals )
            _strRep += ( _visual ) ? ( _visual->name + " | " ) : ( "nullptr | " );
        _strRep += std::string( "\n\r" );

        _strRep += std::string( "\tCollisions               : " );
        for ( auto _collision : kinTreeBodyPtr->collisions )
            _strRep += ( _collision ) ? ( _collision->name + " | " ) : ( "nullptr | " );
        _strRep += std::string( "\n\r" );

        _strRep += std::string( "\tJoints                   : " );
        for ( auto _joint : kinTreeBodyPtr->joints )
            _strRep += ( _joint ) ? ( _joint->name + " | " ) : ( "nullptr | " );
        _strRep += std::string( "\n\r" );

        _strRep += std::string( "\tBody.localTransformZero  : " ) + tysoc::toString( kinTreeBodyPtr->localTransformZero ) + "\n\r";
        _strRep += std::string( "\tBody.worldTransform      : " ) + tysoc::toString( kinTreeBodyPtr->worldTransform ) + "\n\r";

        return _strRep;
    }

    std::string toString( TKinTreeJoint* kinTreeJointPtr )
    {
        if ( !kinTreeJointPtr )
            return std::string( "\tJoint.name                   : nullptr\n\r" );

        std::string _strRep;

        _strRep += std::string( "\tJoint.name                   : " ) + kinTreeJointPtr->name + "\n\r";
        _strRep += std::string( "\tJoint.type                   : " ) + tysoc::toString( kinTreeJointPtr->data.type ) + "\n\r";
        _strRep += std::string( "\tjoint.axis                   : " ) + tysoc::toString( kinTreeJointPtr->data.axis ) + "\n\r";
        _strRep += std::string( "\tJoint.limits                 : " ) + tysoc::toString( kinTreeJointPtr->data.limits ) + "\n\r";
        _strRep += std::string( "\tJoint.stiffness              : " ) + std::to_string( kinTreeJointPtr->data.stiffness ) + "\n\r";
        _strRep += std::string( "\tJoint.armature               : " ) + std::to_string( kinTreeJointPtr->data.armature ) + "\n\r";
        _strRep += std::string( "\tJoint.damping                : " ) + std::to_string( kinTreeJointPtr->data.damping ) + "\n\r";
        _strRep += std::string( "\tJoint.ref                    : " ) + std::to_string( kinTreeJointPtr->data.ref ) + "\n\r";
        _strRep += std::string( "\tJoint.nqpos                  : " ) + std::to_string( kinTreeJointPtr->data.nqpos ) + "\n\r";
        _strRep += std::string( "\tJoint.nqvel                  : " ) + std::to_string( kinTreeJointPtr->data.nqvel ) + "\n\r";
        _strRep += std::string( "\tJoint.localTransform         : \n\r" ) + tysoc::toString( kinTreeJointPtr->data.localTransform ) +  "\n\r";

        _strRep += std::string( "\tJoint.qpos                   : |");
        for ( auto _qpos_i : kinTreeJointPtr->qpos )
            _strRep += std::to_string( _qpos_i ) + " | ";
        _strRep += std::string( "\n\r" );

        _strRep += std::string( "\tJoint.qvel                   : |");
        for ( auto _qvel_i : kinTreeJointPtr->qvel )
            _strRep += std::to_string( _qvel_i ) + " | ";
        _strRep += std::string( "\n\r" );

        _strRep += std::string( "\tJoint.qpos0                  : |");
        for ( auto _qpos0_i : kinTreeJointPtr->qpos0 )
            _strRep += std::to_string( _qpos0_i ) + " | ";
        _strRep += std::string( "\n\r" );

        _strRep += std::string( "\tJoint.qvel0                  : |");
        for ( auto _qvel0_i : kinTreeJointPtr->qvel0 )
            _strRep += std::to_string( _qvel0_i ) + " | ";
        _strRep += std::string( "\n\r" );

        if ( kinTreeJointPtr->parentBodyPtr )
            _strRep += std::string( "\tJoint.ParentBody             : " ) + kinTreeJointPtr->parentBodyPtr->name + "\n\r";
        else
            _strRep += std::string( "\tJoint.ParentBody             : nullptr\n\r" );

        _strRep += std::string( "\tJoint.worldTransform         : " ) + tysoc::toString( kinTreeJointPtr->worldTransform ) + "\n\r";

        return _strRep;
    }

    std::string toString( TKinTreeCollision* kinTreeCollisionPtr )
    {
        if ( !kinTreeCollisionPtr )
            return std::string( "\tCollision.name   : nullptr\n\r" );

        std::string _strRep;

        _strRep += std::string( "\tCollision.name               : " ) + kinTreeCollisionPtr->name + "\n\r";
        _strRep += std::string( "\tCollision.shape              : " ) + tysoc::toString( kinTreeCollisionPtr->data.type ) + "\n\r";
        _strRep += std::string( "\tCollision.size               : " ) + tysoc::toString( kinTreeCollisionPtr->data.size ) + "\n\r";
        _strRep += std::string( "\tCollision.filename           : " ) + kinTreeCollisionPtr->data.filename + "\n\r";
        _strRep += std::string( "\tCollision.localTransform     : " ) + tysoc::toString( kinTreeCollisionPtr->data.localTransform ) + "\n\r";
        _strRep += std::string( "\tCollision.collisionGroup     : " ) + std::to_string( kinTreeCollisionPtr->data.collisionGroup ) + "\n\r";
        _strRep += std::string( "\tCollision.collisionMask      : " ) + std::to_string( kinTreeCollisionPtr->data.collisionMask ) + "\n\r";
        _strRep += std::string( "\tCollision.friction           : " ) + tysoc::toString( kinTreeCollisionPtr->data.friction ) + "\n\r";
        _strRep += std::string( "\tCollision.density            : " ) + std::to_string( kinTreeCollisionPtr->data.density ) + "\n\r";

        if ( kinTreeCollisionPtr->parentBodyPtr )
            _strRep += std::string( "\tCollision.parentBody         : " ) + kinTreeCollisionPtr->parentBodyPtr->name + "\n\r";
        else
            _strRep += std::string( "\tCollision.parentBody         : nullptr\n\r" );

        _strRep += std::string( "\tCollision.worldTransform     : " ) + tysoc::toString( kinTreeCollisionPtr->worldTransform ) + "\n\r";

        return _strRep;
    }

    std::string toString( TKinTreeVisual* kinTreeVisualPtr )
    {
        if ( !kinTreeVisualPtr )
            return std::string( "\tVisual.name  : nullptr\n\r" );

        std::string _strRep;

        _strRep += std::string( "\tVisual.name              : " ) + kinTreeVisualPtr->name + "\n\r";
        _strRep += std::string( "\tVisual.shape             : " ) + tysoc::toString( kinTreeVisualPtr->data.type ) + "\n\r";
        _strRep += std::string( "\tVisual.size              : " ) + tysoc::toString( kinTreeVisualPtr->data.size ) + "\n\r";
        _strRep += std::string( "\tVisual.filename          : " ) + kinTreeVisualPtr->data.filename + "\n\r";
        _strRep += std::string( "\tVisual.localTransform    : " ) + tysoc::toString( kinTreeVisualPtr->data.localTransform ) + "\n\r";
        _strRep += std::string( "\tVisual.ambient           : " ) + tysoc::toString( kinTreeVisualPtr->data.ambient ) + "\n\r";
        _strRep += std::string( "\tVisual.diffuse           : " ) + tysoc::toString( kinTreeVisualPtr->data.diffuse ) + "\n\r";
        _strRep += std::string( "\tVisual.specular          : " ) + tysoc::toString( kinTreeVisualPtr->data.specular ) + "\n\r";
        _strRep += std::string( "\tVisual.shininess         : " ) + std::to_string( kinTreeVisualPtr->data.shininess ) + "\n\r";
        _strRep += std::string( "\tVisual.texture           : " ) + kinTreeVisualPtr->data.texture + "\n\r";

        if ( kinTreeVisualPtr->parentBodyPtr )
            _strRep += std::string( "\tVisual.parentBody        : " ) + kinTreeVisualPtr->parentBodyPtr->name + "\n\r";
        else
            _strRep += std::string( "\tVisual.parentBody        : nullptr\n\r" );

        _strRep += std::string( "\tVisual.worldTransform    : " ) + tysoc::toString( kinTreeVisualPtr->worldTransform ) + "\n\r";

        return _strRep;
    }

    std::string toString( TKinTreeSensor* kinTreeSensorPtr )
    {
        if ( !kinTreeSensorPtr )
            return std::string( "\tSensor.name  : nullptr\n\r" );

        std::string _strRep;

        _strRep += std::string( "\tSensor.name                  : " ) + kinTreeSensorPtr->name + "\n\r";
        _strRep += std::string( "\tSensor.type                  : " ) + tysoc::toString( kinTreeSensorPtr->data.type ) + "\n\r";
        _strRep += std::string( "\tSensor.localTransform        : " ) + tysoc::toString( kinTreeSensorPtr->data.localTransform ) + "\n\r";

        _strRep += std::string( "\tSensor.bodyName              : " ) + kinTreeSensorPtr->bodyName + "\n\r";
        if ( kinTreeSensorPtr->bodyPtr )
            _strRep += std::string( "\tSensor.bodyPtr               : " ) + kinTreeSensorPtr->bodyPtr->name + "\n\r";
        else
            _strRep += std::string( "\tSensor.bodyPtr               : nullptr\n\r" );

        _strRep += std::string( "\tSensor.jointName             : " ) + kinTreeSensorPtr->jointName + "\n\r";
        if ( kinTreeSensorPtr->jointPtr )
            _strRep += std::string( "\tSensor.jointPtr              : " ) + kinTreeSensorPtr->jointPtr->name + "\n\r";
        else
            _strRep += std::string( "\tSensor.jointPtr              : nullptr\n\r" );

        if ( kinTreeSensorPtr->data.type == eSensorType::PROP_JOINT )
        {
            _strRep += std::string( "\tSensor.theta                 : " ) + std::to_string( dynamic_cast< TKinTreeJointSensor* >( kinTreeSensorPtr )->theta ) + "\n\r";
            _strRep += std::string( "\tSensor.thetadot              : " ) + std::to_string( dynamic_cast< TKinTreeJointSensor* >( kinTreeSensorPtr )->thetadot ) + "\n\r";
        }
        else if ( kinTreeSensorPtr->data.type == eSensorType::PROP_BODY )
        {
            _strRep += std::string( "\tSensor.linVelocity           : " ) + tysoc::toString( dynamic_cast< TKinTreeBodySensor* >( kinTreeSensorPtr )->linVelocity ) + "\n\r";
            _strRep += std::string( "\tSensor.linAcceleration       : " ) + tysoc::toString( dynamic_cast< TKinTreeBodySensor* >( kinTreeSensorPtr )->linAcceleration ) + "\n\r";
            _strRep += std::string( "\tSensor.comForce              : " ) + tysoc::toString( dynamic_cast< TKinTreeBodySensor* >( kinTreeSensorPtr )->comForce ) + "\n\r";
            _strRep += std::string( "\tSensor.comTorque             : " ) + tysoc::toString( dynamic_cast< TKinTreeBodySensor* >( kinTreeSensorPtr )->comTorque ) + "\n\r";
        }

        // @todo: add extra logging for other sensor-types as well
        // ...

        _strRep += std::string( "\tSensor.worldTransform        : " ) + tysoc::toString( kinTreeSensorPtr->worldTransform ) + "\n\r";

        return _strRep;
    }

    std::string toString( TKinTreeActuator* kinTreeActuatorPtr )
    {
        if ( !kinTreeActuatorPtr )
            return std::string( "\tActuator.name    : nullptr\n\r" );

        std::string _strRep;

        _strRep += std::string( "\tActuator.name            : " ) + kinTreeActuatorPtr->name +  "\n\r";
        _strRep += std::string( "\tActuator.type            : " ) + tysoc::toString( kinTreeActuatorPtr->data.type ) + "\n\r";
        _strRep += std::string( "\tActuator.limits          : " ) + tysoc::toString( kinTreeActuatorPtr->data.limits ) + "\n\r";
        _strRep += std::string( "\tActuator.gear            : [" ) + tysoc::toString( kinTreeActuatorPtr->data.gear ) + "]\n\r";
        _strRep += std::string( "\tActuator.kp              : " ) + std::to_string( kinTreeActuatorPtr->data.kp ) + "\n\r";
        _strRep += std::string( "\tActuator.kv              : " ) + std::to_string( kinTreeActuatorPtr->data.kv ) + "\n\r";
        _strRep += std::string( "\tActuator.localTransform  : " ) + tysoc::toString( kinTreeActuatorPtr->data.localTransform ) + "\n\r";
        _strRep += std::string( "\tActuator.ctrlValue       : " ) + std::to_string( kinTreeActuatorPtr->ctrlValue ) + "\n\r";

        if ( kinTreeActuatorPtr->jointPtr )
            _strRep += std::string( "\tActuator.jointPtr        : " ) + kinTreeActuatorPtr->jointPtr->name + "\n\r";
        else
            _strRep += std::string( "\tActuator.jointPtr        : nullptr\n\r" );

        _strRep += std::string( "\tActuator.worldTransform  : " ) + tysoc::toString( kinTreeActuatorPtr->worldTransform ) + "\n\r";

        return _strRep;
    }

}