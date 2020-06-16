#pragma once

#include <loco_data.h>
#include <visualizer/loco_visualizer_drawable.h>
#include <kinematic_trees/loco_kinematic_tree_collider.h>
#include <kinematic_trees/loco_kinematic_tree_joint.h>
#include <kinematic_trees/loco_kinematic_tree.h>
#include <kinematic_trees/loco_kinematic_tree_body_adapter.h>

namespace loco {
namespace visualizer {
    class TDrawable;
}}

namespace loco {
namespace kintree {
    struct TKinematicTreeJointData;
}}

namespace loco {
namespace kintree {

    //--------------------------------------------------------------------------------------------//
    //                                                                                            //
    //                               Kinematic-Tree-Body Interface                                //
    //                                                                                            //
    //--------------------------------------------------------------------------------------------//

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

        TKinematicTreeBody( const std::string& name );

        TKinematicTreeBody( const std::string& name,
                            const TKinematicTreeBodyData& body_data );

        virtual ~TKinematicTreeBody();

        void SetBodyAdapter( TIKinematicTreeBodyAdapter* body_adapter_ref );

        void SetKintree( TKinematicTree* kintree_ref );

        void SetParentBody( TKinematicTreeBody* parent_body_ref, const TMat4& local_tf );

        void AddDrawable( std::unique_ptr<visualizer::TDrawable> drawable, const TMat4& local_tf );

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

        std::vector<visualizer::TDrawable*> drawables();

        std::vector<const visualizer::TDrawable*> drawables() const;

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

        std::vector<std::unique_ptr<visualizer::TDrawable>> m_Drawables;

        std::vector<std::unique_ptr<TKinematicTreeCollider>> m_Colliders;

        std::vector<std::unique_ptr<TKinematicTreeJoint>> m_Joints;

        std::vector<std::unique_ptr<TKinematicTreeBody>> m_Children;

        TVec3 m_TotalForce = { 0.0f, 0.0f, 0.0f };

        TVec3 m_TotalTorque = { 0.0f, 0.0f, 0.0f };

        TMat4 m_LocalTf;
    };

    //--------------------------------------------------------------------------------------------//
    //                                                                                            //
    //                              Kinematic-Tree-Body Primitives                                //
    //                                                                                            //
    //--------------------------------------------------------------------------------------------//

    class TBox : public TKinematicTreeBody
    {
    public :

        TBox( const std::string& name,
              const TVec3& extents,
              const int& collision_group = 1,
              const int& collision_mask = 1 );

        TBox( const TBox& other ) = delete;

        TBox& operator=( const TBox& other ) = delete;

        ~TBox() = default;

        /// Sets the width|depth|height extents of the box collider
        ///
        /// @param extents  Vector containing the witdh, depth, and height for the box collider
        void SetExtents( const TVec3& extents );

        /// Sets the width of the box collider
        ///
        /// @param width    Width of the box collider
        void SetWidth( const TScalar& width );

        /// Sets the depth of the box collider
        ///
        /// @param depth    Depth of the box collider
        void SetDepth( const TScalar& depth );

        /// Sets the height of the box collider
        ///
        /// @param height   Height of the box collider
        void SetHeight( const TScalar& height );

        /// Gets the extents of the box collider
        ///
        /// @return Vector containing the witdh, depth, and height for the box collider
        TVec3 extents() const { return m_Extents; }

        /// Gets the width of the box collider
        ///
        /// @return Width value of the box collider
        TScalar width() const { return m_Extents.x(); }

        /// Gets the depth of the box collider
        ///
        /// @return Depth value of the box collider
        TScalar depth() const { return m_Extents.y(); }

        /// Gets the height of the box collider
        ///
        /// @return Height value of the box collider
        TScalar height() const { return m_Extents.z(); }

    private :

        /// Extents of the box-shaped collider|drawable
        TVec3 m_Extents;
    };

    class TSphere : public TKinematicTreeBody
    {
    public :

        TSphere( const std::string& name,
                 const TScalar& radius,
                 const int& collision_group = 1,
                 const int& collision_mask = 1 );

        TSphere( const TSphere& other ) = delete;

        TSphere& operator=( const TSphere& other ) = delete;

        ~TSphere() = default;

        /// Sets the radius of the sphere-shaped collider|drawable
        ///
        /// @param radius   Radius of the sphere
        void SetRadius( const TScalar& radius );

        /// Gets the current radius of the sphere-shaped collider|drawable
        ///
        /// @return Radius of the sphere-shaped collider|drawable
        TScalar radius() const { return m_Radius; }

    private :

        /// Radius of the sphere-shaped collider|drawable
        TScalar m_Radius;
    };

    class TCylinder : public TKinematicTreeBody
    {
    public :

        TCylinder( const std::string& name,
                   const TScalar& radius,
                   const TScalar& height,
                   const int& collision_group = 1,
                   const int& collision_mask = 1 );

        TCylinder( const TCylinder& other ) = delete;

        TCylinder& operator=( const TCylinder& other ) = delete;

        ~TCylinder() = default;

        /// Sets the radius of the cylinder-shaped collider|drawable
        ///
        /// @param radius   Radius of the cylinder-shaped collider|drawable
        void SetRadius( const TScalar& radius );

