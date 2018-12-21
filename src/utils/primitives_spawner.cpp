
#include <utils/primitives_spawner.h>


namespace tysoc {
namespace utils {


    TPrimitivesSpawner::TPrimitivesSpawner()
    {
        m_keyMapping["box"]         = BOX_INDX;
        m_keyMapping["sphere"]      = SPHERE_INDX;
        m_keyMapping["capsule"]     = CAPSULE_INDX;
        m_keyMapping["cylinder"]    = CYLINDER_INDX;

        // initialize containers
        m_primitives[BOX_INDX]                     = std::vector< TDebugPrimitive* >();
        m_queueAvailablePrimitives[BOX_INDX]       = std::queue< TDebugPrimitive* >();
        m_queueWorkingPrimitives[BOX_INDX]         = std::queue< TDebugPrimitive* >();

        m_primitives[SPHERE_INDX]                  = std::vector< TDebugPrimitive* >();
        m_queueAvailablePrimitives[SPHERE_INDX]    = std::queue< TDebugPrimitive* >();
        m_queueWorkingPrimitives[SPHERE_INDX]      = std::queue< TDebugPrimitive* >();

        m_primitives[CAPSULE_INDX]                 = std::vector< TDebugPrimitive* >();
        m_queueAvailablePrimitives[CAPSULE_INDX]   = std::queue< TDebugPrimitive* >();
        m_queueWorkingPrimitives[CAPSULE_INDX]     = std::queue< TDebugPrimitive* >();

        m_primitives[CYLINDER_INDX]                = std::vector< TDebugPrimitive* >();
        m_queueAvailablePrimitives[CYLINDER_INDX]  = std::queue< TDebugPrimitive* >();
        m_queueWorkingPrimitives[CYLINDER_INDX]    = std::queue< TDebugPrimitive* >();
    }

    TPrimitivesSpawner::~TPrimitivesSpawner()
    {
        while ( !m_queueWorkingPrimitives[BOX_INDX].empty() )
            m_queueWorkingPrimitives[BOX_INDX].pop();

        while ( !m_queueWorkingPrimitives[SPHERE_INDX].empty() )
            m_queueWorkingPrimitives[SPHERE_INDX].pop();

        while ( !m_queueWorkingPrimitives[CAPSULE_INDX].empty() )
            m_queueWorkingPrimitives[CAPSULE_INDX].pop();

        while ( !m_queueWorkingPrimitives[CYLINDER_INDX].empty() )
            m_queueWorkingPrimitives[CYLINDER_INDX].pop();

        for ( size_t i = 0; i < PRIMITIVES_SPAWNER_POOL_SIZE; i++ )
        {
            delete m_primitives[BOX_INDX][i];
            delete m_primitives[SPHERE_INDX][i];
            delete m_primitives[CAPSULE_INDX][i];
            delete m_primitives[CYLINDER_INDX][i];
        }
        m_primitives[BOX_INDX].clear();
        m_primitives[SPHERE_INDX].clear();
        m_primitives[CAPSULE_INDX].clear();
        m_primitives[CYLINDER_INDX].clear();
    }

    void TPrimitivesSpawner::initialize()
    {
        for ( size_t i = 0; i < PRIMITIVES_SPAWNER_POOL_SIZE; i++ )
        {
            auto _box = _createPrimitive( "box", 
                                          0.1f, 0.1f, 0.1f,
                                          0.0f, 0.0f, 200.0f + i * ( 0.1f + 1.0f ) );
            m_primitives[BOX_INDX].push_back( _box );
            m_queueAvailablePrimitives[BOX_INDX].push( _box );

            auto _sphere = _createPrimitive( "sphere",
                                             0.1f, 0.1f, 0.1f,
                                             0.0f, 0.0f, 300.0f + i * ( 0.1f + 1.0f ) );
            m_primitives[SPHERE_INDX].push_back( _sphere );
            m_queueAvailablePrimitives[SPHERE_INDX].push( _sphere );

            auto _capsule = _createPrimitive( "capsule",
                                              0.1f, 0.1f, 0.1f,
                                              0.0f, 0.0f, 400.0f + i * ( 0.3f + 1.0f ) );
            m_primitives[CAPSULE_INDX].push_back( _capsule );
            m_queueAvailablePrimitives[CAPSULE_INDX].push( _capsule );

            auto _cylinder = _createPrimitive( "cylinder",
                                               0.1f, 0.1f, 0.1f,
                                               0.0f, 0.0f, 500.0f + i * ( 0.2f + 1.0f ) );
            m_primitives[CYLINDER_INDX].push_back( _cylinder );
            m_queueAvailablePrimitives[CYLINDER_INDX].push( _cylinder );
        }
    }

