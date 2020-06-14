
#include <primitives/loco_single_body_primitives.h>

namespace loco {
namespace primitives {

    //********************************************************************************************//
    //                                Box primitive Implementation                                //
    //********************************************************************************************//

    TBox::TBox( const std::string& name,
                const TVec3& extents,
                const TVec3& position,
                const TMat3& rotation,
                const eDynamicsType& dyntype,
                const int& collision_group,
                const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::BOX;
        collider_data.size = extents;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::BOX;
        visual_data.size = extents;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = dyntype;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_Extents = extents;
    }

    void TBox::SetExtents( const TVec3& extents )
    {
        m_Extents = extents;
        m_Collider->ChangeSize( m_Extents );
        m_Drawable->ChangeSize( m_Extents );
    }

    void TBox::SetWidth( const TScalar& width )
    {
        m_Extents.x() = width;
        m_Collider->ChangeSize( m_Extents );
        m_Drawable->ChangeSize( m_Extents );
    }

    void TBox::SetDepth( const TScalar& depth )
    {
        m_Extents.y() = depth;
        m_Collider->ChangeSize( m_Extents );
        m_Drawable->ChangeSize( m_Extents );
    }

    void TBox::SetHeight( const TScalar& height )
    {
        m_Extents.z() = height;
        m_Collider->ChangeSize( m_Extents );
        m_Drawable->ChangeSize( m_Extents );
    }

    //********************************************************************************************//
    //                               Plane primitive Implementation                               //
    //********************************************************************************************//

