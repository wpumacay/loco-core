
#include <pytysoc_collision.h>

namespace py = pybind11;

namespace pytysoc
{

    PyCollision::PyCollision( const std::string& name,
                              const PyCollisionData& data )
    {
        m_parentPyBodyRef = nullptr;
        m_collisionRef = new tysoc::TCollision( name, data );
    }

    PyCollision::PyCollision( tysoc::TCollision* collisionRef )
    {
        m_parentPyBodyRef = nullptr;
        m_collisionRef = collisionRef;
    }

    PyCollision::~PyCollision()
    {
        m_collisionRef = nullptr;
        m_parentPyBodyRef = nullptr;
    }

    void PyCollision::setParentBody( PyBody* parentPyBodyRef )
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::setParentBody() >>> wrapped internal-collision is null" );

        m_parentPyBodyRef = parentPyBodyRef;
        m_collisionRef->setParentBody( m_parentPyBodyRef->ptr() );
    }

    void PyCollision::setVisibility( bool visible )
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::setVisibility() >>> wrapped internal-collision is null" );

        m_collisionRef->show( visible );
    }

    void PyCollision::setWireframe( bool wireframe )
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::setWireframe() >>> wrapped internal-collision is null" );

        m_collisionRef->wireframe( wireframe );
    }

    void PyCollision::setLocalPosition( py::array_t<TScalar>& localPosition )
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::setLocalPosition() >>> wrapped internal-collision is null" );

        m_collisionRef->setLocalPosition( numpyToVec3( localPosition ) );
    }

    void PyCollision::setLocalRotation( py::array_t<TScalar>& localRotation )
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::setLocalRotation() >>> wrapped internal-collision is null" );

        m_collisionRef->setLocalRotation( numpyToMat3( localRotation ) );
    }

    void PyCollision::setLocalQuat( py::array_t<TScalar>& localQuaternion )
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::setLocalQuat() >>> wrapped internal-collision is null" );

        m_collisionRef->setLocalQuat( numpyToVec4( localQuaternion ) );
    }

    void PyCollision::setLocalTransform( py::array_t<TScalar>& localTransform )
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::setLocalTransform() >>> wrapped internal-collision is null" );

        m_collisionRef->setLocalTransform( numpyToMat4( localTransform ) );
    }

    void PyCollision::changeSize( py::array_t<TScalar>& newSize )
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::changeSize() >>> wrapped internal-collision is null" );

        m_collisionRef->changeSize( numpyToVec3( newSize ) );
    }

    std::string PyCollision::name() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::name() >>> wrapped internal-collision is null" );

        return m_collisionRef->name();
    }

    py::array_t<TScalar> PyCollision::pos() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::pos() >>> wrapped internal-collision is null" );

        return vec3ToNumpy( m_collisionRef->pos() );
    }

    bool PyCollision::visible() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::visible() >>> wrapped internal-collision is null" );

        return m_collisionRef->isVisible();
    }

    bool PyCollision::wireframe() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::wireframe() >>> wrapped internal-collision is null" );

        return m_collisionRef->isWireframe();
    }

    py::array_t<TScalar> PyCollision::rot() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::rot() >>> wrapped internal-collision is null" );

        return mat3ToNumpy( m_collisionRef->rot() );
    }

    py::array_t<TScalar> PyCollision::quat() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::quat() >>> wrapped internal-collision is null" );

        return vec4ToNumpy( m_collisionRef->quat() );
    }

    py::array_t<TScalar> PyCollision::tf() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::tf() >>> wrapped internal-collision is null" );

        return mat4ToNumpy( m_collisionRef->tf() );
    }

    py::array_t<TScalar> PyCollision::localPos() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::localPos() >>> wrapped internal-collision is null" );

        return vec3ToNumpy( m_collisionRef->localPos() );
    }

    py::array_t<TScalar> PyCollision::localRot() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::localRot() >>> wrapped internal-collision is null" );

        return mat3ToNumpy( m_collisionRef->localRot() );
    }

    py::array_t<TScalar> PyCollision::localTf() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::localTf() >>> wrapped internal-collision is null" );

        return mat4ToNumpy( m_collisionRef->localTf() );
    }

    py::array_t<TScalar> PyCollision::localQuat() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::localQuat() >>> wrapped internal-collision is null" );

        return vec4ToNumpy( m_collisionRef->localQuat() );
    }

    tysoc::eShapeType PyCollision::shape() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::shape() >>> wrapped internal-collision is null" );

        return m_collisionRef->shape();
    }

    py::array_t<TScalar> PyCollision::size() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::size() >>> wrapped internal-collision is null" );

        return vec3ToNumpy( m_collisionRef->size() );
    }

    PyCollisionData PyCollision::data() const
    {
        LOCO_CORE_ASSERT( m_collisionRef, "PyCollision::data() >>> wrapped internal-collision is null" );

        PyCollisionData _data;

        _data.type = m_collisionRef->data().type;
        _data.size = m_collisionRef->data().size;
        _data.localTransform = m_collisionRef->data().localTransform;
        _data.filename = m_collisionRef->data().filename;

        return _data;
    }

}