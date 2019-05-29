
#pragma once

// urdf functionality
#include <utils/parsers/rlsim/rlsim.h>
// include kintree to extend
#include <agent/types/agent_kintree.h>


namespace tysoc {
namespace agent {


    class TAgentKinTreeRlsim : public TAgentKinTree
    {

        protected :

        rlsim::RlsimModel* m_rlsimModelPtr;

        std::map< std::string, rlsim::RlsimJoint* > m_jointNameToJointNodeMap;

        TKinTreeBody* _processNode( rlsim::RlsimJoint* rlsimJointPtr, 
                                    TKinTreeBody* parentKinBodyPtr );

        TKinTreeJoint* _processJointFromRlsim( rlsim::RlsimJoint* rlsimJointPtr );

        TKinTreeVisual* _processVisualFromRlsim( rlsim::RlsimVisual* rlsimVisualPtr );

        TKinTreeCollision* _processCollisionFromRlsim( rlsim::RlsimBody* rlSimBodyPtr );

        TKinTreeInertia* _processInertialFromRlsim( rlsim::RlsimBody* rlSimBodyPtr );

        void _constructDefaultActuators();

        void _extractStandardSize( const std::string& shapeType,
                                   const TVec3& srcSize,
                                   TVec3& dstSize );

        void _constructKinTree() override;

        public :

        TAgentKinTreeRlsim( const std::string& name,
                            rlsim::RlsimModel* rlsimModelPtr,
                            const TVec3& position,
                            const TVec3& rotation = TVec3() );

        ~TAgentKinTreeRlsim() override;

        rlsim::RlsimModel* getRlsimModelDataPtr();
    };

    TAgentKinTree* createKinTreeAgent( const std::string& name,
                                       rlsim::RlsimModel* modelDataPtr,
                                       const TVec3& position,
                                       const TVec3& rotation = TVec3() );

}}