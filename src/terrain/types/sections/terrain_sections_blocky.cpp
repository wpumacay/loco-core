
#include <terrain/types/sections/terrain_sections_blocky.h>

namespace tysoc {
namespace terrain {

    TBlockyTerrainGenerator::TBlockyTerrainGenerator( const std::string& name,
                                                      float startX,
                                                      float startY,
                                                      float startZ,
                                                      const TBlockyParams& params )
        : TSectionsTerrainGenerator( name, startX, startY, startZ, params.baseDepth )
    {
        m_params = params;

        m_currentSpawnX = startX;
        m_lastSpawnX = startX;

        m_basePrimitivePtr = NULL;

        m_type = TERRAIN_TYPE_PROCEDURAL_SECTIONS_BLOCKS;
    }

    TBlockyTerrainGenerator::~TBlockyTerrainGenerator()
    {

    }

    void TBlockyTerrainGenerator::_createBase()
    {
        if ( m_basePrimitivePtr == NULL )
        {
            m_basePrimitivePtr = new TBlockyTerrainPrimitive();
            m_primitives.push_back( m_basePrimitivePtr );
        }

        auto _px = m_startX + 0.5f * m_params.sectionLength;
        auto _py = m_startY;
        auto _pz = m_startZ - 0.5f * DEFAULT_SECTION_BASE_HEIGHT;
        
        auto _sx = m_params.sectionLength;
        auto _sy = m_params.baseDepth;
        auto _sz = DEFAULT_SECTION_BASE_HEIGHT;

        // base information
        m_basePrimitivePtr->size.x = _sx;
        m_basePrimitivePtr->size.y = _sy;
        m_basePrimitivePtr->size.z = _sz;

        m_basePrimitivePtr->pos.x = _px;
        m_basePrimitivePtr->pos.y = _py;
        m_basePrimitivePtr->pos.z = _pz;

        m_basePrimitivePtr->rotmat[0] = 1.0f;
        m_basePrimitivePtr->rotmat[1] = 0.0f;
        m_basePrimitivePtr->rotmat[2] = 0.0f;
        m_basePrimitivePtr->rotmat[3] = 0.0f;
        m_basePrimitivePtr->rotmat[4] = 1.0f;
        m_basePrimitivePtr->rotmat[5] = 0.0f;
        m_basePrimitivePtr->rotmat[6] = 0.0f;
        m_basePrimitivePtr->rotmat[7] = 0.0f;
        m_basePrimitivePtr->rotmat[8] = 1.0f;

        // @CHECK: there seems to be a small issue with the platforms
        m_basePrimitivePtr->rbound = 0.5f * std::sqrt( _sx * _sx + _sy * _sy + _sz * _sz );
        std::cout << "rbound: " << m_basePrimitivePtr->rbound << std::endl;

        // custom information (for usage here in the profile computation part)
        m_basePrimitivePtr->AABB.p1x = _px - 0.5f * _sx;
        m_basePrimitivePtr->AABB.p1y = _py - 0.5f * _sy;
        m_basePrimitivePtr->AABB.p1z = _pz - 0.5f * _sz;

        m_basePrimitivePtr->AABB.p2x = _px + 0.5f * _sx;
        m_basePrimitivePtr->AABB.p2y = _py + 0.5f * _sy;
        m_basePrimitivePtr->AABB.p2z = _pz + 0.5f * _sz;

        m_basePrimitivePtr->inUse = true;
        m_basePrimitivePtr->useCustomColor = true;
        m_basePrimitivePtr->color = { 0.83f, 0.83f, 0.83f };

        m_fixed.push( m_basePrimitivePtr );
    }

