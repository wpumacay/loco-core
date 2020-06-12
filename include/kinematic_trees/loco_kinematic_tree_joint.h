#pragma once

#include <loco_data.h>
#include <kinematic_trees/loco_kinematic_tree_body.h>
#include <kinematic_trees/loco_kinematic_tree_joint_adapter.h>

namespace loco {
namespace kintree {

    /// Struct with the details rqquired to create a kinematic-tree joint
    struct TKinematicTreeJointData
    {
        /// Enum representing the type of joint
        eJointType type = eJointType::REVOLUTE;
        /// Limits for the movement of the body along the joint (if high < low, then no limits)
        TVec2 limits = { 1.0f, -1.0f };
        /// Stiffness value (for the spring-like behaviour)
        TScalar stiffness = 0.0f;
        /// Extra term added to the inertia matrix diagonal to make simulation more stable
        TScalar armature = 0.0f;
        /// Axis-1 used to define the plane for planar joints
        TVec3 plane_axis_1 = { 1.0f, 0.0f, 0.0f };
        /// Axis-2 used to define the plane for planar joints
        TVec3 plane_axis_2 = { 0.0f, 0.0f, 1.0f };
        /// Axis used for either rotation or translation (PRISMATIC or REVOLUTE types)
        TVec3 local_axis = { 1.0f, 0.0f, 0.0f };
        /// Relative transform of the joint w.r.t. its body owner
        TMat4 local_tf;
    };

    class TKinematicTreeBody;
    class TIKinematicTreeJointAdapter;

    class TKinematicTreeJoint : public TObject
    {
    public :

        TKinematicTreeJoint( const std::string& name,
                             const TKinematicTreeJointData& joint_data );

        virtual ~TKinematicTreeJoint();

        void SetParentBody( TKinematicTreeBody* parent_body_ref, const TMat4& local_tf );

        void SetJointAdapter( TIKinematicTreeJointAdapter* joint_adapter_ref );

        void SetQpos( const std::vector<TScalar>& qpos );

        void SetQpos0( const std::vector<TScalar>& qpos0 );

        void SetQvel( const std::vector<TScalar>& qvel );

        void SetQvel0( const std::vector<TScalar>& qvel0 );

        void SetStiffness( const TScalar& stiffness );

        void SetArmature( const TScalar& armature );

        void SetLocalTransform( const TMat4& local_tf );

        ssize_t num_qpos() const { return m_NumQpos; }

        ssize_t num_qvel() const { return m_NumQvel; }

        const std::vector<TScalar>& qpos() const { return m_Qpos; }

        const std::vector<TScalar>& qpos0() const { return m_Qpos0; }

        const std::vector<TScalar>& qvel() const { return m_Qvel; }

        const std::vector<TScalar>& qvel0() const { return m_Qvel0; }

        eJointType type() const { return m_Data.type; }

        TScalar stiffness() const { return m_Data.stiffness; }

        TScalar armature() const { return m_Data.armature; }

        const TMat4& local_tf() const { return m_LocalTf; }

        TKinematicTreeJointData& data() { return m_Data; }

        const TKinematicTreeJointData& data() const { return m_Data; }

        TKinematicTreeBody* parent() { return m_ParentBodyRef; }

        const TKinematicTreeBody* parent() const { return m_ParentBodyRef; }

        TIKinematicTreeJointAdapter* adapter() { return m_JointAdapterRef; }

        const TIKinematicTreeJointAdapter* adapter() const { return m_JointAdapterRef; }

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;

        void _DetachSimInternal() override;

        void _DetachVizInternal() override;

        void _SetTransformInternal( const TMat4& transform ) override;

        eObjectType _GetObjectTypeInternal() const override { return eObjectType::KINEMATIC_TREE_JOINT; }

    protected :

        TKinematicTreeJointData m_Data;

        TKinematicTreeBody* m_ParentBodyRef = nullptr;

        TIKinematicTreeJointAdapter* m_JointAdapterRef = nullptr;

        TMat4 m_LocalTf;

        std::vector<TScalar> m_Qpos;

        std::vector<TScalar> m_Qpos0;

        std::vector<TScalar> m_Qvel;

        std::vector<TScalar> m_Qvel0;

        ssize_t m_NumQpos = -1;

        ssize_t m_NumQvel = -1;
    };

    class TKinematicTreeRevoluteJoint : public TKinematicTreeJoint
    {
    public :

        TKinematicTreeRevoluteJoint( const std::string& name,
                                     const TMat4& local_tf,
                                     const TVec3& local_axis,
                                     const TVec2& limits = { 1.0f, -1.0f },
                                     const TScalar& stiffness = 0.0f,
                                     const TScalar& armature = 0.0f );

        ~TKinematicTreeRevoluteJoint() = default;

        void SetAngle( const TScalar& angle );

        void SetAxis( const TVec3& axis );

        void SetLimits( const TVec2& limits );

        TScalar angle() const { return m_Qpos[0]; }

        TVec3 axis() const { return m_Data.local_axis; }

        TVec2 limits() const { return m_Data.limits; }
    };

    class TKinematicTreePrismaticJoint : public TKinematicTreeJoint
    {
    public :

        TKinematicTreePrismaticJoint( const std::string& name,
                                      const TMat4& local_tf,
                                      const TVec3& local_axis,
                                      const TVec2& limits = { 1.0f, -1.0f },
                                      const TScalar& stiffness = 0.0f,
                                      const TScalar& armature = 0.0f );

        ~TKinematicTreePrismaticJoint() = default;

        void SetPosition( const TScalar& position );

        void SetAxis( const TVec3& axis );

        void SetLimits( const TVec2& limits );

        TScalar position() const { return m_Qpos[0]; }

        TVec3 axis() const { return m_Data.local_axis; }

        TVec2 limits() const { return m_Data.limits; }
    };

    class TKinematicTreeSphericalJoint : public TKinematicTreeJoint
    {
    public :

        TKinematicTreeSphericalJoint( const std::string& name,
                                      const TMat4& local_tf,
                                      const TVec2& limits = { 1.0f, -1.0f },
                                      const TScalar& stiffness = 0.0f,
                                      const TScalar& armature = 0.0f );

        ~TKinematicTreeSphericalJoint() = default;

        void SetQuat( const TVec4& quat );

        void SetLimits( const TVec2& limits );

        TVec4 quat() const { return { m_Qpos[0], m_Qpos[1], m_Qpos[2], m_Qpos[3] }; }

        TVec2 limits() const { return m_Data.limits; }
    };

    class TKinematicTreePlanarJoint : public TKinematicTreeJoint
    {
    public :

        TKinematicTreePlanarJoint( const std::string& name,
                                   const TVec3& plane_axis_1,
                                   const TVec3& plane_axis_2 );

        ~TKinematicTreePlanarJoint() = default;
    };

    class TKinematicTreeFixedJoint : public TKinematicTreeJoint
    {
    public :

        TKinematicTreeFixedJoint( const std::string& name,
                                  const TMat4& local_tf );

        ~TKinematicTreeFixedJoint() = default;
    };

    class TKinematicTreeFreeJoint : public TKinematicTreeJoint
    {
    public :

        TKinematicTreeFreeJoint( const std::string& name );

        ~TKinematicTreeFreeJoint() = default;
    };

}}