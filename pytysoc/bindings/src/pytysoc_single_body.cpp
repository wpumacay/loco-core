
#include <pytysoc_body.h>

namespace py = pybind11;

namespace pytysoc
{

    PySingleBody::PySingleBody( const std::string& name,
                                const PySingleBodyData& data,
                                py::array_t<TScalar>& xyz,
                                py::array_t<TScalar>& rpy )
    {
        m_body = new tysoc::TSingleBody( name, 
                                         toTBodyData( data ), 
                                         numpyToVec3( xyz ),
                                         tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ) );

        auto _collision = m_body->collision();
        if ( _collision )
        {
            auto _pyCollision = new PyCollision( _collision );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            _pyCollision->setParentBody( this );
            m_pyCollision = _pyCollision;
        }

        auto _visual = m_body->visual();
        if ( _visual )
        {
            auto _pyVisual = new PyVisual( _visual );
            // @TODO: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            _pyVisual->setParentBody( this );
            m_pyVisual = _pyVisual;
        }
    }

    // @todo: change to 
    PySingleBody::PySingleBody( tysoc::TSingleBody* bodyObj )
    {
        m_body = bodyObj;

        auto _collision = m_body->collision();
        if ( _collision )
        {
            auto _pyCollision = new PyCollision( _collision );
            // @TODO: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            _pyCollision->setParentBody( this );
            m_pyCollision = _pyCollision;
        }

        auto _visual = m_body->visual();
        if ( _visual )
        {
            auto _pyVisual = new PyVisual( _visual );
            // @TODO: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            _pyVisual->setParentBody( this );
            m_pyVisual = _pyVisual;
        }
    }

    PySingleBody::~PySingleBody()
    {
        if ( m_pyCollision )
            delete m_pyCollision;

        if ( m_pyVisual )
            delete m_pyVisual;

        m_body = nullptr;
    }

    void PySingleBody::setCollision( PyCollision* pyCollisionObj )
    {
        TYSOC_CORE_ASSERT( m_body, "PySingleBody setCollision >>> wrapper contains null reference to a body" );

        m_pyCollision = pyCollisionObj;
        m_body->setCollision( std::unique_ptr< tysoc::TCollision >( pyCollisionObj->ptr() ) );
    }

    void PySingleBody::setVisual( PyVisual* pyVisualPtr )
    {
        TYSOC_CORE_ASSERT( m_body, "PySingleBody setCollision >>> wrapper contains null reference to a body" );

        m_pyVisual = pyVisualPtr;
        m_body->setVisual( std::unique_ptr< tysoc::TVisual >( pyVisualPtr->ptr() ) );
    }

    void PySingleBody::setPosition( py::array_t<TScalar>& position )
    {
        assert( m_body );

        m_body->setPosition( numpyToVec3( position ) );
    }

    void PySingleBody::setRotation( py::array_t<TScalar>& rotation )
    {
        assert( m_body );

        m_body->setRotation( numpyToMat3( rotation ) );
    }

    void PySingleBody::setEuler( py::array_t<TScalar>& rpy )
    {
        assert( m_body );

        m_body->setEuler( numpyToVec3( rpy ) );
    }

    void PySingleBody::setQuaternion( py::array_t<TScalar>& quat )
    {
        assert( m_body );

        m_body->setQuaternion( numpyToVec4( quat ) );
    }

    void PySingleBody::setTransform( py::array_t<TScalar>& transform )
    {
        assert( m_body );

        m_body->setTransform( numpyToMat4( transform ) );
    }

    std::string PySingleBody::name()
    {
        assert( m_body );

        return m_body->name();
    }

    py::array_t<TScalar> PySingleBody::pos()
    {
        assert( m_body );

        return vec3ToNumpy( m_body->pos() );
    }

    py::array_t<TScalar> PySingleBody::rot()
    {
        assert( m_body );

        return mat3ToNumpy( m_body->rot() );
    }

    py::array_t<TScalar> PySingleBody::quat()
    {
        assert( m_body );

        return vec4ToNumpy( m_body->quat() );
    }

    py::array_t<TScalar> PySingleBody::euler()
    {
        assert( m_body );

        return vec3ToNumpy( m_body->euler() );
    }

    py::array_t<TScalar> PySingleBody::tf()
    {
        assert( m_body );

        return mat4ToNumpy( m_body->tf() );
    }

    py::array_t<TScalar> PySingleBody::pos0()
    {
        assert( m_body );

        return vec3ToNumpy( m_body->pos0() );
    }

    py::array_t<TScalar> PySingleBody::rot0()
    {
        assert( m_body );

        return mat3ToNumpy( m_body->rot0() );
    }

    py::array_t<TScalar> PySingleBody::quat0()
    {
        assert( m_body );

        return vec4ToNumpy( m_body->quat0() );
    }

    py::array_t<TScalar> PySingleBody::euler0()
    {
        assert( m_body );

        return vec3ToNumpy( m_body->euler0() );
    }

    py::array_t<TScalar> PySingleBody::tf0()
    {
        assert( m_body );

        return mat4ToNumpy( m_body->tf0() );
    }

    tysoc::eDynamicsType PySingleBody::dyntype()
    {
        assert( m_body );

        return m_body->dyntype();
    }

    PySingleBodyData PySingleBody::data()
    {
        assert( m_body );

        return toPySingleBodyData( m_body->data() );
    }

}