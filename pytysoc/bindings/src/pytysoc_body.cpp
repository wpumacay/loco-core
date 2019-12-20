
#include <pytysoc_body.h>

namespace py = pybind11;

namespace pytysoc
{

    PyBody::PyBody( const std::string& name,
                    const PyBodyData& data )
    {
        /* instantiation should happen in sub-classes*/
        m_bodyRef = nullptr;
        m_pyCollision = nullptr;
        m_pyVisual = nullptr;
    }

    PyBody::~PyBody()
    {
        m_pyCollision = nullptr;
        m_pyVisual = nullptr;
        m_bodyRef = nullptr;
    }

    // @todo: check ownership (python passes wrapped ref-counted object???)
    void PyBody::setCollision( PyCollision* pyCollisionObj )
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::setCollision() >>> wrapper contains null reference to a body" );

        m_pyCollision = pyCollisionObj;
        m_bodyRef->setCollision( std::unique_ptr< tysoc::TCollision >( pyCollisionObj->ptr() ) );
    }

    // @todo: check ownership (python passes wrapped ref-counted object???)
    void PyBody::setVisual( PyVisual* pyVisualObj )
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::setVisual() >>> wrapper contains null reference to a body" );

        m_pyVisual = pyVisualObj;
        m_bodyRef->setVisual( std::unique_ptr< tysoc::TVisual >( pyVisualObj->ptr() ) );
    }

    void PyBody::setPosition( py::array_t<TScalar>& position )
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::setPosition() >>> wrapper contains null reference to a body" );

        m_bodyRef->setPosition( numpyToVec3( position ) );
    }

    void PyBody::setRotation( py::array_t<TScalar>& rotation )
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::setRotation() >>> wrapper contains null reference to a body" );

        m_bodyRef->setRotation( numpyToMat3( rotation ) );
    }

    void PyBody::setEuler( py::array_t<TScalar>& rpy )
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::setEuler() >>> wrapper contains null reference to a body" );

        m_bodyRef->setEuler( numpyToVec3( rpy ) );
    }

    void PyBody::setQuaternion( py::array_t<TScalar>& quat )
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::setQuaternion() >>> wrapper contains null reference to a body" );

        m_bodyRef->setQuaternion( numpyToVec4( quat ) );
    }

    void PyBody::setTransform( py::array_t<TScalar>& transform )
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::setTransform() >>> wrapper contains null reference to a body" );

        m_bodyRef->setTransform( numpyToMat4( transform ) );
    }

    py::array_t<TScalar> PyBody::pos() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::pos() >>> wrapper contains null reference to a body" );

        return vec3ToNumpy( m_bodyRef->pos() );
    }

    py::array_t<TScalar> PyBody::rot() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::rot() >>> wrapper contains null reference to a body" );

        return mat3ToNumpy( m_bodyRef->rot() );
    }

    py::array_t<TScalar> PyBody::quat() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::quat() >>> wrapper contains null reference to a body" );

        return vec4ToNumpy( m_bodyRef->quat() );
    }

    py::array_t<TScalar> PyBody::euler() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::euler() >>> wrapper contains null reference to a body" );

        return vec3ToNumpy( m_bodyRef->euler() );
    }

    py::array_t<TScalar> PyBody::tf() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::tf() >>> wrapper contains null reference to a body" );

        return mat4ToNumpy( m_bodyRef->tf() );
    }

    py::array_t<TScalar> PyBody::pos0() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::pos0() >>> wrapper contains null reference to a body" );

        return vec3ToNumpy( m_bodyRef->pos0() );
    }

    py::array_t<TScalar> PyBody::rot0() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::rot0() >>> wrapper contains null reference to a body" );

        return mat3ToNumpy( m_bodyRef->rot0() );
    }

    py::array_t<TScalar> PyBody::quat0() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::quat0() >>> wrapper contains null reference to a body" );

        return vec4ToNumpy( m_bodyRef->quat0() );
    }

    py::array_t<TScalar> PyBody::euler0() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::euler0() >>> wrapper contains null reference to a body" );

        return vec3ToNumpy( m_bodyRef->euler0() );
    }

    py::array_t<TScalar> PyBody::tf0() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::tf0() >>> wrapper contains null reference to a body" );

        return mat4ToNumpy( m_bodyRef->tf0() );
    }

    std::string PyBody::name() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::name() >>> wrapper contains null reference to a body" );

        return m_bodyRef->name();
    }

    tysoc::eBodyClassType PyBody::classType() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::classType() >>> wrapper contains null reference to a body" );

        return m_bodyRef->classType();
    }

    tysoc::eDynamicsType PyBody::dyntype() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::dyntype() >>> wrapper contains null reference to a body" );

        return m_bodyRef->dyntype();
    }

    PyBodyData PyBody::data() const
    {
        TYSOC_CORE_ASSERT( m_bodyRef, "PyBody::data() >>> wrapper contains null reference to a body" );

        return toPyBodyData( m_bodyRef->data() );
    }

}