    TDebugPrimitive* TPrimitivesSpawner::_createPrimitive( const std::string& type,
                                                           float sx, float sy, float sz,
                                                           float x, float y, float z )
    {
        auto _primitivePtr = _createPrimitiveInternal( type,
                                                       sx, sy, sz,
                                                       x, y, z );

        _primitivePtr->type         = type;
        _primitivePtr->size         = { sx, sy, sz };
        _primitivePtr->restPosition = { x, y, z };
        _primitivePtr->position     = { x, y, z };
        _primitivePtr->velocity     = { 0.0f, 0.0f, 0.0f };
        _primitivePtr->color        = { 0.447f, 0.435f, 0.361f };
        _primitivePtr->inUse        = false;
        _primitivePtr->lifetime     = 0.0f;
        
        _primitivePtr->rotmat[0] = 1.0f;
        _primitivePtr->rotmat[1] = 0.0f;
        _primitivePtr->rotmat[2] = 0.0f;
        _primitivePtr->rotmat[3] = 0.0f;
        _primitivePtr->rotmat[4] = 1.0f;
        _primitivePtr->rotmat[5] = 0.0f;
        _primitivePtr->rotmat[6] = 0.0f;
        _primitivePtr->rotmat[7] = 0.0f;
        _primitivePtr->rotmat[8] = 1.0f;

        return _primitivePtr;
    }

    void TPrimitivesSpawner::spawnObject( const std::string& type,
                                          float sx, float sy, float sz, 
                                          float x, float y, float z )
    {
        if ( m_keyMapping.find( type ) == m_keyMapping.end() )
        {
            std::cout << "WARNING> Could not spawn object with type: " << type << std::endl;
            return;
        }

        if ( m_queueAvailablePrimitives[ m_keyMapping[ type ] ].empty() )
        {
            m_queueAvailablePrimitives[ m_keyMapping[ type ] ].push( m_queueWorkingPrimitives[ m_keyMapping[ type ] ].front() );
            m_queueWorkingPrimitives[ m_keyMapping[ type ] ].pop();
        }

        // grab the latest primitive from the avaiable-queue
        auto _primitivePtr = m_queueAvailablePrimitives[ m_keyMapping[ type ] ].front();
        m_queueAvailablePrimitives[ m_keyMapping[ type ] ].pop();

        // activate the grabbed primitive
        _activatePrimitive( _primitivePtr,
                            sx, sy, sz, x, y, z );
    }

    void TPrimitivesSpawner::_activatePrimitive( TDebugPrimitive* primitivePtr,
                                                float sx, float sy, float sz,
                                                float x, float y, float z )
    {
        // Make specific activation steps here
        _activatePrimitiveInternal( primitivePtr,
                                    sx, sy, sz,
                                    x, y, z );

        primitivePtr->size = { sx, sy, sz };
        primitivePtr->inUse = true;

        // Send it to work
        m_queueWorkingPrimitives[ m_keyMapping[ primitivePtr->type ] ].push( primitivePtr );
    }

