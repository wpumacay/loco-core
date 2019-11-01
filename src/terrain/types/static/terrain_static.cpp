
#include <terrain/types/static/terrain_static.h>

namespace tysoc {
namespace terrain {

    TStaticTerrainGenerator::TStaticTerrainGenerator( const std::string& name )
        : TITerrainGenerator( name )
    {
        // do nothing
    }

    TStaticTerrainGenerator::~TStaticTerrainGenerator()
    {
        // do nothing, parent destructor should already do the job
    }

    TTerrainPrimitive* TStaticTerrainGenerator::createPrimitive( const std::string& type,
                                                                 const TVec3& size,
                                                                 const TVec3& position,
                                                                 const TMat3& rotation,
                                                                 const TVec3& rgb,
                                                                 const std::string& texturename,
                                                                 const std::string& filename )
    {
        auto _terrainPrimitivePtr = new TTerrainPrimitive();
        // set type as static, for potential checkings later
        _terrainPrimitivePtr->type = "static";
        // and the type of primitive to use (plane|box|sphere|capsule|cylinder|mesh)
        _terrainPrimitivePtr->geomType = type;
        // and the texturename
        _terrainPrimitivePtr->texturename = texturename;
        // and the filename (meshfile or heightmap)
        _terrainPrimitivePtr->filename = filename;
        // and the size of the primitive
        _terrainPrimitivePtr->size = size;
        // also the position
        _terrainPrimitivePtr->pos = position;
        // the rotation as well
        _terrainPrimitivePtr->rotmat = rotation;
        // set it as already in use (unlike in-pool)
        _terrainPrimitivePtr->inUse = true;
        // force to use the color given from the user
        _terrainPrimitivePtr->useCustomColor = true;
        _terrainPrimitivePtr->color = rgb;
        // and a approx bound of the primitive (radius of a bounding sphere)
        _terrainPrimitivePtr->rbound = TVec3::length( size );

        m_primitives.push_back( _terrainPrimitivePtr );
        m_fixed.push( _terrainPrimitivePtr );

        return _terrainPrimitivePtr;
    }

    void TStaticTerrainGenerator::initialize()
    {
        // do nothing
    }

    void TStaticTerrainGenerator::update()
    {
        // do nothing
    }



}}