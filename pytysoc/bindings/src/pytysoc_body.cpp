
#include <pytysoc_body.h>

namespace py = pybind11;

namespace pytysoc
{

    PyBody::PyBody( const std::string& name,
                    const PyBodyData& data,
                    py::array_t<TScalar>& xyz,
                    py::array_t<TScalar>& rpy )
    {
        m_bodyPtr = new tysoc::TBody( name, 
                                      toTBodyData( data ), 
                                      numpyToVec3( xyz ),
                                      tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ) );

        auto _collision = m_bodyPtr->collision();
        if ( _collision )
        {
            auto _pyCollision = new PyCollision( _collision );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            _pyCollision->setParentBody( this );
            m_pyCollision = _pyCollision;
        }

        auto _visual = m_bodyPtr->visual();
        if ( _visual )
        {
            auto _pyVisual = new PyVisual( _visual );
            // @TODO: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            _pyVisual->setParentBody( this );
            m_pyVisual = _pyVisual;
        }
    }

    PyBody::PyBody( tysoc::TBody* bodyPtr )
    {
        m_bodyPtr = bodyPtr;

        auto _collision = m_bodyPtr->collision();
        if ( _collision )
        {
            auto _pyCollision = new PyCollision( _collision );
            // @TODO: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            _pyCollision->setParentBody( this );
            m_pyCollision = _pyCollision;
        }

        auto _visual = m_bodyPtr->visual();
        if ( _visual )
        {
            auto _pyVisual = new PyVisual( _visual );
            // @TODO: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            _pyVisual->setParentBody( this );
            m_pyVisual = _pyVisual;
        }
    }

    PyBody::~PyBody()
    {
        if ( m_pyCollision )
            delete m_pyCollision;

        if ( m_pyVisual )
            delete m_pyVisual;

        m_bodyPtr = nullptr;
    }

    void PyBody::setCollision( PyCollision* pyCollisionPtr )
    {
        TYSOC_CORE_ASSERT( m_bodyPtr, "PyBody setCollision >>> wrapper contains null reference to a body" );

        m_pyCollision = pyCollisionPtr;
        m_bodyPtr->setCollision( std::unique_ptr< tysoc::TCollision >( pyCollisionPtr->ptr() ) );
    }

    void PyBody::setVisual( PyVisual* pyVisualPtr )
    {
        TYSOC_CORE_ASSERT( m_bodyPtr, "PyBody setCollision >>> wrapper contains null reference to a body" );

        m_pyVisual = pyVisualPtr;
        m_bodyPtr->setVisual( std::unique_ptr< tysoc::TVisual >( pyVisualPtr->ptr() ) );
    }

    void PyBody::setPosition( py::array_t<TScalar>& position )
    {
        assert( m_bodyPtr );

        m_bodyPtr->setPosition( numpyToVec3( position ) );
    }

    void PyBody::setRotation( py::array_t<TScalar>& rotation )
    {
        assert( m_bodyPtr );

        m_bodyPtr->setRotation( numpyToMat3( rotation ) );
    }

    void PyBody::setEuler( py::array_t<TScalar>& rpy )
    {
        assert( m_bodyPtr );

        m_bodyPtr->setEuler( numpyToVec3( rpy ) );
    }

    void PyBody::setQuaternion( py::array_t<TScalar>& quat )
    {
        assert( m_bodyPtr );

        m_bodyPtr->setQuaternion( numpyToVec4( quat ) );
    }

    void PyBody::setTransform( py::array_t<TScalar>& transform )
    {
        assert( m_bodyPtr );

        m_bodyPtr->setTransform( numpyToMat4( transform ) );
    }

    std::string PyBody::name()
    {
        assert( m_bodyPtr );

        return m_bodyPtr->name();
    }

    py::array_t<TScalar> PyBody::pos()
    {
        assert( m_bodyPtr );

        return vec3ToNumpy( m_bodyPtr->pos() );
    }

    py::array_t<TScalar> PyBody::rot()
    {
        assert( m_bodyPtr );

        return mat3ToNumpy( m_bodyPtr->rot() );
    }

    py::array_t<TScalar> PyBody::quat()
    {
        assert( m_bodyPtr );

        return vec4ToNumpy( m_bodyPtr->quat() );
    }

    py::array_t<TScalar> PyBody::euler()
    {
        assert( m_bodyPtr );

        return vec3ToNumpy( m_bodyPtr->euler() );
    }

    py::array_t<TScalar> PyBody::tf()
    {
        assert( m_bodyPtr );

        return mat4ToNumpy( m_bodyPtr->tf() );
    }

    py::array_t<TScalar> PyBody::pos0()
    {
        assert( m_bodyPtr );

        return vec3ToNumpy( m_bodyPtr->pos0() );
    }

    py::array_t<TScalar> PyBody::rot0()
    {
        assert( m_bodyPtr );

        return mat3ToNumpy( m_bodyPtr->rot0() );
    }

    py::array_t<TScalar> PyBody::quat0()
    {
        assert( m_bodyPtr );

        return vec4ToNumpy( m_bodyPtr->quat0() );
    }

    py::array_t<TScalar> PyBody::euler0()
    {
        assert( m_bodyPtr );

        return vec3ToNumpy( m_bodyPtr->euler0() );
    }

    py::array_t<TScalar> PyBody::tf0()
    {
        assert( m_bodyPtr );

        return mat4ToNumpy( m_bodyPtr->tf0() );
    }

    tysoc::eDynamicsType PyBody::dyntype()
    {
        assert( m_bodyPtr );

        return m_bodyPtr->dyntype();
    }

    PyBodyData PyBody::data()
    {
        assert( m_bodyPtr );

        return toPyBodyData( m_bodyPtr->data() );
    }

}