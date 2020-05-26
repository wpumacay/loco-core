
#include <loco_visualizer_opengl_common.h>

namespace loco {
namespace glviz {

    std::unique_ptr<engine::CICamera> CreateCamera( TVizCamera* vizCameraRef )
    {
        std::unique_ptr<engine::CICamera> camera = nullptr;

        auto cameraProjData = engine::CCameraProjData();
        cameraProjData.fov = 45.0f;
        cameraProjData.aspect = engine::CApplication::GetInstance()->window()->aspect();
        cameraProjData.zNear = 0.1f;
        cameraProjData.zFar = 50.0f;

        switch ( vizCameraRef->type() )
        {
            case loco::eVizCameraType::ORBIT :
            {
                camera = std::make_unique<engine::COrbitCamera>( vizCameraRef->name(),
                                                                 vizCameraRef->position(),
                                                                 vizCameraRef->target(),
                                                                 engine::eAxis::Z,
                                                                 cameraProjData,
                                                                 engine::CApplication::GetInstance()->window()->width(),
                                                                 engine::CApplication::GetInstance()->window()->height() );
                break;
            }
            case loco::eVizCameraType::FPS :
            {
                const float cameraSensitivity = 0.1f;
                const float cameraSpeed = 50.0f;
                const float cameraMaxDelta = 10.0f;

                camera = std::make_unique<engine::CFpsCamera>( vizCameraRef->name(),
                                                               vizCameraRef->position(),
                                                               vizCameraRef->target(),
                                                               engine::eAxis::Z,
                                                               cameraProjData,
                                                               cameraSensitivity,
                                                               cameraSpeed,
                                                               cameraMaxDelta );
                break;
            }
            case loco::eVizCameraType::FIXED :
            {
                camera = std::make_unique<engine::CFixedCamera>( vizCameraRef->name(),
                                                                 vizCameraRef->position(),
                                                                 vizCameraRef->target(),
                                                                 engine::eAxis::Z,
                                                                 cameraProjData );
                break;
            }
        }

        return std::move( camera );
    }

    std::unique_ptr<engine::CILight> CreateLight( TVizLight* vizLightRef )
    {
        std::unique_ptr<engine::CILight> light = nullptr;

        switch ( vizLightRef->type() )
        {
            case loco::eVizLightType::DIRECTIONAL :
            {
                light = std::make_unique<engine::CDirectionalLight>( vizLightRef->name(),
                                                                     vizLightRef->ambient(),
                                                                     vizLightRef->diffuse(),
                                                                     vizLightRef->specular(),
                                                                     vizLightRef->direction() );
                break;
            }
            case loco::eVizLightType::POINT :
            {
                const float attenuationFactorConstant = 1.0f;
                const float attenuationFactorLinear = 0.01f;
                const float attenuationFactorQuadratic = 0.0001f;

                light = std::make_unique<engine::CPointLight>( vizLightRef->name(),
                                                               vizLightRef->ambient(),
                                                               vizLightRef->diffuse(),
                                                               vizLightRef->specular(),
                                                               vizLightRef->position(),
                                                               attenuationFactorConstant,
                                                               attenuationFactorLinear,
                                                               attenuationFactorQuadratic );
                break;
            }
            case loco::eVizLightType::SPOT :
            {
                const float attenuationFactorConstant = 1.0f;
                const float attenuationFactorLinear = 0.01f;
                const float attenuationFactorQuadratic = 0.0001f;

                light = std::make_unique<engine::CSpotLight>( vizLightRef->name(),
                                                              vizLightRef->ambient(),
                                                              vizLightRef->diffuse(),
                                                              vizLightRef->specular(),
                                                              vizLightRef->position(),
                                                              vizLightRef->direction(),
                                                              attenuationFactorConstant,
                                                              attenuationFactorLinear,
                                                              attenuationFactorQuadratic,
                                                              vizLightRef->innerCutoff(),
                                                              vizLightRef->outerCutoff() );
                break;
            }
        }

        return std::move( light );
    }

