#pragma once

// mjcf functionality
#include <utils/parsers/mjcf/mjcf.h>
// base agent functionality
#include <agent/agent_base.h>

namespace tysoc {

    class TAgent;

    /* Storage used while parsing the mjcf data into a kinematic tree */
    struct TMjcfParsingContext
    {
        // Agent being constructed with this context
        TAgent* agentPtr;
        // Data used to construct the agent
        mjcf::GenericElement* modelDataPtr;
        // Storage for the default values for no-class cases (see mjcf format)
        std::map< std::string, TGenericParams > defaultsNoClass;
        // Storage for the default values related to specific classes (see mjcf format)
        std::map< std::string, std::map< std::string, TGenericParams > > defaultsPerClass;
        // Storage for mesh-assets
        std::map< std::string, TGenericParams > assetsMeshes;
        // Storage for material-assets
        std::map< std::string, TGenericParams > assetsMaterials;
        // File path used to load the model
        std::string filepath;
        // Folder path from where the model was loaded
        std::string folderpath;
        // File name, without folder path
        std::string filename;
        // Flag to indicate whether or not we are using local coordinates
        bool useLocalCoordinates;
        // Flag to indicate whether or not use degrees or radings for angles
        bool useDegrees;
    };

    /* Creates an agent from the given mjcf model */
    mjcf::GenericElement* constructAgentFromModel( TAgent* agentPtr,
                                                   mjcf::GenericElement* modelDataPtr );

    /*************************************************************
    *   Helpers used to construct the components of the kintree  *
    **************************************************************/

    TKinTreeBody* _processBodyFromMjcf( TMjcfParsingContext& context, 
                                        mjcf::GenericElement* bodyElementPtr, 
                                        TKinTreeBody* parentKinBodyPtr );

    TKinTreeJoint* _processJointFromMjcf( TMjcfParsingContext& context, 
                                          mjcf::GenericElement* jointElementPtr );

    TKinTreeVisual* _processVisualFromMjcf( TMjcfParsingContext& context, 
                                            mjcf::GenericElement* geomElementPtr );

    TKinTreeCollision* _processCollisionFromMjcf( TMjcfParsingContext& context, 
                                                  mjcf::GenericElement* geomElementPtr );

    TInertialData _processInertialFromMjcf( TMjcfParsingContext& context, 
                                            mjcf::GenericElement* inertialElmPtr );

    TKinTreeActuator* _processActuator( TMjcfParsingContext& context, 
                                        mjcf::GenericElement* actuatorElementPtr );

    void _collectDefaults( TMjcfParsingContext& context );

    void _collectDefaultsFromClass( TMjcfParsingContext& context, 
                                    mjcf::GenericElement* defClassElmPtr );

    void _collectDefaultsNoClass( TMjcfParsingContext& context, 
                                  mjcf::GenericElement* defElmPtr );

    void _collectAssets( TMjcfParsingContext& context );

    void _extractTransform( TMjcfParsingContext& context, 
                            mjcf::GenericElement* elementPtr, 
                            TMat4& targetTransform );

    std::string _extractFilename( TMjcfParsingContext& context, const std::string& meshStr );

    bool _extractStandardSize( TMjcfParsingContext& context,
                               mjcf::GenericElement* geomElm,
                               TVec3& targetSize,
                               TVec3& posFromFromto,
                               TMat3& rotFromFromto );

    /* Grabs the settings from mjcf model and saves it into the current context */
    void _extractMjcfModelSettings( TMjcfParsingContext& context );

    /* Helper used to convert global to local coordinates */
    void _convertGlobalToLocalCoordinates( TKinTreeBody* kinTreeBodyPtr, 
                                           const TMat4& parentWorldTransform );

    /* Creates the local transform that satisfies both world transforms given */
    TMat4 _convertGlobalToLocalTransform( const TMat4& parentWorldTransform,
                                          const TMat4& childWorldTransform );

    /*************************************************************
    *   Safe parsing that takes into account defaults from model *
    **************************************************************/

    std::string _grabString( TMjcfParsingContext& context,
                             mjcf::GenericElement* elementPtr,
                             const std::string& attribId,
                             const std::string& defString );

    float _grabFloat( TMjcfParsingContext& context,
                      mjcf::GenericElement* elementPtr,
                      const std::string& attribId,
                      const float& defFloat );

    int _grabInt( TMjcfParsingContext& context,
                  mjcf::GenericElement* elementPtr,
                  const std::string& attribId,
                  const int& defInt );

    TVec2 _grabVec2( TMjcfParsingContext& context,
                     mjcf::GenericElement* elementPtr,
                     const std::string& attribId,
                     const TVec2& defVec2 );

    TVec3 _grabVec3( TMjcfParsingContext& context,
                     mjcf::GenericElement* elementPtr,
                     const std::string& attribId,
                     const TVec3& defVec3 );

    TVec4 _grabVec4( TMjcfParsingContext& context,
                     mjcf::GenericElement* elementPtr,
                     const std::string& attribId,
                     const TVec4& defVec4 );

    TSizef _grabArrayFloat( TMjcfParsingContext& context,
                            mjcf::GenericElement* elementPtr,
                            const std::string& attribId,
                            const TSizef& defSizef );

    TSizei _grabArrayInt( TMjcfParsingContext& context,
                          mjcf::GenericElement* elementPtr,
                          const std::string& attribId,
                          const TSizei& defSizei );

    bool _hasDefaultAttrib( TMjcfParsingContext& context,
                            mjcf::GenericElement* elementPtr,
                            const std::string& attribId );
}