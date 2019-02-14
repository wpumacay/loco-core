
#pragma once

// mjcf functionality
#include <utils/parsers/mjcf/mjcf.h>
// include kintree to extend
#include <agent/types/agent_kintree.h>


namespace tysoc {
namespace agent {

    struct TMjcfMeshAsset
    {
        std::string                             name;
        std::string                             file;
        struct { float x; float y; float z; }   scale;
    };


    class TAgentKinTreeMjcf : public TAgentKinTree
    {

        protected :

        mjcf::GenericElement* m_modelElementPtr;
        std::map< std::string, TMjcfMeshAsset > m_mjcfMeshAssets;

        TKinTreeBody* _processBodyFromMjcf( mjcf::GenericElement* bodyElementPtr, TKinTreeBody* parentKinBodyPtr );
        TKinTreeJoint* _processJointFromMjcf( mjcf::GenericElement* jointElementPtr );
        TKinTreeVisual* _processVisualFromMjcf( mjcf::GenericElement* geomElementPtr );
        TKinTreeCollision* _processCollisionFromMjcf( mjcf::GenericElement* geomElementPtr );
        TKinTreeInertia* _processInertialFromMjcf( mjcf::GenericElement* inertialElmPtr );

        void _processActuator( mjcf::GenericElement* actuatorElementPtr );

        void _constructKinTree() override;

        void _collectAssets();

        // helpers
        void _extractTransform( mjcf::GenericElement* elementPtr, TMat4& targetTransform );
        bool _extractStandardSize( mjcf::GenericElement* geomElm,
                                   TVec3& targetSize,
                                   TVec3& posFromFromto,
                                   TMat3& rotFromFromto );

        public :

        TAgentKinTreeMjcf( const std::string& name,
                           const TVec3& position,
                           mjcf::GenericElement* modelElementPtr );

        ~TAgentKinTreeMjcf() override;

        mjcf::GenericElement* getMjcfModelDataPtr();
    };

    TAgentKinTree* createKinTreeAgent( const std::string& name,
                                       const TVec3& position,
                                       mjcf::GenericElement* modelDataPtr );
}}