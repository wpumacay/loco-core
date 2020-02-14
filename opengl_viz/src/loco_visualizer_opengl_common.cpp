
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
                renderable = engine::CMeshBuilder::createCapsule( data.size.x(), data.size.y(), engine::eAxis::Z );
                break;
            }
            case loco::eShapeType::ELLIPSOID :
            {
                renderable = engine::CMeshBuilder::createEllipsoid( data.size.x(), data.size.y(), data.size.z() );
                break;
            }
            case loco::eShapeType::MESH :
            {
                renderable = engine::CMeshBuilder::createModelFromFile( data.filename );
                break;
            }
            case loco::eShapeType::HFIELD :
            {
                // Unnormalize the heights data (use data.size.z() as scaler)
                auto heightsScaled = data.hdata.heights;
                for ( size_t i = 0; i < heightsScaled.size(); i++ )
                    heightsScaled[i] *= data.size.z();

                renderable = engine::CMeshBuilder::createHeightField( data.hdata.nWidthSamples,
                                                                      data.hdata.nDepthSamples,
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
}}