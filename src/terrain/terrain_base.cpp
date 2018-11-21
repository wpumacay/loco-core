
#include <terrain/terrain_base.h>


namespace tysocterrain
{

    TTerrainGenerator::TTerrainGenerator( const std::string& name )
    {
        m_name = name;
        m_generatorInfo = new TGeneratorInfo();
    }

    TTerrainGenerator::~TTerrainGenerator()
    {
        while ( !m_pool.empty() )
        {
            delete m_pool.front();
            m_pool.pop();
        }

        while ( !m_justCreated.empty() )
        {
            m_justCreated.pop();
        }
    }

    void TTerrainGenerator::recycle( TTerrainPrimitive* pPrimitive )
    {
        // reset the object properties
        pPrimitive->inUse   = false;
        pPrimitive->pos     = { 100.0f, 100.0f, 100.0f };
        pPrimitive->size    = { 0.1f, 0.1f, 0.1f };
        pPrimitive->rbound  = 0.173;
        // put it back into the pool
        m_pool.push( pPrimitive );
    }

    void TTerrainGenerator::flushJustCreatedQueue()
    {
        while( !m_justCreated.empty() )
        {
            m_justCreated.pop();
        }
    }

}