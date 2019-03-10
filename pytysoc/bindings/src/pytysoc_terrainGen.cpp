
#include <pytysoc_terrainGen.h>

namespace pytysoc
{

    PyTerrainGen::PyTerrainGen( const std::string& name )
    {
        m_terrainGenPtr = NULL;
    }

    PyTerrainGen::~PyTerrainGen()
    {
        m_terrainGenPtr = NULL;
    }

    std::string PyTerrainGen::name()
    {
        if ( m_terrainGenPtr )
            return m_terrainGenPtr->name();

        return "undefined";
    }

    tysoc::terrain::TITerrainGenerator* PyTerrainGen::ptr()
    {
        return m_terrainGenPtr;
    }

    PyStaticTerrainGen::PyStaticTerrainGen( const std::string& name )
        : PyTerrainGen( name )
    {
        m_terrainGenPtr = new tysoc::terrain::TStaticTerrainGenerator( name );
    }

    void PyStaticTerrainGen::createPrimitive( const std::string& type,
                                              py::array_t<TScalar>& size,
                                              py::array_t<TScalar>& position,
                                              py::array_t<TScalar>& rotation,
                                              py::array_t<TScalar>& rgb,
                                              const std::string& texturename,
                                              const std::string& filename )
    {
        if ( !m_terrainGenPtr )
            return;

        reinterpret_cast< tysoc::terrain::TStaticTerrainGenerator* >( m_terrainGenPtr )
                        ->createPrimitive( type,
                                           numpyToVec3( size ),
                                           numpyToVec3( position ),
                                           tysoc::TMat3::fromEuler( numpyToVec3( rotation ) ),
                                           numpyToVec3( rgb ),
                                           texturename,
                                           filename );
    }
}