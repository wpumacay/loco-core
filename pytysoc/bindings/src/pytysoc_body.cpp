
#include <pytysoc_body.h>

namespace py = pybind11;

namespace pytysoc
{

    PyBody::PyBody( const std::string& name,
                    const PyBodyData& data )
    {
        /* instantiation should happen in sub-classes*/
        m_body = nullptr;
        m_pyCollision = nullptr;
        m_pyVisual = nullptr;
    }

    PyBody::~PyBody()
    {
        m_pyCollision = nullptr;
        m_pyVisual = nullptr;
        m_body = nullptr;
    }

    // @todo: check ownership (python passes wrapped ref-counted object???)
    void PyBody::setCollision( PyCollision* pyCollisionObj )
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::setCollision() >>> wrapper contains null reference to a body" );

        m_pyCollision = pyCollisionObj;
        m_body->setCollision( std::unique_ptr< tysoc::TCollision >( pyCollisionObj->ptr() ) );
    }

    // @todo: check ownership (python passes wrapped ref-counted object???)
    void PyBody::setVisual( PyVisual* pyVisualObj )
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::setVisual() >>> wrapper contains null reference to a body" );

        m_pyVisual = pyVisualObj;
        m_body->setVisual( std::unique_ptr< tysoc::TVisual >( pyVisualObj->ptr() ) );
    }

    void PyBody::setPosition( py::array_t<TScalar>& position )
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::setPosition() >>> wrapper contains null reference to a body" );

        m_body->setPosition( numpyToVec3( position ) );
    }

    void PyBody::setRotation( py::array_t<TScalar>& rotation )
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::setRotation() >>> wrapper contains null reference to a body" );

        m_body->setRotation( numpyToMat3( rotation ) );
    }

    void PyBody::setEuler( py::array_t<TScalar>& rpy )
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::setEuler() >>> wrapper contains null reference to a body" );

        m_body->setEuler( numpyToVec3( rpy ) );
    }

    void PyBody::setQuaternion( py::array_t<TScalar>& quat )
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::setQuaternion() >>> wrapper contains null reference to a body" );

        m_body->setQuaternion( numpyToVec4( quat ) );
    }

    void PyBody::setTransform( py::array_t<TScalar>& transform )
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::setTransform() >>> wrapper contains null reference to a body" );

        m_body->setTransform( numpyToMat4( transform ) );
    }

    std::string PyBody::name() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::name() >>> wrapper contains null reference to a body" );

        return m_body->name();
    }

    py::array_t<TScalar> PyBody::pos() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::pos() >>> wrapper contains null reference to a body" );

        return vec3ToNumpy( m_body->pos() );
    }

    py::array_t<TScalar> PyBody::rot() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::rot() >>> wrapper contains null reference to a body" );

        return mat3ToNumpy( m_body->rot() );
    }

    py::array_t<TScalar> PyBody::quat() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::quat() >>> wrapper contains null reference to a body" );

        return vec4ToNumpy( m_body->quat() );
    }

    py::array_t<TScalar> PyBody::euler() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::euler() >>> wrapper contains null reference to a body" );

        return vec3ToNumpy( m_body->euler() );
    }

    py::array_t<TScalar> PyBody::tf() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::tf() >>> wrapper contains null reference to a body" );

        return mat4ToNumpy( m_body->tf() );
    }

    py::array_t<TScalar> PyBody::pos0() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::pos0() >>> wrapper contains null reference to a body" );

        return vec3ToNumpy( m_body->pos0() );
    }

    py::array_t<TScalar> PyBody::rot0() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::rot0() >>> wrapper contains null reference to a body" );

        return mat3ToNumpy( m_body->rot0() );
    }

    py::array_t<TScalar> PyBody::quat0() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::quat0() >>> wrapper contains null reference to a body" );

        return vec4ToNumpy( m_body->quat0() );
    }

    py::array_t<TScalar> PyBody::euler0() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::euler0() >>> wrapper contains null reference to a body" );

        return vec3ToNumpy( m_body->euler0() );
    }

    py::array_t<TScalar> PyBody::tf0() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::tf0() >>> wrapper contains null reference to a body" );

        return mat4ToNumpy( m_body->tf0() );
    }

    tysoc::eDynamicsType PyBody::dyntype() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::dyntype() >>> wrapper contains null reference to a body" );

        return m_body->dyntype();
    }

    PyBodyData PyBody::data() const
    {
        TYSOC_CORE_ASSERT( m_body, "PyBody::data() >>> wrapper contains null reference to a body" );

        return toPyBodyData( m_body->data() );
    }

}