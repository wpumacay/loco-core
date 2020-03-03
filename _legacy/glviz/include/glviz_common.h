
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <locosim_common.h>
#include <components/data.h>

#include <CEngine.h>

#define GLVIZ_DEFAULT_HFIELD_BASE 0.1f

namespace tysoc {

    TMat4 toTMat4( const engine::CMat4& mat );

    engine::CMat4 fromTMat4( const TMat4& mat );
    engine::CMat4 fromTMat3( const TMat3& mat );
    engine::CVec3 fromTVec3( const TVec3& vec );
    engine::CVec4 fromTVec4( const TVec4& vec );

    engine::CIRenderable* createRenderable( const eShapeType& type,
                                            const TVec3& size,
                                            const std::string& filename = "" );

    engine::CIRenderable* createRenderable( const TShapeData& data );

    void setRenderableColor( engine::CIRenderable* renderable,
                             const TVec3& cAmbient,
                             const TVec3& cDiffuse,
                             const TVec3& cSpecular );

    void setRenderableTexture( engine::CIRenderable* renderable,
                               engine::CTexture* albedoTex );

    void resizeRenderable( engine::CIRenderable* renderable,
                           const eShapeType& type,
                           const TVec3& baseSize,
                           const TVec3& newSize );

}