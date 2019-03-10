
#pragma once

#include <terrain/terrain_base.h>

#define DEFAULT_STATIC_PRIMITIVE_COLOR tysoc::TVec3( 0.2, 0.3, 0.4 )

namespace tysoc {
namespace terrain {

    class TStaticTerrainGenerator : public TITerrainGenerator
    {

        public :

        TStaticTerrainGenerator( const std::string& name );
        ~TStaticTerrainGenerator();

        TTerrainPrimitive* createPrimitive( const std::string& type,
                                            const TVec3& size,
                                            const TVec3& position = TVec3(), // (0,0,0) by default
                                            const TMat3& rotation = TMat3(), // Identity by default
                                            const TVec3& rgb = DEFAULT_STATIC_PRIMITIVE_COLOR,
                                            const std::string& texturename = "",
                                            const std::string& filename = "" );
        void initialize() override;
        void update() override;


    };

}}