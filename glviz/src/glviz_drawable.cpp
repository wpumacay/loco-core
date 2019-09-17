
#include <glviz_drawable.h>

namespace tysoc {

    /***************************************************************************
    *                                                                          *
    *                               UTILITIES                                  *
    *                                                                          *
    ***************************************************************************/

    engine::LIRenderable* createRenderable( const TShapeData& data )
    {
        engine::LIRenderable* _renderablePtr = NULL;

        if ( data.type == eShapeType::BOX )
        {
            _renderablePtr = engine::CMeshBuilder::createBox( data.size.x,
                                                              data.size.y,
                                                              data.size.z );
        }
        else if ( data.type == eShapeType::SPHERE )
        {
            _renderablePtr = engine::CMeshBuilder::createSphere( data.size.x );
        }
        else if ( data.type == eShapeType::CYLINDER )
        {
            _renderablePtr = engine::CMeshBuilder::createCylinder( data.size.x,
                                                                   data.size.y );
        }
        else if ( data.type == eShapeType::CAPSULE )
        {
            _renderablePtr = engine::CMeshBuilder::createCapsule( data.size.x,
                                                                  data.size.y );
        }
        else if ( data.type == eShapeType::MESH )
        {
            _renderablePtr = engine::CMeshBuilder::createModelFromFile( data.filename,
                                                                        getFilenameNoExtensionFromFilePath( data.filename ) );
            _renderablePtr->scale = { data.size.x, data.size.y, data.size.z };
        }
        else if ( data.type == eShapeType::HFIELD )
        {
            auto _heightDataScaled = data.hdata.heightData;
            for ( size_t i = 0; i < _heightDataScaled.size(); i++ )
                _heightDataScaled[i] *= data.size.z;

            _renderablePtr = engine::CMeshBuilder::createHeightField( data.hdata.nWidthSamples,
                                                                      data.hdata.nDepthSamples,
                                                                      data.size.x,
                                                                      data.size.y,
                                                                      data.size.x / 2.0f, 
                                                                      data.size.y / 2.0f,
                                                                      _heightDataScaled,
                                                                      GLVIZ_DEFAULT_HFIELD_BASE );
        }
        else
        {
            std::cout << "WARNING> drawable of type: " 
                      << toString( data.type ) << " not supported yet" << std::endl;
        }

        if ( !_renderablePtr )
            return NULL;

        return _renderablePtr;
    }

    /***************************************************************************
    *                                                                          *
    *                           TGLDRAWABLE-IMPL                               *
    *                                                                          *
    ***************************************************************************/

    std::map< eShapeType, std::queue< TGLDrawable* > > TGLDrawable::m_shapesPool;

    std::map< eShapeType, int > TGLDrawable::m_numShapes = { { eShapeType::BOX, 0 },
                                                             { eShapeType::SPHERE, 0 },
                                                             { eShapeType::CYLINDER, 0 },
                                                             { eShapeType::CAPSULE, 0 } };


    TGLDrawable::TGLDrawable( engine::LIRenderable* renderablePtr,
                              const TShapeData& shapeData )
    {
        m_scale         = { 1.0, 1.0, 1.0 };
        m_size          = shapeData.size;
        m_size0         = shapeData.size;
        m_type          = shapeData.type;
        m_renderablePtr = renderablePtr;
        m_data          = shapeData;
    }

    TGLDrawable::~TGLDrawable()
    {
        // @TODO: Implement free in our rendering backend
        // // Notify the owner that this resource can be freed now
        // if ( m_renderablePtr )
        //     m_renderablePtr->free();

        m_renderablePtr = NULL;
    }

    void TGLDrawable::setWorldTransform( const TMat4& transform )
    {
        if ( !m_renderablePtr )
            return;

        auto _position = transform.getPosition();
        auto _rotation = transform.getRotation();

        auto _ePosition = fromTVec3( _position );
        auto _eRotation = fromTMat3( _rotation );

        m_renderablePtr->pos = _ePosition;
        m_renderablePtr->rotation =  _eRotation;
    }