    // @CHECK: Could generalize this in the base class and make ...
    // profileType and blockyType implement the specific terrain logic ???
    void TBlockyTerrainGenerator::_createBlock()
    {
        // go forward by one spawn section
        m_lastSpawnX = m_currentSpawnX;
        m_currentSpawnX += _RANDOM( m_params.baseSpacingX * m_params.percentSpacingX.min,
                                    m_params.baseSpacingX * m_params.percentSpacingX.max );

        // create the block data
        TBlockyTerrainPrimitive* _blockPrimitive = NULL;
        if ( !m_pool.empty() )
        {
            // get the recycled object
            _blockPrimitive = ( TBlockyTerrainPrimitive* ) m_pool.front();
            // remove it from the pool
            m_pool.pop();
        }
        else
        {
            // just create a new one
            _blockPrimitive = new TBlockyTerrainPrimitive();
            m_primitives.push_back( _blockPrimitive );
        }

        _blockPrimitive->inUse = true;
        _blockPrimitive->useCustomColor = true;
        // compute information for the primitive
        {
            auto _sx = _RANDOM( m_params.baseWidth * m_params.percentWidth.min,
                                m_params.baseWidth * m_params.percentWidth.max );
            auto _sy = _RANDOM( m_params.baseDepth * m_params.percentDepth.min,
                                m_params.baseDepth * m_params.percentDepth.max );
            auto _sz = _RANDOM( m_params.baseHeight * m_params.percentHeight.min,
                                m_params.baseHeight * m_params.percentHeight.max );

            auto _px = m_currentSpawnX;
            auto _py = m_startY;
            if ( m_params.usesSides )
            {
                float _sideSign = ( _RAND() > 0.5f ) ? -1.0f : 1.0f;
                _py = m_startY + _sideSign * ( 0.5f * m_params.baseDepth - 0.5f * _sy );
            }
            auto _pz = m_startZ + 0.5f * _sz + _RANDOM( m_params.baseOffsetZ * m_params.percentOffsetZ.min,
                                                        m_params.baseOffsetZ * m_params.percentOffsetZ.max );

            // base information
            _blockPrimitive->size.x = _sx;
            _blockPrimitive->size.y = _sy;
            _blockPrimitive->size.z = _sz;

            _blockPrimitive->pos.x = _px;
            _blockPrimitive->pos.y = _py;
            _blockPrimitive->pos.z = _pz;

            _blockPrimitive->rotmat[0] = 1.0f;
            _blockPrimitive->rotmat[1] = 0.0f;
            _blockPrimitive->rotmat[2] = 0.0f;
            _blockPrimitive->rotmat[3] = 0.0f;
            _blockPrimitive->rotmat[4] = 1.0f;
            _blockPrimitive->rotmat[5] = 0.0f;
            _blockPrimitive->rotmat[6] = 0.0f;
            _blockPrimitive->rotmat[7] = 0.0f;
            _blockPrimitive->rotmat[8] = 1.0f;

            _blockPrimitive->rbound = 0.5f * std::sqrt( _sx * _sx + _sy * _sy + _sz * _sz );

            // custom information (for usage here in the profile computation part)
            _blockPrimitive->AABB.p1x = _px - 0.5f * _sx;
            _blockPrimitive->AABB.p1y = _py - 0.5f * _sy;
            _blockPrimitive->AABB.p1z = _pz - 0.5f * _sz;

            _blockPrimitive->AABB.p2x = _px + 0.5f * _sx;
            _blockPrimitive->AABB.p2y = _py + 0.5f * _sy;
            _blockPrimitive->AABB.p2z = _pz + 0.5f * _sz;

            // _blockPrimitive->color = _RANDOM_COLOR;
            _blockPrimitive->color = { 0.447f, 0.435f, 0.361f };
        }

        m_justCreated.push( _blockPrimitive );
    }

    void TBlockyTerrainGenerator::_initializeSection()
    {
        if ( m_params.usesBase )
        {
            _createBase();
        }

        // reload starting X position
        m_currentSpawnX = m_startX;

        // create some sections in front
        for ( size_t i = 0; i < PROCEDURAL_TERRAIN_POOL_SIZE; i++ )
        {
            _createBlock();
        }
    }

    void TBlockyTerrainGenerator::_updateSection()
    {
        // If noone grabbed the just created elements, then
        // put those pool elements back into the pool for reuse
        while ( !m_justCreated.empty() )
        {
            m_pool.push( m_justCreated.front() );
            m_justCreated.pop();
        }

        auto _dx = ( m_generatorInfo->trackingpoint.x - m_lastSpawnX );
        if ( _dx > 0 )
        {
            // create the section with this new position
            _createBlock();
        }
    }

    float TBlockyTerrainGenerator::_computeProfile1D( float x, float y )
    {
        for ( size_t i = 0; i < m_primitives.size(); i++ )
        {
            if ( m_primitives[i] == m_basePrimitivePtr )
            {
                continue;
            }

            if ( !m_primitives[i]->inUse )
            {
                continue;
            }

            auto _primitive = reinterpret_cast< TBlockyTerrainPrimitive* >( m_primitives[i] );
            if ( _primitive->AABB.p1x < x && x < _primitive->AABB.p2x &&
                 _primitive->AABB.p1y < y && y < _primitive->AABB.p2y )
            {
                return _primitive->AABB.p2z;
            }
        }

        return m_startZ;
    }
    
}}