
#include <pytysoc_collision.h>

namespace py = pybind11;

namespace pytysoc
{

    PyCollision::PyCollision( const std::string& name,
                              const PyCollisionData& data )
    {
        m_parentPyBodyPtr = nullptr;
        m_collisionPtr = new tysoc::TCollision( name, data );
    }

    PyCollision::PyCollision( tysoc::TCollision* collisionPtr )
    {
        m_parentPyBodyPtr = nullptr;
        m_collisionPtr = collisionPtr;
    }

    PyCollision::~PyCollision()
    {
        m_collisionPtr = nullptr;
        m_parentPyBodyPtr = nullptr;
    }

    void PyCollision::setParentBody( PyBody* parentPyBodyPtr )
    {
        assert( m_collisionPtr );

        m_parentPyBodyPtr = parentPyBodyPtr;
        m_collisionPtr->setParentBody( m_parentPyBodyPtr->ptr() );
    }

    void PyCollision::setVisibility( bool visible )
    {
        assert( m_collisionPtr );

        m_collisionPtr->show( visible );
    }

    void PyCollision::setWireframe( bool wireframe )
    {
        assert( m_collisionPtr );

        m_collisionPtr->wireframe( wireframe );
    }

    void PyCollision::setLocalPosition( py::array_t<TScalar>& localPosition )
    {
        assert( m_collisionPtr );

        m_collisionPtr->setLocalPosition( numpyToVec3( localPosition ) );
    }

    void PyCollision::setLocalRotation( py::array_t<TScalar>& localRotation )
    {
        assert( m_collisionPtr );

        m_collisionPtr->setLocalRotation( numpyToMat3( localRotation ) );
    }

    void PyCollision::setLocalQuat( py::array_t<TScalar>& localQuaternion )
    {
        assert( m_collisionPtr );

        m_collisionPtr->setLocalQuat( numpyToVec4( localQuaternion ) );
    }

    void PyCollision::setLocalTransform( py::array_t<TScalar>& localTransform )
    {
        assert( m_collisionPtr );

        m_collisionPtr->setLocalTransform( numpyToMat4( localTransform ) );
    }

    void PyCollision::changeSize( py::array_t<TScalar>& newSize )
    {
        assert( m_collisionPtr );

        m_collisionPtr->changeSize( numpyToVec3( newSize ) );
    }

    std::string PyCollision::name()
    {
        assert( m_collisionPtr );

        return m_collisionPtr->name();
    }

    py::array_t<TScalar> PyCollision::pos()
    {
        assert( m_collisionPtr );

        return vec3ToNumpy( m_collisionPtr->pos() );
    }

    bool PyCollision::visible()
    {
        assert( m_collisionPtr );

        return m_collisionPtr->isVisible();
    }

    bool PyCollision::wireframe()
    {
        assert( m_collisionPtr );

        return m_collisionPtr->isWireframe();
    }

    py::array_t<TScalar> PyCollision::rot()
    {
        assert( m_collisionPtr );

        return mat3ToNumpy( m_collisionPtr->rot() );
    }

    py::array_t<TScalar> PyCollision::quat()
    {
        assert( m_collisionPtr );

        return vec4ToNumpy( m_collisionPtr->quat() );
    }

    py::array_t<TScalar> PyCollision::tf()
    {
        assert( m_collisionPtr );

        return mat4ToNumpy( m_collisionPtr->tf() );
    }

    py::array_t<TScalar> PyCollision::localPos()
    {
        assert( m_collisionPtr );

        return vec3ToNumpy( m_collisionPtr->localPos() );
    }

    py::array_t<TScalar> PyCollision::localRot()
    {
        assert( m_collisionPtr );

        return mat3ToNumpy( m_collisionPtr->localRot() );
    }

    py::array_t<TScalar> PyCollision::localTf()
    {
        assert( m_collisionPtr );

        return mat4ToNumpy( m_collisionPtr->localTf() );
    }

    py::array_t<TScalar> PyCollision::localQuat()
    {
        assert( m_collisionPtr );

        return vec4ToNumpy( m_collisionPtr->localQuat() );
    }

    tysoc::eShapeType PyCollision::shape()
    {
        assert( m_collisionPtr );

        return m_collisionPtr->shape();
    }

    py::array_t<TScalar> PyCollision::size()
    {
        assert( m_collisionPtr );

        return vec3ToNumpy( m_collisionPtr->size() );
    }

    PyCollisionData PyCollision::data()
    {
        assert( m_collisionPtr );

        PyCollisionData _data;

        _data.type = m_collisionPtr->data().type;
        _data.size = m_collisionPtr->data().size;
        _data.localPos = m_collisionPtr->data().localPos;
        _data.localRot = m_collisionPtr->data().localRot;
        _data.filename = m_collisionPtr->data().filename;

        return _data;
    }

    PyBody* PyCollision::parent()
    {
        return m_parentPyBodyPtr;
    }

    tysoc::TCollision* PyCollision::ptr()
    {
        return m_collisionPtr;
    }

}