    void TGLDrawable::setColor( const TVec3& fullColor )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->getType() == RENDERABLE_TYPE_MODEL )
        {
            auto _children = reinterpret_cast< engine::LModel* >( m_renderablePtr )->getMeshes();
            for ( size_t i = 0; i < _children.size(); i++ )
                _children[i]->getMaterial()->setColor( { fullColor.x, fullColor.y, fullColor.z } );

            return;
        }

        // all color components are set to the same given color
        m_renderablePtr->getMaterial()->setColor( fromTVec3( fullColor ) );
    }

    void TGLDrawable::setAmbientColor( const TVec3& ambientColor )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->getType() == RENDERABLE_TYPE_MODEL )
        {
            auto _children = reinterpret_cast< engine::LModel* >( m_renderablePtr )->getMeshes();
            for ( size_t i = 0; i < _children.size(); i++ )
                _children[i]->getMaterial()->ambient = { ambientColor.x, ambientColor.y, ambientColor.z };

            return;
        }

        m_renderablePtr->getMaterial()->ambient = fromTVec3( ambientColor );
    }

    void TGLDrawable::setDiffuseColor( const TVec3& diffuseColor )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->getType() == RENDERABLE_TYPE_MODEL )
        {
            auto _children = reinterpret_cast< engine::LModel* >( m_renderablePtr )->getMeshes();
            for ( size_t i = 0; i < _children.size(); i++ )
                _children[i]->getMaterial()->diffuse = { diffuseColor.x, diffuseColor.y, diffuseColor.z };

            return;
        }

        m_renderablePtr->getMaterial()->diffuse = fromTVec3( diffuseColor );
    }

    void TGLDrawable::setSpecularColor( const TVec3& specularColor )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->getType() == RENDERABLE_TYPE_MODEL )
        {
            auto _children = reinterpret_cast< engine::LModel* >( m_renderablePtr )->getMeshes();
            for ( size_t i = 0; i < _children.size(); i++ )
                _children[i]->getMaterial()->specular = { specularColor.x, specularColor.y, specularColor.z };

            return;
        }

        m_renderablePtr->getMaterial()->specular = fromTVec3( specularColor );
    }

    void TGLDrawable::setShininess( const TScalar& shininess )
    {
        if ( !m_renderablePtr )
            return;

        if ( m_renderablePtr->getType() == RENDERABLE_TYPE_MODEL )
        {
            auto _children = reinterpret_cast< engine::LModel* >( m_renderablePtr )->getMeshes();
            for ( size_t i = 0; i < _children.size(); i++ )
                _children[i]->getMaterial()->shininess = shininess;

            return;
        }

        m_renderablePtr->getMaterial()->shininess = shininess;
    }

    void TGLDrawable::changeSize( const tysoc::TVec3& newSize )
    {
        m_size = newSize;
        if ( m_type == eShapeType::PLANE )
        {
            m_scale.x = m_size.x / m_size0.x;
            m_scale.y = m_size.y / m_size0.y;
        }
        else if ( m_type == eShapeType::BOX )
        {
            m_scale = { m_size.x / m_size0.x,
                        m_size.y / m_size0.y,
                        m_size.z / m_size0.z };
        }
        else if ( m_type == eShapeType::SPHERE )
        {
            // scale according to radius
            m_scale.x = m_scale.y = m_scale.z = m_size.x / m_size0.x;
        }
        else if ( m_type == eShapeType::CYLINDER )
        {
            // scale according to radius
            m_scale.x = m_size.x / m_size0.x;
            m_scale.y = m_size.x / m_size0.x;
            // scale according to height
            m_scale.z = m_size.y / m_size0.y;
        }
        else if ( m_type == eShapeType::CAPSULE )
        {
            // scale according to radius
            m_scale.x = m_size.x / m_size0.x;
            m_scale.y = m_size.x / m_size0.x;
            // scale according to height
            m_scale.z = m_size.y / m_size0.y;
        }
        else if ( m_type == eShapeType::MESH )
        {
            // scale every dimension
            m_scale.x = m_size.x / m_size0.x;
            m_scale.y = m_size.y / m_size0.y;
            m_scale.z = m_size.z / m_size0.z;
        }
        else if ( m_type == eShapeType::HFIELD )
        {
            // not allowed, use changeElevationData instead
            std::cout << "WARNING> tried changing the scale of the drawable for a hfield. Should "
                      << "change the elevation data instead" << std::endl;
        }

        if ( !m_renderablePtr )
            return;

        m_renderablePtr->scale = fromTVec3( m_scale );
    }

    void TGLDrawable::changeElevationData( const std::vector< float >& heightData )
    {
        if ( m_data.type != eShapeType::HFIELD )
        {
            std::cout << "WARNING> tried changing gldrawable's elevation data, even though "
                      << "its not a hfield shape" << std::endl;
            return;
        }

        if( ( m_data.hdata.nWidthSamples * m_data.hdata.nDepthSamples ) != heightData.size() )
        {
            std::cout << "WARNING> number of elements in internal and given elevation buffers does not match" << std::endl;
            std::cout << "nx-samples    : " << m_data.hdata.nWidthSamples << std::endl;
            std::cout << "ny-samples    : " << m_data.hdata.nDepthSamples << std::endl;
            std::cout << "hdata.size()  : " << heightData.size() << std::endl;
            return;
        }

        // @TODO: Should add support for dynamic buffers, so for now just destroy the 
        //        renderable and create a new one :(
        delete m_renderablePtr;

        m_data.hdata.heightData = heightData;
        m_renderablePtr = createRenderable( m_data );
    }

    void TGLDrawable::show( bool visible )
    {
        if ( !m_renderablePtr )
            return;

        m_renderablePtr->setVisibility( visible );
    }

    void TGLDrawable::wireframe( bool wireframe )
    {
        if ( !m_renderablePtr )
            return;

        m_renderablePtr->setWireframeMode( wireframe );
    }

    bool TGLDrawable::isVisible()
    {
        if ( !m_renderablePtr )
            return false;

        return m_renderablePtr->isVisible();
    }

    bool TGLDrawable::isWireframe()
    {
        if ( !m_renderablePtr )
            return false;

        return m_renderablePtr->isWireframe();
    }

    void TGLDrawable::recycle()
    {
        if ( !m_renderablePtr )
        {
            std::cout << "WARNING> There's a dangling gl-drawable in here" << std::endl;
            return;
        }

        // hide it till we have to use it again
        m_renderablePtr->setVisibility( false );
        // and add it to the corresponding pool
        if ( TGLDrawable::m_shapesPool.find( m_type ) == TGLDrawable::m_shapesPool.end() )
            TGLDrawable::m_shapesPool[m_type] = std::queue< TGLDrawable* >();
        TGLDrawable::m_shapesPool[m_type].push( this );
    }

    void TGLDrawable::reuse( const TShapeData& data )
    {
        changeSize( data.size );
    }

    TGLDrawable* TGLDrawable::CreateDrawable( const TShapeData& data )
    {
        if ( TGLDrawable::m_shapesPool[data.type].size() > 0 )
        {
            // grab an object from the pool
            auto _recycledDrawable = TGLDrawable::m_shapesPool[data.type].front();
            _recycledDrawable->reuse( data );

            // and make sure we remove it
            TGLDrawable::m_shapesPool[data.type].pop();

            return _recycledDrawable;
        }

        auto _renderablePtr = createRenderable( data );

        return new TGLDrawable( _renderablePtr,
                                data );
    }

    extern "C" TIDrawable* visualizer_createDrawable( const TShapeData& data )
    {
        return TGLDrawable::CreateDrawable( data );
    }

}