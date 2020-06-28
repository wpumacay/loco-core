#pragma once

#include <utils/loco_parsing_element.h>
#include <kinematic_trees/loco_kinematic_tree_body.h>
#include <kinematic_trees/loco_kinematic_tree_adapter.h>
#include <kinematic_trees/parsers/loco_kinematic_tree_mjcf_parser.h>
#include <kinematic_trees/parsers/loco_kinematic_tree_urdf_parser.h>

namespace loco {
namespace kintree {
    class TKinematicTreeBody;
    class TKinematicTreeJoint;
    class TIKinematicTreeAdapter;
    class TKinematicTreeMjcfParser;
}}

namespace loco {
namespace kintree {

    class TKinematicTree : public TObject
    {
    public :

        TKinematicTree( const std::string& name,
                        const TVec3& position,
                        const TMat3& rotation );

        ~TKinematicTree();

        void LoadFromMjcf( const std::string& filepath_mjcf );

        void LoadFromUrdf( const std::string& filepath_urdf );

        void LoadFromRlsim( const std::string& filepath_rlsim );

        void SetKintreeAdapter( TIKinematicTreeAdapter* kintree_adapter_ref );

        void SetRoot( std::unique_ptr<TKinematicTreeBody> body );

        void RegisterBody( TKinematicTreeBody* body );

        void SetInitialPosition( const TVec3& position0 ) { m_tf0.set( position0, 3 ); }

        void SetInitialRotation( const TMat3& rotation0 ) { m_tf0.set( rotation0 ); }

        void SetInitialEuler( const TVec3& euler0 ) { m_tf0.set( tinymath::rotation( euler0 ) ); }

        void SetInitialQuaternion( const TVec4& quat0 ) { m_tf0.set( tinymath::rotation( quat0 ) ); }

        void SetInitialTransform( const TMat4& transform0 ) { m_tf0 = transform0; }

        ssize_t GetNumBodies() const { return m_NumKintreeBodies; }

        bool HasBody( const std::string& name ) const;

        void RemoveBodyByName( const std::string& name );

        TKinematicTreeBody* GetBodyByName( const std::string& name );

        const TKinematicTreeBody* GetBodyByName( const std::string& name ) const;

        std::vector<TKinematicTreeBody*> GetBodiesList();

        std::vector<const TKinematicTreeBody*> GetBodiesList() const;

        TVec3 pos0() const { return TVec3( m_tf0.col( 3 ) ); }

        TMat3 rot0() const { return TMat3( m_tf0 ); }

        TVec3 euler0() const { return tinymath::euler( m_tf0 ); }

        TVec4 quat0() const { return tinymath::quaternion( m_tf0 ); }

        TMat4 tf0() const { return m_tf0; }

        parsing::TElement* mjcf_model() { return m_MjcfModel.get(); }

        const parsing::TElement* mjcf_model() const { return m_MjcfModel.get(); }

        parsing::TElement* urdf_model() { return m_UrdfModel.get(); }

        const parsing::TElement* urdf_model() const { return m_UrdfModel.get(); }

        parsing::TElement* rlsim_model() { return m_RlsimModel.get(); }

        const parsing::TElement* rlsim_model() const { return m_RlsimModel.get(); }

        TKinematicTreeBody* root() { return m_Root.get(); }

        const TKinematicTreeBody* root() const { return m_Root.get(); }

        TIKinematicTreeAdapter* adapter() { return m_KintreeAdapterRef; }

        const TIKinematicTreeAdapter* adapter() const { return m_KintreeAdapterRef; }

        static eObjectType GetStaticType() { return eObjectType::KINEMATIC_TREE; }

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;

        void _DetachSimInternal() override;

        void _DetachVizInternal() override;

        void _SetTransformInternal( const TMat4& transform ) override;

        eObjectType _GetObjectTypeInternal() const override { return eObjectType::KINEMATIC_TREE; }

    protected :

        /// Root kintree-body of this kinematic-tree
        std::unique_ptr<TKinematicTreeBody> m_Root = nullptr;
        /// Reference to the adapter for this kinematic-tree
        TIKinematicTreeAdapter* m_KintreeAdapterRef = nullptr;
        /// Number of kintree-bodies registered in this kintree
        ssize_t m_NumKintreeBodies = 0;
        /// Hashmap for kintree-bodies registered in this kintree
        std::unordered_map<std::string, TKinematicTreeBody*> m_BodiesMap;
        /// Model information for MuJoCo's mjcf file format
        std::unique_ptr<parsing::TElement> m_MjcfModel = nullptr;
        /// Model information for URDF-Xml file format
        std::unique_ptr<parsing::TElement> m_UrdfModel = nullptr;
        /// Model information for RLSIM-Json file format
        std::unique_ptr<parsing::TElement> m_RlsimModel = nullptr;
        /// Initial world-transform of this kinematic-tree on the scenario
        TMat4 m_tf0;
        /// Current value of the linear velocity of the base|root of the kinematic tree
        TVec3 m_LinearVel;
        /// Initial value of the linear velocity of the base|root of the kinematic tree
        TVec3 m_LinearVel0;
        /// Current value of the angular velocity of the base|root of the kinemtaic tree
        TVec3 m_AngularVel;
        /// Initial value of the angular velocity of the base|root of the kinematic tree
        TVec3 m_AngularVel0;
    };
}}