    TPlane::TPlane( const std::string& name,
                    const TScalar& width,
                    const TScalar& depth,
                    const TVec3& position,
                    const TMat3& rotation,
                    const int& collision_group,
                    const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::PLANE;
        collider_data.size = { width, depth, 0.0f };
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::PLANE;
        visual_data.size = { width, depth, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = eDynamicsType::STATIC;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_Width = width;
        m_Depth = depth;
    }

    void TPlane::SetWidth( const TScalar& width )
    {
        m_Width = width;
        m_Collider->ChangeSize( { m_Width, m_Depth, 0.0f } );
        m_Drawable->ChangeSize( { m_Width, m_Depth, 0.0f } );
    }

    void TPlane::SetDepth( const TScalar& depth )
    {
        m_Depth = depth;
        m_Collider->ChangeSize( { m_Width, m_Depth, 0.0f } );
        m_Collider->ChangeSize( { m_Width, m_Depth, 0.0f } );
    }

    //********************************************************************************************//
    //                               Sphere primitive Implementation                              //
    //********************************************************************************************//

    TSphere::TSphere( const std::string& name,
                      const TScalar& radius,
                      const TVec3& position,
                      const TMat3& rotation,
                      const eDynamicsType& dyntype,
                      const int& collision_group,
                      const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::SPHERE;
        collider_data.size = { radius, 0.0f, 0.0f };
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::SPHERE;
        visual_data.size = { radius, 0.0f, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = dyntype;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_Radius = radius;
    }

    void TSphere::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Collider->ChangeSize( { m_Radius, 0.0f, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, 0.0f, 0.0f } );
    }

    //********************************************************************************************//
    //                              Cylinder primitive Implementation                             //
    //********************************************************************************************//

    TCylinder::TCylinder( const std::string& name,
                          const TScalar& radius,
                          const TScalar& height,
                          const TVec3& position,
                          const TMat3& rotation,
                          const eDynamicsType& dyntype,
                          const int& collision_group,
                          const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::CYLINDER;
        collider_data.size = { radius, height, 0.0f };
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::CYLINDER;
        visual_data.size = { radius, height, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = dyntype;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_Radius = radius;
        m_Height = height;
    }

    void TCylinder::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Collider->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    void TCylinder::SetHeight( const TScalar& height )
    {
        m_Height = height;
        m_Collider->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    //********************************************************************************************//
    //                              Capsule primitive Implementation                              //
    //********************************************************************************************//

    TCapsule::TCapsule( const std::string& name,
                        const TScalar& radius,
                        const TScalar& height,
                        const TVec3& position,
                        const TMat3& rotation,
                        const eDynamicsType& dyntype,
                        const int& collision_group,
                        const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::CAPSULE;
        collider_data.size = { radius, height, 0.0f };
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::CAPSULE;
        visual_data.size = { radius, height, 0.0f };
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = dyntype;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_Radius = radius;
        m_Height = height;
    }

    void TCapsule::SetRadius( const TScalar& radius )
    {
        m_Radius = radius;
        m_Collider->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    void TCapsule::SetHeight( const TScalar& height )
    {
        m_Height = height;
        m_Collider->ChangeSize( { m_Radius, m_Height, 0.0f } );
        m_Drawable->ChangeSize( { m_Radius, m_Height, 0.0f } );
    }

    //********************************************************************************************//
    //                            Ellipsoid primitive Implementation                              //
    //********************************************************************************************//

    TEllipsoid::TEllipsoid( const std::string& name,
                            const TVec3& radii,
                            const TVec3& position,
                            const TMat3& rotation,
                            const eDynamicsType& dyntype,
                            const int& collision_group,
                            const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::ELLIPSOID;
        collider_data.size = radii;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::ELLIPSOID;
        visual_data.size = radii;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = dyntype;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_Radii = radii;
    }

    void TEllipsoid::SetRadii( const TVec3& radii )
    {
        m_Radii = radii;
        m_Collider->ChangeSize( m_Radii );
        m_Drawable->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusX( const TScalar& radius_x )
    {
        m_Radii.x() = radius_x;
        m_Collider->ChangeSize( m_Radii );
        m_Drawable->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusY( const TScalar& radius_y )
    {
        m_Radii.y() = radius_y;
        m_Collider->ChangeSize( m_Radii );
        m_Drawable->ChangeSize( m_Radii );
    }

    void TEllipsoid::SetRadiusZ( const TScalar& radius_z )
    {
        m_Radii.z() = radius_z;
        m_Collider->ChangeSize( m_Radii );
        m_Drawable->ChangeSize( m_Radii );
    }

    //********************************************************************************************//
    //                               Mesh primitive Implementation                                //
    //********************************************************************************************//

    TMesh::TMesh( const std::string& name,
                  const std::string& mesh_collider_filepath,
                  const std::string& mesh_visual_filepath,
                  const TScalar& mesh_scale,
                  const TVec3& position,
                  const TMat3& rotation,
                  const eDynamicsType& dyntype,
                  const int& collision_group,
                  const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::MESH;
        collider_data.size = { mesh_scale, mesh_scale, mesh_scale };
        collider_data.mesh_data.filename = mesh_collider_filepath;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::MESH;
        visual_data.size = { mesh_scale, mesh_scale, mesh_scale };
        visual_data.mesh_data.filename = mesh_visual_filepath;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = dyntype;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_Scale = mesh_scale;
    }

    TMesh::TMesh( const std::string& name,
                  const std::vector<float>& mesh_vertices,
                  const std::vector<int>& mesh_faces,
                  const TScalar& mesh_scale,
                  const TVec3& position,
                  const TMat3& rotation,
                  const eDynamicsType& dyntype,
                  const int& collision_group,
                  const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::MESH;
        collider_data.size = { mesh_scale, mesh_scale, mesh_scale };
        collider_data.mesh_data.vertices = mesh_vertices;
        collider_data.mesh_data.faces = mesh_faces;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::MESH;
        visual_data.size = { mesh_scale, mesh_scale, mesh_scale };
        visual_data.mesh_data.vertices = mesh_vertices;
        visual_data.mesh_data.faces = mesh_faces;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = dyntype;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_Scale = mesh_scale;
    }

    void TMesh::SetScale( const TScalar& scale )
    {
        m_Scale = scale;
        m_Collider->ChangeSize( { m_Scale, m_Scale, m_Scale } );
        m_Drawable->ChangeSize( { m_Scale, m_Scale, m_Scale } );
    }

    void TMesh::SetVertexData( const std::vector<float>& vertices, const std::vector<int>& faces )
    {
        m_Collider->ChangeVertexData( vertices, faces );
        m_Drawable->ChangeVertexData( vertices, faces );
    }

    //********************************************************************************************//
    //                            Heightfield primitive Implementation                            //
    //********************************************************************************************//

    THeightfield::THeightfield( const std::string& name,
                                const ssize_t& num_width_samples,
                                const ssize_t& num_depth_samples,
                                const TScalar& width_extent,
                                const TScalar& depth_extent,
                                const std::vector<float>& heights,
                                const TVec3& position,
                                const TMat3& rotation,
                                const int& collision_group,
                                const int& collision_mask )
        : TSingleBody( name, position, rotation )
    {
        auto collider_data = TCollisionData();
        collider_data.type = eShapeType::HFIELD;
        collider_data.size = { width_extent, depth_extent, 1.0f };
        collider_data.hfield_data.nWidthSamples = num_width_samples;
        collider_data.hfield_data.nDepthSamples = num_depth_samples;
        collider_data.hfield_data.heights = heights;
        collider_data.collisionGroup = collision_group;
        collider_data.collisionMask = collision_mask;
        m_Collider = std::make_unique<TSingleBodyCollider>( name + loco::SUFFIX_COLLIDER, collider_data );
        m_Collider->SetParentBody( this );

        auto visual_data = TVisualData();
        visual_data.type = eShapeType::HFIELD;
        visual_data.size = { width_extent, depth_extent, 1.0f };
        visual_data.hfield_data.nWidthSamples = num_width_samples;
        visual_data.hfield_data.nDepthSamples = num_depth_samples;
        visual_data.hfield_data.heights = heights;
        visual_data.ambient = loco::DEFAULT_AMBIENT_COLOR;
        visual_data.diffuse = loco::DEFAULT_DIFFUSE_COLOR;
        visual_data.specular = loco::DEFAULT_SPECULAR_COLOR;
        visual_data.shininess = loco::DEFAULT_SHININESS;
        m_Drawable = std::make_unique<visualizer::TDrawable>( name + loco::SUFFIX_DRAWABLE, visual_data );
        m_Drawable->SetParentObject( this );

        m_Data.dyntype = eDynamicsType::STATIC;
        m_Data.collision = collider_data;
        m_Data.visual = visual_data;

        m_WidthExtent = width_extent;
        m_DepthExtent = depth_extent;
        m_NumWidthSamples = num_width_samples;
        m_NumDepthSamples = num_depth_samples;
        m_Heights = heights;
    }

    void THeightfield::SetHeights( const std::vector<float>& heights )
    {
        memcpy( m_Heights.data(), heights.data(), sizeof( float ) * heights.size() );
        m_Collider->ChangeElevationData( m_Heights );
        m_Drawable->ChangeElevationData( m_Heights );
    }
}}