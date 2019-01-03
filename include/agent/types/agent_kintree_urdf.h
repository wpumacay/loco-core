
#pragma once

// urdf functionality
#include <utils/parsers/urdf/urdf.h>
// include kintree to extend
#include <agent/types/agent_kintree.h>


namespace tysoc{
namespace agent{

    struct TUrdfMeshAsset
    {
        std::string                             name;
        std::string                             file;
        struct { float x; float y; float z; }   scale;
    };


    class TAgentKinTreeUrdf : public TAgentKinTree
    {

        protected :

        std::map< std::string, TUrdfMeshAsset > m_urdfMeshAssets;

        public :

        TAgentKinTreeUrdf( const std::string& name,
                           const TVec3& position,
                           urdf::UrdfModel* urdfModelPtr );

        ~TAgentKinTreeUrdf() override;


    };


}}