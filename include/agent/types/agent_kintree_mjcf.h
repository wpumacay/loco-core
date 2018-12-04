
#pragma once

// mjcf functionality
#include <mjcf/mjcf_api.h>
// include kintrees to wrap
#include <agent/types/agent_kintree.h>


namespace tysoc{
namespace agent{




    class TAgentKinTreeMjcf : public TAgentKinTree
    {

        protected :

        TKinTreeBody* _processBodyFromMjcf( mjcf::GenericElement* bodyElementPtr, TKinTreeBody* parentKinBodyPtr );
        TKinTreeJoint* _processJointFromMjcf( mjcf::GenericElement* jointElementPtr );
        TKinTreeVisual* _processVisualFromMjcf( mjcf::GenericElement* geomElementPtr );
        TKinTreeCollision* _processCollisionFromMjcf( mjcf::GenericElement* geomElementPtr );

        void _processActuator( mjcf::GenericElement* actuatorElementPtr );

        mjcf::GenericElement* m_modelElementPtr;

        // helpers
        void _extractTransform( mjcf::GenericElement* elementPtr, TMat4& targetTransform );
        bool _extractStandardSize( mjcf::GenericElement* geomElm,
                                   TVec3& targetSize,
                                   TVec3& posFromFromto,
                                   TMat3& rotFromFromto );

        void _constructKinTree() override;
        void _initializeWorldTransforms() override;
        void _initializeBody( TKinTreeBody* kinTreeBodyPtr );

        public :

        TAgentKinTreeMjcf( const std::string& name,
                           const TVec3& position,
                           mjcf::GenericElement* modelElementPtr );

        ~TAgentKinTreeMjcf() override;


    };


}}