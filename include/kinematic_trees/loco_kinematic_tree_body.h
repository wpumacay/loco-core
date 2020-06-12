#pragma once

#include <loco_data.h>
#include <visualizer/loco_visualizer_drawable.h>
#include <kinematic_trees/loco_kinematic_tree_collider.h>
#include <kinematic_trees/loco_kinematic_tree_joint.h>
#include <kinematic_trees/loco_kinematic_tree.h>
#include <kinematic_trees/loco_kinematic_tree_body_adapter.h>

namespace loco {
    class TDrawable;
}

namespace loco {
namespace kintree {

    struct TKinematicTreeJointData;
    /// Struct with the details required to create a kinematic-tree body
    struct TKinematicTreeBodyData
    {
        /// Inertial properties of the body
        TInertialData inertia;
        /// List of all colliders owned by the body
        std::vector<TCollisionData> colliders;
        /// List of all drawables owned by the body
        std::vector<TVisualData> drawables;
        /// List of all joints owned by the body
        std::vector<TKinematicTreeJointData> joints;
    };

    class TKinematicTreeCollider;
    class TKinematicTreeJoint;
    class TKinematicTree;
    class TIKinematicTreeBodyAdapter;

    class TKinematicTreeBody : public TObject
    {
    public :

        TKinematicTreeBody( const std::string& name,
                            const TKinematicTreeBodyData& body_data,
                            const TVec3& position = TVec3(),
                            const TMat3& rotation = TMat3() );

        ~TKinematicTreeBody();

        void SetBodyAdapter( TIKinematicTreeBodyAdapter* body_adapter_ref );

        void SetKintree( TKinematicTree* kintree_ref );

        void SetParentBody( TKinematicTreeBody* parent_body_ref, const TMat4& local_tf );

        void AddDrawable( std::unique_ptr<TDrawable> drawable, const TMat4& local_tf );

        void AddCollider( std::unique_ptr<TKinematicTreeCollider> collider, const TMat4& local_tf );

        void AddJoint( std::unique_ptr<TKinematicTreeJoint> joint, const TMat4& local_tf );

        void AddChild( std::unique_ptr<TKinematicTreeBody> body, const TMat4& local_tf );

        void AddForceCOM( const TVec3& force );

        void AddTorqueCOM( const TVec3& torque );

        void Forward();

        const TMat4& local_tf() const { return m_LocalTf; }

        TKinematicTreeBodyData& data() { return m_Data; }

        const TKinematicTreeBodyData& data() const { return m_Data; }

        TKinematicTree* kintree() { return m_KinematicTreeRef; }

        const TKinematicTree* kintree() const { return m_KinematicTreeRef; }

        TKinematicTreeBody* parent() { return m_ParentBodyRef; }

        const TKinematicTreeBody* parent() const { return m_ParentBodyRef; }

        TIKinematicTreeBodyAdapter* adapter() { return m_BodyAdapterRef; }

        const TIKinematicTreeBodyAdapter* adapter() const { return m_BodyAdapterRef; }

        std::vector<TDrawable*> drawables();

        std::vector<const TDrawable*> drawables() const;

        std::vector<TKinematicTreeCollider*> colliders();

        std::vector<const TKinematicTreeCollider*> colliders() const;

        std::vector<TKinematicTreeJoint*> joints();

        std::vector<const TKinematicTreeJoint*> joints() const;

        std::vector<TKinematicTreeBody*> children();

        std::vector<const TKinematicTreeBody*> children() const;

        static eObjectType GetStaticType() { return eObjectType::KINEMATIC_TREE_BODY; }

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;

        void _DetachSimInternal() override;

        void _DetachVizInternal() override;

        void _SetTransformInternal( const TMat4& transform ) override;

        eObjectType _GetObjectTypeInternal() const override { return eObjectType::KINEMATIC_TREE_BODY; }

    protected :

        TKinematicTreeBodyData m_Data;

        TKinematicTree* m_KinematicTreeRef = nullptr;

        TKinematicTreeBody* m_ParentBodyRef = nullptr;

        TIKinematicTreeBodyAdapter* m_BodyAdapterRef = nullptr;

        std::vector<std::unique_ptr<TDrawable>> m_Drawables;

        std::vector<std::unique_ptr<TKinematicTreeCollider>> m_Colliders;

        std::vector<std::unique_ptr<TKinematicTreeJoint>> m_Joints;

        std::vector<std::unique_ptr<TKinematicTreeBody>> m_Children;

        TVec3 m_TotalForce = { 0.0f, 0.0f, 0.0f };

        TVec3 m_TotalTorque = { 0.0f, 0.0f, 0.0f };

        TMat4 m_LocalTf;
    };

}}