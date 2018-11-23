
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
        for ( size_t i = 0; i < m_primitives.size(); i++ )
        {
            delete m_primitives[i];
        }
        m_primitives.clear();

        while ( !m_pool.empty() )
        {
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