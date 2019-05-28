
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

    struct TMjcSettings
    {
        bool useLocalCoordinates;
    };

    class TAgentKinTreeMjcf : public TAgentKinTree
    {

        protected :

        mjcf::GenericElement* m_modelElementPtr;
        std::map< std::string, TMjcfMeshAsset > m_mjcfMeshAssets;

        std::map< std::string, TGenericParams > m_mjcfDefaultsNoClass;
        std::map< std::string, std::map< std::string, TGenericParams > > m_mjcfDefaultsPerClass;

        TMjcSettings m_mjcfModelSettings;

        TKinTreeBody* _processBodyFromMjcf( mjcf::GenericElement* bodyElementPtr, TKinTreeBody* parentKinBodyPtr );
        TKinTreeJoint* _processJointFromMjcf( mjcf::GenericElement* jointElementPtr );
        TKinTreeVisual* _processVisualFromMjcf( mjcf::GenericElement* geomElementPtr );
        TKinTreeCollision* _processCollisionFromMjcf( mjcf::GenericElement* geomElementPtr );
        TKinTreeInertia* _processInertialFromMjcf( mjcf::GenericElement* inertialElmPtr );

        void _processActuator( mjcf::GenericElement* actuatorElementPtr );

        void _constructKinTree() override;

        void _collectDefaults();
        void _collectDefaultsFromClass( mjcf::GenericElement* defClassElmPtr );
        void _collectDefaultsNoClass( mjcf::GenericElement* defElmPtr );
        void _collectAssets();

        // helpers
        void _extractTransform( mjcf::GenericElement* elementPtr, TMat4& targetTransform );
        bool _extractStandardSize( mjcf::GenericElement* geomElm,
                                   TVec3& targetSize,
                                   TVec3& posFromFromto,
                                   TMat3& rotFromFromto );

        void _extractMjcfModelSettings();
        void _convertGlobalToLocalCoordinates( TKinTreeBody* kinTreeBodyPtr, 
                                               const TMat4& parentWorldTransform );
        TMat4 _convertGlobalToLocalTransform( const TMat4& parentWorldTransform,
                                              const TMat4& childWorldTransform );

        std::string _grabString( mjcf::GenericElement* elementPtr,
                                 const std::string& attribId,
                                 const std::string& defString );

        float _grabFloat( mjcf::GenericElement* elementPtr,
                          const std::string& attribId,
                          const float& defFloat );

        int _grabInt( mjcf::GenericElement* elementPtr,
                      const std::string& attribId,
                      const int& defInt );

        TVec2 _grabVec2( mjcf::GenericElement* elementPtr,
                         const std::string& attribId,
                         const TVec2& defVec2 );

        TVec3 _grabVec3( mjcf::GenericElement* elementPtr,
                         const std::string& attribId,
                         const TVec3& defVec3 );

        TVec4 _grabVec4( mjcf::GenericElement* elementPtr,
                         const std::string& attribId,
                         const TVec4& defVec4 );

        TSizef _grabArrayFloat( mjcf::GenericElement* elementPtr,
                                const std::string& attribId,
                                const TSizef& defSizef );

        TSizei _grabArrayInt( mjcf::GenericElement* elementPtr,
                              const std::string& attribId,
                              const TSizei& defSizei );

        bool _hasDefaultAttrib( mjcf::GenericElement* elementPtr,
                                const std::string& attribId );

        public :

        TAgentKinTreeMjcf( const std::string& name,
                           mjcf::GenericElement* modelElementPtr,
                           const TVec3& position,
                           const TVec3& rotation = TVec3() );

        ~TAgentKinTreeMjcf() override;

        mjcf::GenericElement* getMjcfModelDataPtr();
    };

    TAgentKinTree* createKinTreeAgent( const std::string& name,
                                       mjcf::GenericElement* modelDataPtr,
                                       const TVec3& position,
                                       const TVec3& rotation = TVec3() );
}}