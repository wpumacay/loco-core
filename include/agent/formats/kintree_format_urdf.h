
#pragma once

// urdf functionality
#include <utils/parsers/urdf/urdf.h>
// base agent functionality
#include <agent/agent_base.h>

namespace tysoc {

    class TAgent;

    /* Storage used while parsing the urdf data into a kinematic tree */
    struct TUrdfParsingContext
    {
        // Agent being constructed with this context
        TAgent* agentPtr;
        // Data used to construct the agent
        urdf::UrdfModel* modelDataPtr;
        // File path used to load the model
        std::string filepath;
        // Folder path from where the model was loaded
        std::string folderpath;
    };

    /* Creates an agent from the given urdf model */
    void constructAgentFromModel( TAgent* agentPtr,
                                  urdf::UrdfModel* modelDataPtr );

    /*************************************************************
    *   Helpers used to construct the components of the kintree  *
    **************************************************************/

    TKinTreeBody* _processBodyFromUrdf( TUrdfParsingContext& context, 
                                        urdf::UrdfLink* urdfLinkPtr, 
                                        TKinTreeBody* parentKinBodyPtr );

    TKinTreeJoint* _processJointFromUrdf( TUrdfParsingContext& context, 
                                          urdf::UrdfJoint* urdfJointPtr );

    TKinTreeVisual* _processVisualFromUrdf( TUrdfParsingContext& context, 
                                            const urdf::UrdfVisual& urdfVisual );

    TKinTreeCollision* _processCollisionFromUrdf( TUrdfParsingContext& context, 
                                                  const urdf::UrdfCollision& urdfCollision );

    TInertialData _processInertialFromUrdf( TUrdfParsingContext& context, 
                                            const urdf::UrdfInertia& urdfInertia );

    void _constructDefaultActuators( TUrdfParsingContext& context );

    void _extractStandardSize( const urdf::UrdfShape& urdfShape,
                               TVec3& targetSize );

}