    void TPrimitivesSpawner::update( float dt )
    {
        for ( size_t i = 0; i < PRIMITIVES_SPAWNER_POOL_SIZE; i++ )
        {
            if ( m_primitives[BOX_INDX][i]->inUse )
            {
                m_primitives[BOX_INDX][i]->lifetime += dt;
                _updatePrimitiveInternal( m_primitives[BOX_INDX][i] );
            }

            if ( m_primitives[SPHERE_INDX][i]->inUse )
            {
                m_primitives[SPHERE_INDX][i]->lifetime += dt;
                _updatePrimitiveInternal( m_primitives[SPHERE_INDX][i] );
            }

            if ( m_primitives[CAPSULE_INDX][i]->inUse )
            {
                m_primitives[CAPSULE_INDX][i]->lifetime += dt;
                _updatePrimitiveInternal( m_primitives[CAPSULE_INDX][i] );
            }

            if ( m_primitives[CYLINDER_INDX][i]->inUse )
            {
                m_primitives[CYLINDER_INDX][i]->lifetime += dt;
                _updatePrimitiveInternal( m_primitives[CYLINDER_INDX][i] );
            }
        }

        while ( !m_queueWorkingPrimitives[BOX_INDX].empty() )
        {
            auto _box = m_queueWorkingPrimitives[BOX_INDX].front();
            if ( _box->lifetime > PRIMITIVES_DEFAULT_LIFETIME )
            {
                m_queueAvailablePrimitives[BOX_INDX].push( _box );
                m_queueWorkingPrimitives[BOX_INDX].pop();
                _recyclePrimitive( _box );
            }
            else
            {
                break;
            }
        }

        while ( !m_queueWorkingPrimitives[SPHERE_INDX].empty() )
        {
            auto _sphere = m_queueWorkingPrimitives[SPHERE_INDX].front();
            if ( _sphere->lifetime > PRIMITIVES_DEFAULT_LIFETIME )
            {
                m_queueAvailablePrimitives[SPHERE_INDX].push( _sphere );
                m_queueWorkingPrimitives[SPHERE_INDX].pop();
                _recyclePrimitive( _sphere );
            }
            else
            {
                break;
            }
        }

        while ( !m_queueWorkingPrimitives[CAPSULE_INDX].empty() )
        {
            auto _capsule = m_queueWorkingPrimitives[CAPSULE_INDX].front();
            if ( _capsule->lifetime > PRIMITIVES_DEFAULT_LIFETIME )
            {
                m_queueAvailablePrimitives[CAPSULE_INDX].push( _capsule );
                m_queueWorkingPrimitives[CAPSULE_INDX].pop();
                _recyclePrimitive( _capsule );
            }
            else
            {
                break;
            }
        }

        while ( !m_queueWorkingPrimitives[CYLINDER_INDX].empty() )
        {
            auto _cylinder = m_queueWorkingPrimitives[CYLINDER_INDX].front();
            if ( _cylinder->lifetime > PRIMITIVES_DEFAULT_LIFETIME )
            {
                m_queueAvailablePrimitives[CYLINDER_INDX].push( _cylinder );
                m_queueWorkingPrimitives[CYLINDER_INDX].pop();
                _recyclePrimitive( _cylinder );
            }
            else
            {
                break;
            }
        }
    }

    void TPrimitivesSpawner::_recyclePrimitive( TDebugPrimitive* primitivePtr )
    {
        primitivePtr->size          = { 0.1f, 0.1f, 0.1f };
        primitivePtr->position.x    = primitivePtr->restPosition.x;
        primitivePtr->position.y    = primitivePtr->restPosition.y;
        primitivePtr->position.z    = primitivePtr->restPosition.z;
        primitivePtr->velocity      = { 0.0f, 0.0f, 0.0f };
        primitivePtr->inUse         = false;
        primitivePtr->lifetime      = 0.0f;

        _recyclePrimitiveInternal( primitivePtr );
    }

    std::vector< TDebugPrimitive* > TPrimitivesSpawner::getPrimitives( const std::string& type )
    {
        if ( m_keyMapping.find( type ) == m_keyMapping.end() )
        {
            return std::vector< TDebugPrimitive* >();
        }

        return m_primitives[ m_keyMapping[ type ] ];
    }

}}