    std::unique_ptr<engine::CIRenderable> CreateRenderable( const TShapeData& data )
    {
        std::unique_ptr<engine::CIRenderable> renderable = nullptr;

        switch ( data.type )
        {
            case loco::eShapeType::PLANE :
            {
                renderable = engine::CMeshBuilder::createPlane( data.size.x(), data.size.y(), engine::eAxis::Z );
                break;
            }
            case loco::eShapeType::BOX :
            {
                renderable = engine::CMeshBuilder::createBox( data.size.x(), data.size.y(), data.size.z() );
                break;
            }
            case loco::eShapeType::SPHERE :
            {
                renderable = engine::CMeshBuilder::createSphere( data.size.x() );
                break;
            }
            case loco::eShapeType::CYLINDER :
            {
                renderable = engine::CMeshBuilder::createCylinder( data.size.x(), data.size.y(), engine::eAxis::Z );
                break;
            }
            case loco::eShapeType::CAPSULE :
            {
                renderable = std::make_unique<engine::CModel>( "capsule_compmesh" );
                static_cast<engine::CModel*>( renderable.get() )->addMesh( engine::CMeshBuilder::createCylinder( data.size.x(), data.size.y(), engine::eAxis::Z ),
                                                                           engine::CMat4() /* set cylindrical part in the center */ );
                static_cast<engine::CModel*>( renderable.get() )->addMesh( engine::CMeshBuilder::createSphere( data.size.x() ),
                                                                           engine::CMat4( engine::CMat3(), { 0.0f, 0.0f, 0.5f * data.size.y() } ) );
                static_cast<engine::CModel*>( renderable.get() )->addMesh( engine::CMeshBuilder::createSphere( data.size.x() ),
                                                                           engine::CMat4( engine::CMat3(), { 0.0f, 0.0f, -0.5f * data.size.y() } ) );
                break;
            }
            case loco::eShapeType::ELLIPSOID :
            {
                renderable = engine::CMeshBuilder::createEllipsoid( data.size.x(), data.size.y(), data.size.z() );
                break;
            }
            case loco::eShapeType::MESH :
            {
                auto& mesh_data = data.mesh_data;
                if ( mesh_data.filename != "" )
                    renderable = engine::CMeshBuilder::createModelFromFile( mesh_data.filename );
                else if ( mesh_data.vertices.size() > 0 && mesh_data.faces.size() > 0 )
                    renderable = CreateMeshFromData( mesh_data.vertices, mesh_data.faces );
                else
                    LOCO_CORE_ERROR( "CreateRenderable >>> couldn't create mesh-renderable (no filename, no data)" );
                if ( renderable )
                    renderable->scale = { data.size.x(), data.size.y(), data.size.z() };
                break;
            }
            case loco::eShapeType::HFIELD :
            {
                // Unnormalize the heights data (use data.size.z() as scaler)
                auto heightsScaled = data.hfield_data.heights;
                for ( size_t i = 0; i < heightsScaled.size(); i++ )
                    heightsScaled[i] *= data.size.z();

                renderable = engine::CMeshBuilder::createHeightField( data.hfield_data.nWidthSamples,
                                                                      data.hfield_data.nDepthSamples,
                                                                      data.size.x(), data.size.y(),
                                                                      data.size.x() / 2.0f,
                                                                      data.size.y() / 2.0f,
                                                                      heightsScaled,
                                                                      HFIELD_HEIGHT_BASE,
                                                                      engine::eAxis::Z );
                break;
            }
        }

        return std::move( renderable );
    }

    std::unique_ptr<engine::CMesh> CreateMeshFromData( const std::vector<float>& vertices,
                                                       const std::vector<int>& indices )
    {
        if ( vertices.size() % 3 != 0 )
            LOCO_CORE_ERROR( "CreateMeshFromData >>> there must be 3 elements per vertex" );
        if ( indices.size() % 3 != 0 )
            LOCO_CORE_ERROR( "CreateMeshFromData >>> there must be 3 elements per face" );

        const size_t num_faces = indices.size() / 3;
        const size_t num_vertices = num_faces * 3;
        const size_t num_vertices_unique = vertices.size() / 3;
        std::vector<engine::CVec3> mesh_vertices_unique( num_vertices_unique );
        std::vector<engine::CVec3> mesh_vertices( num_vertices );
        std::vector<engine::CVec3> mesh_normals( num_vertices );
        std::vector<engine::CVec2> mesh_texcoords( num_vertices );
        std::vector<engine::CInd3> mesh_faces( num_faces );

        for ( size_t v = 0; v < num_vertices_unique; v++ )
            mesh_vertices_unique[v] = { vertices[3 * v + 0], vertices[3 * v + 1], vertices[3 * v + 2] };

        for ( size_t f = 0; f < num_faces; f++ )
        {
            mesh_faces[f] = { (int)(3 * f + 0), (int)(3 * f + 1), (int)(3 * f + 2) };
            // Compute the normals (flat-normals, assuming winding order is 0-1-2 <=> out-normal )
            if ( indices[3 * f + 0] < num_vertices_unique &&
                 indices[3 * f + 1] < num_vertices_unique &&
                 indices[3 * f + 2] < num_vertices_unique )
            {
                mesh_vertices[3 * f + 0] = mesh_vertices_unique[indices[3 * f + 0]];
                mesh_vertices[3 * f + 1] = mesh_vertices_unique[indices[3 * f + 1]];
                mesh_vertices[3 * f + 2] = mesh_vertices_unique[indices[3 * f + 2]];

                auto v0 = mesh_vertices[3 * f + 0];
                auto v1 = mesh_vertices[3 * f + 1];
                auto v2 = mesh_vertices[3 * f + 2];

                const auto flat_normal = tinymath::cross( v1 - v0, v2 - v0 ).normalized();
                mesh_normals[3 * f + 0] = flat_normal;
                mesh_normals[3 * f + 1] = flat_normal;
                mesh_normals[3 * f + 2] = flat_normal;

                mesh_texcoords[3 * f + 0] = { 0.0f, 0.0f };
                mesh_texcoords[3 * f + 1] = { 0.0f, 0.0f };
                mesh_texcoords[3 * f + 2] = { 0.0f, 0.0f };
            }
            else
            {
                LOCO_CORE_ERROR( "CreateMeshFromData >>> one of the given indices is out of range" );
            }
        }

        static size_t num_meshes = 0;
        auto mesh = std::make_unique<engine::CMesh>( "mesh_" + std::to_string( num_meshes++ ),
                                                     mesh_vertices, mesh_normals, mesh_texcoords, mesh_faces );
        return std::move( mesh );
    }
}}