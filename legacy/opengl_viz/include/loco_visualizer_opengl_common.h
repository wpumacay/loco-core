#pragma once

#include <loco_data.h>
#include <visualizer/loco_visualizer_camera.h>
#include <visualizer/loco_visualizer_light.h>
#include <CEngine.h>

namespace loco {
namespace visualizer {

    const float HFIELD_HEIGHT_BASE = 1.0f;

    std::unique_ptr<engine::CICamera> CreateCamera( TVizCamera* vizCameraRef );
    std::unique_ptr<engine::CILight> CreateLight( TVizLight* vizLightRef );
    std::unique_ptr<engine::CIRenderable> CreateRenderable( const TShapeData& data );
    std::unique_ptr<engine::CMesh> CreateMeshFromUserData( const std::vector<float>& vertices,
                                                           const std::vector<int>& indices );
    void CreateMeshVertexDataFromUserData( const std::vector<float>& vertices,
                                           const std::vector<int>& indices,
                                           std::vector<engine::CVec3>& dst_vertices_unique,
                                           std::vector<engine::CVec3>& dst_vertices,
                                           std::vector<engine::CVec3>& dst_normals,
                                           std::vector<engine::CVec2>& dst_texcoords,
                                           std::vector<engine::CInd3>& dst_faces );

}}