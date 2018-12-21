
#include <terrain/types/sections/terrain_sections_connectedpath.h>



namespace tysoc {
namespace terrain {

    
    TPathTerrainGenerator::TPathTerrainGenerator( const std::string& name,
                                                  float startX,
                                                  float startY,
                                                  float startZ,
                                                  float sectionDepth,
                                                  float componentsSpacingX, 
                                                  float componentsThickness,
                                                  TProfileGenerator* generator )
        : TSectionsTerrainGenerator( name, startX, startY, startZ, sectionDepth )
    {
        m_currentX              = startX;
        m_generator             = generator;
        m_componentsThickness   = componentsThickness;
        m_componentsSpacingX    = componentsSpacingX;

        m_type = TERRAIN_TYPE_PROCEDURAL_SECTIONS_PATH;
    }

    TPathTerrainGenerator::~TPathTerrainGenerator()
    {
        if ( m_generator )
        {
            delete m_generator;
            m_generator = NULL;
        }
    }

    void TPathTerrainGenerator::_initializeSection()
    {
        // reload the starting X position
        m_currentX = m_startX;

        // create some sections in front
        for ( size_t i = 0; i < PROCEDURAL_TERRAIN_POOL_SIZE; i++ )
        {
            _createPathComponent();
        }
    }

    float TPathTerrainGenerator::_computeProfile1D( float x, float y )
    {
        return m_generator->generate( x );
    }

    void TPathTerrainGenerator::_updateSection()
    {
        // If noone grabbed the just created elements, then
        // put those pool elements back into the pool for reuse
        while ( !m_justCreated.empty() )
        {
            m_pool.push( m_justCreated.front() );
            m_justCreated.pop();
        }

        auto _dx = ( m_generatorInfo->trackingpoint.x - m_currentX );
        if ( _dx > 4 * m_componentsSpacingX )
        {
            // create the section with this new position
            _createPathComponent();
        }
    }
    
