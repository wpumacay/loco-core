
#pragma once

// urdf functionality
#include <utils/parsers/urdf/urdf.h>
// include kintree to extend
#include <agent/types/agent_kintree.h>


namespace tysoc {
namespace agent {

    struct TUrdfMeshAsset
    {
        std::string     name;
        std::string     file;
        TVec3           scale;

        TUrdfMeshAsset()
        {
            name = "";
            file = "";
            scale = { 1, 1, 1 };
        }
    };


    class TAgentKinTreeUrdf : public TAgentKinTree
    {

        protected :

        urdf::UrdfModel* m_urdfModelPtr;
        std::map< std::string, TUrdfMeshAsset > m_urdfMeshAssets;

        TKinTreeBody* _processBodyFromUrdf( urdf::UrdfLink* urdfLinkPtr, TKinTreeBody* parentKinBodyPtr );
        TKinTreeJoint* _processJointFromUrdf( urdf::UrdfJoint* urdfJointPtr );
        TKinTreeVisual* _processVisualFromUrdf( urdf::UrdfVisual* urdfVisualtPtr );
        TKinTreeCollision* _processCollisionFromUrdf( urdf::UrdfCollision* urdfCollisionPtr );
        TKinTreeInertia* _processInertialFromUrdf( urdf::UrdfInertia* urdfInertiaPtr );

        void _constructKinTree() override;

        void _collectAssets();
        void _collectAssetsFromLink( urdf::UrdfLink* urdfLinkPtr );

        void _processJointConnection( urdf::UrdfJoint* urdfJointPtr );

        void _constructDefaultActuators();

        /**
        * @brief                    Extracts a TVec3 with the primitives standard size for the framework
        * @param urdfGeometryPtr    urdf::UrdfGeometry pointer that holds the size and other info of the current element
        * @param targetSize         TVec3 in which to store the normalized calculations
        */
        void _extractStandardSize( urdf::UrdfGeometry* urdfGeometryPtr,
                                   TVec3& targetSize );

        public :

        TAgentKinTreeUrdf( const std::string& name,
                           const TVec3& position,
                           urdf::UrdfModel* urdfModelPtr );

        ~TAgentKinTreeUrdf() override;

        urdf::UrdfModel* getUrdfModelDataPtr();

    };

    TAgentKinTree* createKinTreeAgent( const std::string& name,
                                       const TVec3& position,
                                       urdf::UrdfModel* modelDataPtr );

}}