        /// Sets the height of the cylinder-shaped collider|drawable
        ///
        /// @param height   Heights of the cylinder-shaped collider|drawable
        void SetHeight( const TScalar& height );

        /// Gets the radius of the cylinder-shaped collider|drawable
        ///
        /// @return Radius of the cylinder-shaped collider|drawable
        TScalar radius() const { return m_Radius; }

        /// Gets the heights of the cylinder-shapes collider|drawable
        ///
        /// @return Height of the cylinder-shaped collider|drawable
        TScalar height() const { return m_Height; }

    private :

        /// Radius of the cylinder-shaped collider|drawable
        TScalar m_Radius;
        /// Height of the cylinder-shaped collider|drawable
        TScalar m_Height;
    };

    class TCapsule : public TKinematicTreeBody
    {
    public :

        TCapsule( const std::string& name,
                  const TScalar& radius,
                  const TScalar& height,
                  const int& collision_group = 1,
                  const int& collision_mask = 1 );

        TCapsule( const TCapsule& other ) = delete;

        TCapsule& operator=( const TCapsule& other ) = delete;

        ~TCapsule() = default;

        /// Sets the radius of the capsule-shaped collider|drawable
        ///
        /// @param radius   Radius of the capsule-shaped collider|drawable
        void SetRadius( const TScalar& radius );

        /// Sets the height of the capsule-shaped collider|drawable
        ///
        /// @param height   Heights of the capsule-shaped collider|drawable
        void SetHeight( const TScalar& height );

        /// Gets the radius of the capsule-shaped collider|drawable
        ///
        /// @return Radius of the capsule-shaped collider|drawable
        TScalar radius() const { return m_Radius; }

        /// Gets the height of the capsule-shaped collider|drawable
        ///
        /// @return Height of the capsule-shaped collider|drawable
        TScalar height() const { return m_Height; }

    private :

        /// Radius of the capsule-shaped collider|drawable
        TScalar m_Radius;
        /// Height of the capsule-shaped collider|drawable
        TScalar m_Height;
    };

    class TEllipsoid : public TKinematicTreeBody
    {
    public :

        TEllipsoid( const std::string& name,
                    const TVec3& radii,
                    const int& collision_group = 1,
                    const int& collision_mask = 1 );

        TEllipsoid( const TEllipsoid& other ) = delete;

        TEllipsoid& operator=( const TEllipsoid& other ) = delete;

        ~TEllipsoid() = default;

        /// Sets the radii (x,y,z radius) of the ellipsoid-shaped collider|drawable
        ///
        /// @param radii    Radii (vector-3 with x,y,z radius) of the ellipsoid-shaped collider|drawable
        void SetRadii( const TVec3& radii );

        /// Sets the x-radius of the ellipsoid-shaped collider|drawable
        ///
        /// @param radius_x X-radius of the ellipsoid-shaped collider|drawable
        void SetRadiusX( const TScalar& radius_x );

        /// Sets the y-radius of the ellipsoid-shaped collider|drawable
        ///
        /// @param radius_y Y-radius of the ellipsoid-shaped collider|drawable
        void SetRadiusY( const TScalar& radius_y );

        /// Sets the z-radius of the ellipsoid-shaped collider|drawable
        ///
        /// @param radius_z Z-radius of the ellipsoid-shaped collider|drawable
        void SetRadiusZ( const TScalar& radius_z );

        /// Gets the radii of the ellipsoid-shaped collider|drawable
        ///
        /// @return Radii (vector-3 with x,y,z radius) of the ellipsoid-shaped collider|drawable
        TVec3 radii() const { return m_Radii; }

        /// Gets the x-radius of the ellipsoid-shaped collider|drawable
        ///
        /// @return X-radius of the ellipsoid-shaped collider|drawable
        TScalar radius_x() const { return m_Radii.x(); }

        /// Gets the y-radius of the ellipsoid-shaped collider|drawable
        ///
        /// @return Y-radius of the ellipsoid-shaped collider|drawable
        TScalar radius_y() const { return m_Radii.y(); }

        /// Gets the z-radius of the ellipsoid-shaped collider|drawable
        ///
        /// @return Z-radius of the ellipsoid-shaped collider|drawable
        TScalar radius_z() const { return m_Radii.z(); }

    private :

        TVec3 m_Radii;
    };

    class TConvexMesh : public TKinematicTreeBody
    {
    public :

        TConvexMesh( const std::string& name,
                     const std::string& mesh_collider_filepath,
                     const std::string& mesh_visual_filepath,
                     const TScalar& mesh_scale,
                     const int& collision_group = 1,
                     const int& collision_mask = 1 );

        TConvexMesh( const TConvexMesh& other ) = delete;

        TConvexMesh& operator=( const TConvexMesh& other ) = delete;

        ~TConvexMesh() = default;

        /// Sets the scale of the mesh collider|drawable
        ///
        /// @param scale    Scale factor applied to the mesh collider|drawable
        void SetScale( const TScalar& scale );

        /// Gets the scale of the mesh collider|drawable
        ///
        /// @return Scale factor of the mesh collider|drawable
        TScalar scale() const { return m_Scale; }

    private :

        /// Scale of the mesh collider|drawable
        TScalar m_Scale;
    };
}}