    void TPathTerrainGenerator::_createPathComponent()
    {
        float _prevX = m_currentX - 0.5f * m_componentsSpacingX;
        float _nextX = m_currentX + 0.5f * m_componentsSpacingX;
        
        // create the new section data
        TPathTerrainPrimitive* _pathPrimitive = NULL;
        if ( !m_pool.empty() )
        {
            // get the recycled object
            _pathPrimitive = ( TPathTerrainPrimitive* ) m_pool.front();
            // and remove it from the pool
            m_pool.pop();
        }
        else
        {
            // just create a new one
            _pathPrimitive = new TPathTerrainPrimitive();
            m_primitives.push_back( _pathPrimitive );
        }

        _pathPrimitive->inUse = true;
        _pathPrimitive->useCustomColor = true;
        // compute vertices
        {
            _pathPrimitive->p1.x = _prevX;
            _pathPrimitive->p1.y = m_startY + 0.5f * m_sectionDepth;
            _pathPrimitive->p1.z = m_generator->generate( _prevX );

            _pathPrimitive->p2.x = _nextX;
            _pathPrimitive->p2.y = m_startY + 0.5f * m_sectionDepth;
            _pathPrimitive->p2.z = m_generator->generate( _nextX );

            _pathPrimitive->p3.x = _pathPrimitive->p2.x;
            _pathPrimitive->p3.y = _pathPrimitive->p2.y - m_sectionDepth;
            _pathPrimitive->p3.z = _pathPrimitive->p2.z;

            _pathPrimitive->p4.x = _pathPrimitive->p1.x;
            _pathPrimitive->p4.y = _pathPrimitive->p1.y - m_sectionDepth;
            _pathPrimitive->p4.z = _pathPrimitive->p1.z;
        }
        // compute other necessary info
        {
            // position
            _pathPrimitive->pos.x = ( _pathPrimitive->p1.x + _pathPrimitive->p2.x + _pathPrimitive->p3.x + _pathPrimitive->p4.x ) / 4.0f;
            _pathPrimitive->pos.y = ( _pathPrimitive->p1.y + _pathPrimitive->p2.y + _pathPrimitive->p3.y + _pathPrimitive->p4.y ) / 4.0f;
            _pathPrimitive->pos.z = ( _pathPrimitive->p1.z + _pathPrimitive->p2.z + _pathPrimitive->p3.z + _pathPrimitive->p4.z ) / 4.0f;
            
            // normal ( p21 cross p41, where p41 = p4 - p1, p21 = p2 - p1 )
            auto _p41x = _pathPrimitive->p4.x - _pathPrimitive->p1.x; // 0.0
            auto _p41y = _pathPrimitive->p4.y - _pathPrimitive->p1.y; // depth
            auto _p41z = _pathPrimitive->p4.z - _pathPrimitive->p1.z; // 0.0

            auto _p21x = _pathPrimitive->p2.x - _pathPrimitive->p1.x; // spawndeltax
            auto _p21y = _pathPrimitive->p2.y - _pathPrimitive->p1.y; // 0.0
            auto _p21z = _pathPrimitive->p2.z - _pathPrimitive->p1.z; // dz

            // @TODO: Add some asserts. Because of the assumed plane the normal.y must be 0.0
            _pathPrimitive->normal.x = _p21y * _p41z - _p41y * _p21z; // -depth * dz
            _pathPrimitive->normal.y = _p21z * _p41x - _p41z * _p21x; // 0.0
            _pathPrimitive->normal.z = _p21x * _p41y - _p41x * _p21y; // spawndelta * depth

            auto _norm = std::sqrt( _pathPrimitive->normal.x * _pathPrimitive->normal.x +
                                    _pathPrimitive->normal.y * _pathPrimitive->normal.y +
                                    _pathPrimitive->normal.z * _pathPrimitive->normal.z );

            _pathPrimitive->normal.x = _pathPrimitive->normal.x / _norm;
            _pathPrimitive->normal.y = _pathPrimitive->normal.y / _norm;
            _pathPrimitive->normal.z = _pathPrimitive->normal.z / _norm;

            // size
            _pathPrimitive->size.x = std::sqrt( _p21x * _p21x + _p21y * _p21y + _p21z * _p21z );
            _pathPrimitive->size.y = std::sqrt( _p41x * _p41x + _p41y * _p41y + _p41z * _p41z ); // should be the same as m_sectionDepth
            _pathPrimitive->size.z = m_componentsThickness;

            _pathPrimitive->rbound = 0.5f * std::sqrt( _pathPrimitive->size.x * _pathPrimitive->size.x +
                                                       _pathPrimitive->size.y * _pathPrimitive->size.y +
                                                       _pathPrimitive->size.z * _pathPrimitive->size.z );

            // rotation matrix -> stored in column major form
            /*
            *  
            * [ _  0  _ ]    [  nz  0  nx ]
            * [ t  1  n ] =  [  0   1  0  ] (because it's a plane)
            * [    0    ]    [ -nx  0  nz ]
            */
            _pathPrimitive->rotmat[0] = _pathPrimitive->normal.z;
            _pathPrimitive->rotmat[1] = 0.0f;
            _pathPrimitive->rotmat[2] = -_pathPrimitive->normal.x;

            _pathPrimitive->rotmat[3] = 0.0f;
            _pathPrimitive->rotmat[4] = 1.0f;
            _pathPrimitive->rotmat[5] = 0.0f;

            _pathPrimitive->rotmat[6] = _pathPrimitive->normal.x;
            _pathPrimitive->rotmat[7] = _pathPrimitive->normal.y;// must be 0.0
            _pathPrimitive->rotmat[8] = _pathPrimitive->normal.z;

            _pathPrimitive->color = { 0.83f, 0.83f, 0.83f };
        }

        m_justCreated.push( _pathPrimitive );

        m_currentX += m_componentsSpacingX;
    }


}}