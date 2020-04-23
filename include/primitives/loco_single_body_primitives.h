#pragma once

#include <primitives/loco_single_body.h>

namespace loco
{
    /// Box primitive, corresponding to a single-body with box-shaped collider and drawable
    ///
    /// @details
    /// This helper class creates a single body with a box-shaped collider, defined
    /// by its x-y-z extents (width|depth|height). This body exposes its extents as
    /// user modifiable parameters to the user, which can also be changed at runtime
    class TBox : public TSingleBody
    {
    public :

        /// Constructs a box primitive from its extents (width|depth|height)
        ///
        /// Creates a single-body object with a box collider, given width (x-extent),
        /// depth (y-extent), and height (z-extent) provided by the user.
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
    /// parameters to the user, which can also be changed at runtime
    class TPlane : public TSingleBody
    {
    public :

        /// Constructs a plane primitive from user-specified width and depth
        ///
        /// Creates a single-body object with a plane collider, given width (x-extent),
        /// and depth (y-extent) provided by the user. The plane is placed at the given
        /// position (xyz-coordinates) and with the given orientation (rotation-matrix),
        /// and the user has the ability to define its collision filtering flags (group and mask)
        /// The resulting primitive is always a STATIC single-body object (dynamics won't affect it)
        /// 
        /// @param name             Unique identifier of the primitive in the simulation
        /// @param extents          Vector containing the witdh, depth, and height for the box collider
        /// @param position         Initial position of the body object (xyz position)
        /// @param rotation         Initial rotation of the body object (rotation matrix)
        /// @param collision_group  Collision group used for collision filtering
        /// @param collision_mask   Collision mask used for collision filtering
        TPlane( const std::string& name,
                const TScalar& width,
                const TScalar& depth,
                const TVec3& position,
                const TMat3& rotation,
                const int& collision_group = 1,
                const int& collision_mask = 1 );

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
    /// user, which can also be changed at runtime
    class TSphere : public TSingleBody
    {
    public :

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

        void SetRadius( const TScalar& radius );

        TScalar radius() const { return m_Radius; }

    private :

        /// Radius of the sphere-shapes collider|drawable
        TScalar m_Radius;
    };

    class TCylinder : public TSingleBody
    {
    public :

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

        void SetRadius( const TScalar& radius );

        void SetHeight( const TScalar& height );

        TScalar radius() const { return m_Radius; }

        TScalar height() const { return m_Height; }

    private :

        TScalar m_Radius;

        TScalar m_Height;
    };

    class TCapsule : public TSingleBody
    {
    public :

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

        void SetRadius( const TScalar& radius );

        void SetHeight( const TScalar& height );

        TScalar radius() const;

        TScalar height() const;

    private :

        TScalar m_Radius;

        TScalar m_Height;
    };

    class TEllipsoid : public TSingleBody
    {
    public :

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

        void SetRadii( const TVec3& radii );

        void SetRadiusX( const TScalar& radius_x );

        void SetRadiusY( const TScalar& radius_y );

        void SetRadiusZ( const TScalar& radius_z );

        TVec3 radii() const { return m_Radii; }

        TScalar radius_x() const { return m_Radii.x(); }

        TScalar radius_y() const { return m_Radii.y(); }

        TScalar radius_z() const { return m_Radii.z(); }

    private :

        TVec3 m_Radii;
    };

    class TMesh : public TSingleBody
    {
    public :

        TMesh( const std::string& name,
               const std::string& mesh_collider_filepath,
               const std::string& mesh_visual_filepath,
               const TScalar& mesh_scale,
               const TVec3& position,
               const TMat3& rotation,
               const eDynamicsType& dyntype = eDynamicsType::DYNAMIC,
               const int& collision_group = 1,
               const int& collision_mask = 1 );

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

        void SetScale( const TScalar& scale );

        TScalar scale() const { return m_Scale; }

    private :

        TScalar m_Scale;
    };

    class THeightfield : public TSingleBody
    {
    public :

        THeightfield( const std::string& name,
                      const ssize_t& num_width_samples,
                      const ssize_t& num_depth_samples,
                      const TScalar& width_extent,
                      const TScalar& depth_extent,
                      const std::vector<float>& heights,
                      const TVec3& position,
                      const TMat3& rotation,
                      const int& collision_group = 1,
                      const int& collision_mask = 1 );

        THeightfield( const THeightfield& other ) = delete;

        THeightfield& operator=( const THeightfield& other ) = delete;

        ~THeightfield() = default;

        void SetHeights( const std::vector<float>& heights );

        std::vector<float> heights() const { return m_Heights; }

        ssize_t num_width_samples() const { return m_NumWidthSamples; }

        ssize_t num_depth_samples() const { return m_NumDepthSamples; }

        TScalar width_extent() const { return m_WidthExtent; }

        TScalar depth_extent() const { return m_DepthExtent; }

    private :

        TScalar m_WidthExtent;

        TScalar m_DepthExtent;

        ssize_t m_NumWidthSamples;

        ssize_t m_NumDepthSamples;

        std::vector<float> m_Heights;
    };
}