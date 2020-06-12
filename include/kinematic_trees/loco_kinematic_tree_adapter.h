#pragma once

#include <kinematic_trees/loco_kinematic_tree.h>
#include <kinematic_trees/loco_kinematic_tree_body_adapter.h>

namespace loco {
namespace kintree {

    class TKinematicTree;
    class TIKinematicTreeBodyAdapter;

    class TIKinematicTreeAdapter
    {
    public :

        TIKinematicTreeAdapter( TKinematicTree* kintree_ref );

        virtual ~TIKinematicTreeAdapter();

        virtual void Build() = 0;

        virtual void Initialize() = 0;

        virtual void Reset() = 0;

        virtual void OnDetach();

        virtual void SetTransform( const TMat4& tf ) = 0;

        virtual void SetLinearVelocity( const TVec3& linear_vel ) = 0;

        virtual void SetAngularVelocity( const TVec3& angular_vel ) = 0;

        virtual void GetTransform( TMat4& dst_tf ) = 0;

        virtual void GetLinearVelocity( TVec3& dst_linearl_vel ) = 0;

        virtual void GetAngularVelocity( TVec3& dst_angular_vel ) = 0;

        TKinematicTree* kintree() { return m_KintreeRef; }

        const TKinematicTree* kintree() const { return m_KintreeRef; }

        std::vector<TIKinematicTreeBodyAdapter*> body_adapters();

        std::vector<const TIKinematicTreeBodyAdapter*> body_adapters() const;

        bool detached() const { return m_Detached; }

    protected :

        /// Reference to kintree that handles this adapter
        TKinematicTree* m_KintreeRef;
        /// Flags used to check resource state (either linked to simulation or not)
        bool m_Detached;
        /// Body-adapters owned by this kintree-adapter, and handled by corresponding body objects
        std::vector<std::unique_ptr<TIKinematicTreeBodyAdapter>> m_BodyAdapters;
    };
}}