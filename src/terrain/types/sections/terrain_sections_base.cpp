
#include <terrain/types/sections/terrain_sections_base.h>

namespace tysoc {
namespace terrain {

    TSectionsTerrainGenerator::TSectionsTerrainGenerator( const std::string& name,
                                                          float startX,
                                                          float startY,
                                                          float startZ,
                                                          float sectionDepth )
        : TITerrainGenerator( name )
    {
        m_startX = startX;
        m_startY = startY;
        m_startZ = startZ;
        
        m_sectionDepth = sectionDepth;
    }

    TSectionsTerrainGenerator::~TSectionsTerrainGenerator()
    {

    }

    void TSectionsTerrainGenerator::setStart( float x, float y, float z )
    {
        m_startX = x;
        m_startY = y;
        m_startZ = z;
    }

    void TSectionsTerrainGenerator::initialize()
    {
        _initializeSection();
    }

    void TSectionsTerrainGenerator::update()
    {
        _updateSection();
    }

    float TSectionsTerrainGenerator::getProfile1D( float x, float y )
    {
        return _computeProfile1D( x, y );
    }
    
}}