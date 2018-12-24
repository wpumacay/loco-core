
#pragma once



namespace tysoc {
namespace urdf {

    UrdfModel* loadGenericModel( const std::string& modelfile );

    bool _parseTransform( btTransform& transform, 
                          tinyxml2::XMLElement* xml );

    bool _parseInertia( UrdfInertia* inertiaDataPtr, 
                        tinyxml2::XMLElement* xmlElement );

    bool _parseGeometry( UrdfGeometry* geomDataPtr, 
                         tinyxml2::XMLElement* g );

    bool _parseVisual( UrdfModel* modelDataPtr, 
                       UrdfVisual* visual, 
                       tinyxml2::XMLElement* xmlElement );

    bool _parseCollision( UrdfCollision* collisionDataPtr, 
                          tinyxml2::XMLElement* xmlElement );

    bool _initTreeAndRoot( UrdfModel* modelDataPtr );

    bool _parseMaterial( UrdfMaterial* materialDataPtr, 
                         tinyxml2::XMLElement* xmlElement );

    bool _parseJointLimits( UrdfJoint* jointDataPtr, 
                            tinyxml2::XMLElement* xmlElement );

    bool _parseJointDynamics( UrdfJoint* jointDataPtr, 
                              tinyxml2::XMLElement* xmlElement );

    bool _parseJoint( UrdfJoint* jointDataPtr, 
                      tinyxml2::XMLElement* xmlElement );

    bool _parseLink( UrdfModel* modelDataPtr, 
                     UrdfLink* link, 
                     tinyxml2::XMLElement* xmlElement );

    void _logError( const std::string& errorMsg );
    
    void _logWarning( const std::string& warningMsg );

}}