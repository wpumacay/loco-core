#pragma once

#include <primitives/loco_single_body.h>

namespace loco {
namespace primitives {
    /// Box primitive, corresponding to a single-body with box-shaped collider and drawable
    ///
    /// @details
    /// This helper class creates a single body with a box-shaped collider, defined
    /// by its x-y-z extents (width|depth|height). This body exposes its extents as
    /// user modifiable parameters to the user, which can be changed at runtime
    class TBox : public TSingleBody
    {
    public :

        /// Constructs a box primitive from its extents (width|depth|height)
        ///
        /// @details
        /// Creates a single-body object with a box collider, given width (x-extent),
        /// depth (y-extent), and height (z-extent) provided by the user. The plane is placed
        /// at the given position (xyz-coordinates) and with the given orientation (rotation-matrix),
        /// and the user has the ability to define its dynamics type (either DYNAMIC or STATIC),
        /// as well as its collision filtering flags (collision-group and collision-mask) which can be
        /// used for broadphase collision checking and exclusion accordingly
        /// 
        /// @param name             Unique identifier of the primitive in the simulation
        /// @param extents          Vector containing the witdh, depth, and height for the box collider
        /// @param position         Initial position of the body object (xyz position)
        /// @param rotation         Initial rotation of the body object (rotation matrix)
        /// @param dyntype          Dynamics type, either DYNAMIC or STATIC
        /// @param collision_group  Collision group used for collision filtering
        /// @param collision_mask   Collision mask used for collision filtering
        TBox( const std::string& name,
              const TVec3& extents,
              const TVec3& position,
              const TMat3& rotation,
              const eDynamicsType& dyntype = eDynamicsType::DYNAMIC,
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

    /// Plane primitive, corresponding to a single-body with plane-shaped collider and drawable
    ///
    /// @details
    /// This helper class creates a single body with a plane-shaped collider, defined
    /// by its x-y extents (width|depth). This body exposes its extents as user modifiable
    /// parameters to the user, which can be changed at runtime
    class TPlane : public TSingleBody
    {
    public :

        /// Constructs a plane primitive from user-specified width and depth
        ///
        /// @details
        /// Creates a single-body object with a plane collider, given width (x-extent),
        /// and depth (y-extent) provided by the user. The plane is placed at the given
        /// position (xyz-coordinates) and with the given orientation (rotation-matrix),
        /// and the user has the ability to define its collision filtering flags (group and mask)
        /// The resulting primitive is always a STATIC single-body object (dynamics won't affect it)
        /// 
        /// @param name             Unique identifier of the primitive in the simulation
        /// @param width            Witdh (x-extent) of the plane-shaped collider
        /// @param depth            Depth (y-extent) of the plane-shaped collider
        /// @param position         Initial position of the body object (xyz position)
        /// @param rotation         Initial rotation of the body object (rotation matrix)
        /// @param collision_group  Collision group used for collision filtering
        /// @param collision_mask   Collision mask used for collision filtering
        TPlane( const std::string& name,
                const TScalar& width,
                const TScalar& depth,
                const TVec3& position,
                const TMat3& rotation,
                const int& collision_group = 0x7fffffff,
                const int& collision_mask = 0x7fffffff );

        TPlane( const TPlane& other ) = delete;

        TPlane& operator=( const TPlane& other ) = delete;

        ~TPlane() = default;

        /// Sets the width of the plane-shaped collider|drawable
        ///
        /// @param width    Width extension (x-axis) of the plane
        void SetWidth( const TScalar& width );

        /// Sets the depth of the plane-shaped collider|drawable
        ///
        /// @param depth    Depth extension (y-axis) of the plane
        void SetDepth( const TScalar& depth );

        /// Gets the width of the plane-shaped collider|drawable
        ///
        /// @return Width extension (x-axis) of the plane
        TScalar width() const { return m_Width; }

        /// Gets the depth of the plane-shaped collider|drawable
        ///
        /// @return Depth extension (y-axis) of the plane
        TScalar depth() const { return m_Depth; }

    private :

        /// Width of the plane-shaped collider|drawable
        TScalar m_Width;
        /// Depth of the plane-shaped collider|drawable
        TScalar m_Depth;
    };

    /// Sphere primitive, corresponding to a single-body with sphere-shaped collider and drawable
    ///
    /// @details
    /// This helper class creates a single body with a sphere-shaped collider, defined
    /// by its radius. This body exposes its radius as a user modifiable parameter to the
    /// user, which can be changed at runtime
    class TSphere : public TSingleBody
    {
    public :

        /// Constructs a sphere primitive from a user-specified radius
        ///
        /// @details
        /// Creates a single-body object with a sphere collider and sphere drawable, given the
        /// radius of the sphere provided by the user. The sphere is placed at the given position
        /// (xyz-coordinates) and with the given orientation (rotation-matrix), and the user has
        /// the ability to define its dynamics type (either DYNAMIC or STATIC), as well as its
        /// collision filtering flags (collision-group and collision-mask) which can be used for
        /// broadphase collision checking and exclusion accordingly
        ///
        /// @param name             Unique identifier of the primitive in the simulation
        /// @param radius           Radius of the sphere-shaped collider|drawable
        /// @param position         Initial position of the body object (xyz position)
        /// @param rotation         Initial rotation of the body object (rotation matrix)
        /// @param dyntype          Dynamics type, either DYNAMIC or STATIC
        /// @param collision_group  Collision group used for collision filtering
        /// @param collision_mask   Collision mask used for collision filtering
        TSphere( const std::string& name,
                 const TScalar& radius,
                 const TVec3& position,
                 const TMat3& rotation,
                 const eDynamicsType& dyntype = eDynamicsType::DYNAMIC,
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

    /// Cylinder primitive, corresponding to a single-body with a cylinder-shaped collider and drawable
    ///
    /// @details
    /// This helper class creates a single body with a cylinder-shaped collider, defined by its
    /// base-radius and its height. This body exposes both its radius and height as user modifiable
    /// parameters to the user, which can be changed at runtime
    class TCylinder : public TSingleBody
    {
    public :

        /// Constructs a cylinder primitive from user-specified radius and height
        ///
        /// @details
        /// Creates a single-body object with a cylinder collider and cylinder drawable, given the
        /// radius and height provided by the user. The cylinder is placed at the given position
        /// (xyz-coordinates) and with the given orientation (rotation-matrix), and the user has
        /// the ability to define its dynamics type (either DYNAMIC or STATIC), as well as its
        /// collision filtering flags (collision-group and collision-mask) which can be used for
        /// broadphase collision checking and exclusion accordingly
        ///
        /// @param name             Unique identifier of the primitive in the simulation
        /// @param radius           Radius of the cylinder-shaped collider|drawable
        /// @param height           Height of the cylinder-shaped collider|drawable
        /// @param position         Initial position of the body object (xyz position)
        /// @param rotation         Initial rotation of the body object (rotation matrix)
        /// @param dyntype          Dynamics type, either DYNAMIC or STATIC
        /// @param collision_group  Collision group used for collision filtering
        /// @param collision_mask   Collision mask used for collision filtering
        TCylinder( const std::string& name,
                   const TScalar& radius,
                   const TScalar& height,
                   const TVec3& position,
                   const TMat3& rotation,
                   const eDynamicsType& dyntype = eDynamicsType::DYNAMIC,
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

    /// Capsule primitive, corresponding to a single-body with a capsule-shaped collider and drawable
    ///
    /// @details
    /// This helper class creates a single body with a capsule-shaped collider, defined by its
    /// radius and its height (cylindrical portion). This body exposes both its radius and height as user modifiable
    /// parameters to the user, which can be changed at runtime
    class TCapsule : public TSingleBody
    {
    public :

        /// Constructs a capsule primitive from user-specified radius and height
        ///
        /// @details
        /// Creates a single-body object with a capsule collider and capsule drawable, given the
        /// radius and height (cylindrical portion) provided by the user. The capsule is placed at the given position
        /// (xyz-coordinates) and with the given orientation (rotation-matrix), and the user has
        /// the ability to define its dynamics type (either DYNAMIC or STATIC), as well as its
        /// collision filtering flags (collision-group and collision-mask) which can be used for
        /// broadphase collision checking and exclusion accordingly
        ///
        /// @param name             Unique identifier of the primitive in the simulation
        /// @param radius           Radius of the capsule-shaped collider|drawable
        /// @param height           Height of the capsule-shaped collider|drawable
        /// @param position         Initial position of the body object (xyz position)
        /// @param rotation         Initial rotation of the body object (rotation matrix)
        /// @param dyntype          Dynamics type, either DYNAMIC or STATIC
        /// @param collision_group  Collision group used for collision filtering
        /// @param collision_mask   Collision mask used for collision filtering
        TCapsule( const std::string& name,
                  const TScalar& radius,
                  const TScalar& height,
                  const TVec3& position,
                  const TMat3& rotation,
                  const eDynamicsType& dyntype = eDynamicsType::DYNAMIC,
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

    /// Ellipsoid primitive, corresponding to a single-body with a ellipsoid-shaped collider and drawable
    ///
    /// @details
    /// This helper class creates a single body with a ellipsoid-shaped collider, defined by its
    /// radii (radius along its 3 axes). This body exposes both its radii as a user modifiable
    /// parameter, which can be changed at runtime
    class TEllipsoid : public TSingleBody
    {
    public :

        /// Constructs an ellipsoid primitive from user-specified radii
        ///
        /// @details
        /// Creates a single-body object with an ellipsoid collider and ellipsoid drawable, given the
        /// radius and height (cylindrical portion) provided by the user. The ellipsoid is placed at the
        /// given position (xyz-coordinates) and with the given orientation (rotation-matrix), and the user has
        /// the ability to define its dynamics type (either DYNAMIC or STATIC), as well as its
        /// collision filtering flags (collision-group and collision-mask) which can be used for
        /// broadphase collision checking and exclusion accordingly
        ///
        /// @param name             Unique identifier of the primitive in the simulation
        /// @param radii            Radii (vector-3 with x,y,z radius) of the ellipsoid-shaped collider|drawable
        /// @param position         Initial position of the body object (xyz position)
        /// @param rotation         Initial rotation of the body object (rotation matrix)
        /// @param dyntype          Dynamics type, either DYNAMIC or STATIC
        /// @param collision_group  Collision group used for collision filtering
        /// @param collision_mask   Collision mask used for collision filtering
        TEllipsoid( const std::string& name,
                    const TVec3& radii,
                    const TVec3& position,
                    const TMat3& rotation,
                    const eDynamicsType& dyntype = eDynamicsType::DYNAMIC,
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

    /// Mesh primitive, corresponding to a single-body with a mesh collider and drawable
    ///
    /// @details
    /// This helper class creates a single body with a mesh collider, defined by its vertex data,
    /// which can be loaded from a file or from user-defined vertex-data. The mesh-collider implementation
    /// depends on the specific backend used, and could be the mesh's convex-hull or a tri-mesh. This body
    /// exposes its scale as a user modifiable parameter, which can be changed at runtime
    class TMesh : public TSingleBody
    {
    public :

        /// Constructs a mesh primitive from user-specified collider and drawable file resources
        ///
        /// @details
        /// Creates a single-body object with a mesh collider and mesh drawable, given the files
        /// provided by the user for both collider and drawable. Note that the mesh file for the
        /// collider should be .stl, as most backends require this format internally. The mesh-file
        /// for the drawable can be of any format supported by Assimp (.stl, .obj, .dae, ...). The mesh
        /// is placed at the given position (xyz-coordinates) and with the given orientation (rotation-matrix),
        /// and the user has the ability to define its dynamics type (either DYNAMIC or STATIC), as well as its
        /// collision filtering flags (collision-group and collision-mask) which can be used for
        /// broadphase collision checking and exclusion accordingly
        ///
        /// @param name                     Unique identifier of the primitive in the simulation
        /// @param mesh_collider_filepath   Absolute path to the .stl file to be loaded for the mesh-collider
        /// @param mesh_visual_filepath     Absolute path to a valid mesh file to be loaded for the mesh-drawable
        /// @param mesh_scale               Scale factor for the mesh collider and drawable
        /// @param position                 Initial position of the body object (xyz position)
        /// @param rotation                 Initial rotation of the body object (rotation matrix)
        /// @param dyntype                  Dynamics type, either DYNAMIC or STATIC
        /// @param collision_group          Collision group used for collision filtering
        /// @param collision_mask           Collision mask used for collision filtering
        TMesh( const std::string& name,
               const std::string& mesh_collider_filepath,
               const std::string& mesh_visual_filepath,
               const TScalar& mesh_scale,
               const TVec3& position,
               const TMat3& rotation,
               const eDynamicsType& dyntype = eDynamicsType::DYNAMIC,
               const int& collision_group = 1,
               const int& collision_mask = 1 );

        /// Constructs a mesh primitive from user-specified vertex data
        ///
        /// @details
        /// Creates a single-body object with a mesh collider and mesh drawable, given the vertex-data
        /// provided by the user (used for both collider and drawable). Note that the vertices are given
        /// in x,y,z coordinates, and each 3 contiguous vertices form a face (triangle). Note also that
        /// the faces are given in tris, each face consisting of a integer for indexing in the vertices buffer.
        /// The mesh is placed at the given position (xyz-coordinates) and with the given orientation (rotation-matrix),
        /// and the user has the ability to define its dynamics type (either DYNAMIC or STATIC), as well as its
        /// collision filtering flags (collision-group and collision-mask) which can be used for
        /// broadphase collision checking and exclusion accordingly
        ///
        /// @param name             Unique identifier of the primitive in the simulation
        /// @param mesh_vertices    Vertices data used for both mesh-collider and mesh-drawable
        /// @param mesh_faces       Faces data used for both mesh-collider and mesh-drawable
        /// @param mesh_scale       Scale factor for the mesh collider and drawable
        /// @param position         Initial position of the body object (xyz position)
        /// @param rotation         Initial rotation of the body object (rotation matrix)
        /// @param dyntype          Dynamics type, either DYNAMIC or STATIC
        /// @param collision_group  Collision group used for collision filtering
        /// @param collision_mask   Collision mask used for collision filtering
        TMesh( const std::string& name,
               const std::vector<float>& mesh_vertices,
               const std::vector<int>& mesh_faces,
               const TScalar& mesh_scale,
               const TVec3& position,
               const TMat3& rotation,
               const eDynamicsType& dyntype = eDynamicsType::DYNAMIC,
               const int& collision_group = 1,
               const int& collision_mask = 1 );

        TMesh( const TMesh& other ) = delete;

        TMesh& operator=( const TMesh& other ) = delete;

        ~TMesh() = default;

        /// Sets the scale of the mesh collider|drawable
        ///
        /// @param scale    Scale factor applied to the mesh collider|drawable
        void SetScale( const TScalar& scale );

        /// Sets the vertex-data for the mesh collider|drawable
        ///
        /// @param vertices     List of vertices, as triplets of triangles
        /// @param faces        List of indices for the mesh's triangles faces
        void SetVertexData( const std::vector<float>& vertices, const std::vector<int>& faces );

        /// Gets the scale of the mesh collider|drawable
        ///
        /// @return Scale factor of the mesh collider|drawable
        TScalar scale() const { return m_Scale; }

    private :

        /// Scale of the mesh collider|drawable
        TScalar m_Scale;
    };

    class TTriMesh : public TSingleBody
    {
    public :

        TTriMesh( const std::string& name,
                  const std::string& trimesh_collider_filepath,
                  const std::string& trimesh_visual_filepath,
                  const TScalar& trimesh_scale,
                  const TVec3& position,
                  const TMat3& rotation,
                  const int& collision_group = 0x7fffffff,
                  const int& collision_mask = 0x7fffffff );

        TTriMesh( const std::string& name,
                  const std::vector<float>& trimesh_vertices,
                  const std::vector<int>& trimesh_faces,
                  const TScalar& trimesh_scale,
                  const TVec3& position,
                  const TMat3& rotation,
                  const int& collision_group = 0x7fffffff,
                  const int& collision_mask = 0x7fffffff );

        TTriMesh( const TTriMesh& other ) = delete;

        TTriMesh& operator=( const TTriMesh& other ) = delete;

        ~TTriMesh() = default;

        /// Sets the scale of the triangular-mesh collider|drawable
        ///
        /// @param scale    Scale factor applied to the triangular-mesh collider|drawable
        void SetScale( const TScalar& scale );

        /// Gets the scale of the triangular-mesh collider|drawable
        ///
        /// @return Scale factor of the triangular-mesh collider|drawable
        TScalar scale() const { return m_Scale; }

    private :

        /// Scale of the mesh collider|drawable
        TScalar m_Scale;
    };

    /// Heightfield primitive, corresponding to a single-body with a heightfield collider|drawable
    ///
    /// @details
    /// This helper class creates a single body with a heightfield collider, defined by elevation data
    /// over a rectangular grid, which comes from user-defined data. This body exposes its heights as a
    /// user modifiable parameter, which can be changed at runtime
    class THeightfield : public TSingleBody
    {
    public :

        /// Constructs a heightfield primitive from user-specified elevation data
        ///
        /// @details
        /// Creates a single-body object with a heightfield collider and drawable, given the elevation-data
        /// provided by the user (used for both collider and drawable). Note that the elevation data is given
        /// in row-major order, i.e. the row's of the heightfield's base grid are stored in consecutive order
        /// in the elevation-data buffer. The heightfield's center is placed at the given position (xyz-coordinates)
        /// and with the given orientation (rotation-matrix), and the user has the ability to define its collision
        /// filtering flags (collision-group and collision-mask) which can be used for broadphase collision checking
        /// and exclusion accordingly. Note also that the dynamics type of heightfield primitives is always set to STATIC.
        ///
        /// @param name                 Unique identifier of the primitive in the simulation
        /// @param num_width_samples    X-axis resolution (ticks|steps|points) of the heightfield's base grid
        /// @param num_depth_samples    Y-axis resolution (ticks|steps|points) of the heightfield's base grid
        /// @param width_extent         X-axis extension of the heightfield's base grid
        /// @param depth_extent         Y-axis extension of the heightfield's base grid
        /// @param heights              User-defined elevation data for the heightfield
        /// @param position             Initial position of the body object (xyz position)
        /// @param rotation             Initial rotation of the body object (rotation matrix)
        /// @param collision_group      Collision group used for collision filtering
        /// @param collision_mask       Collision mask used for collision filtering
        THeightfield( const std::string& name,
                      const ssize_t& num_width_samples,
                      const ssize_t& num_depth_samples,
                      const TScalar& width_extent,
                      const TScalar& depth_extent,
                      const std::vector<float>& heights,
                      const TVec3& position,
                      const TMat3& rotation,
                      const int& collision_group = 0x7fffffff,
                      const int& collision_mask = 0x7fffffff );

        THeightfield( const THeightfield& other ) = delete;

        THeightfield& operator=( const THeightfield& other ) = delete;

        ~THeightfield() = default;

        /// Sets the elevation data for this heightfield
        ///
        /// @param heights  Elevation data for this heightfield
        void SetHeights( const std::vector<float>& heights );

        /// Gets the current elevation data for this heightfield
        ///
        /// @return Current elevation data for this heightfield
        std::vector<float> heights() const { return m_Heights; }

        /// Gets the number of ticks|steps (resolution) of the heightfield's base grid in the x-axis
        ///
        /// @return Grid resolution of the heightfield in the x-axis
        ssize_t num_width_samples() const { return m_NumWidthSamples; }

        /// Gets the number of ticks|steps (resolution) of the heightfield's base grid in the y-axis
        ///
        /// @return Grid resolution of the heightfield in the y-axis
        ssize_t num_depth_samples() const { return m_NumDepthSamples; }

        /// Gets the X-extension of the base of the heightfield (width)
        ///
        /// @return Width of the base of the heightfield (x-axis)
        TScalar width_extent() const { return m_WidthExtent; }

        /// Gets the Y-extension of the base of the heightfield (width)
        ///
        /// @return Depth of the base of the heightfield (y-axis)
        TScalar depth_extent() const { return m_DepthExtent; }

    private :

        /// X-axis extension of the heightfield's base grid
        TScalar m_WidthExtent;
        /// Y-axis extension of the heightfield's base grid
        TScalar m_DepthExtent;
        /// X-axis resolution of the heightfield's base grid
        ssize_t m_NumWidthSamples;
        /// Y-axis resolution of the heightfield's base grid
        ssize_t m_NumDepthSamples;
        /// Elevation data of the heightfield
        std::vector<float> m_Heights;
    };
}}