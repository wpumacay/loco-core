
#pragma once

// urdf functionality
#include <utils/parsers/urdf/urdf.h>
// base agent functionality
#include <agent/agent_base.h>

namespace tysoc {
namespace agent {

    class TAgent;

    /* Storage used while parsing the urdf data into a kinematic tree */
    struct TUrdfParsingContext
    {
        // Agent being constructed with this context
        TAgent* agentPtr;
        // Data used to construct the agent
        urdf::UrdfModel* modelDataPtr;
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
                                            urdf::UrdfVisual* urdfVisualtPtr );

    TKinTreeCollision* _processCollisionFromUrdf( TUrdfParsingContext& context, 
                                                  urdf::UrdfCollision* urdfCollisionPtr );

    TKinTreeInertia* _processInertialFromUrdf( TUrdfParsingContext& context, 
                                               urdf::UrdfInertia* urdfInertiaPtr );

    void _collectAssetsFromLink( TUrdfParsingContext& context, 
                                  urdf::UrdfLink* urdfLinkPtr );

    void _constructDefaultActuators( TUrdfParsingContext& context );

    void _extractStandardSize( urdf::UrdfGeometry* urdfGeometryPtr,
                               TVec3& targetSize );

}}