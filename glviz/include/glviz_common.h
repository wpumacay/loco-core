
#pragma once

#include <LCommon.h>
#include <tysoc_common.h>

namespace tysoc {

    engine::LMat4 fromTMat4( const TMat4& mat );
    engine::LMat4 fromTMat3( const TMat3& mat );
    engine::LVec3 fromTVec3( const TVec3& vec );

}