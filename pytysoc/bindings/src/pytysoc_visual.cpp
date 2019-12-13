
#include <pytysoc_visual.h>

namespace py = pybind11;

namespace pytysoc
{

    PyVisual::PyVisual( const std::string& name,
                        const PyVisualData& data )
    {
        m_parentPyBodyRef = nullptr;
        m_visualRef = new tysoc::TVisual( name, data );
    }

    PyVisual::PyVisual( tysoc::TVisual* visualRef )
    {
        m_parentPyBodyRef = nullptr;
        m_visualRef = visualRef;
    }

    PyVisual::~PyVisual()
    {
        m_visualRef = nullptr;
        m_parentPyBodyRef = nullptr;
    }

    void PyVisual::setParentBody( PyBody* parentPyBodyRef )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::setParentBody() >>> wrapped internal-collision is null" );

        m_parentPyBodyRef = parentPyBodyRef;
        m_visualRef->setParentBody( m_parentPyBodyRef->ptr() );
    }

    void PyVisual::setVisibility( bool visible )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::setParentBody() >>> wrapped internal-collision is null" );

        m_visualRef->show( visible );
    }

    void PyVisual::setWireframe( bool wireframe )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::setWireframe() >>> wrapped internal-collision is null" );

        m_visualRef->wireframe( wireframe );
    }

    void PyVisual::setLocalPosition( py::array_t<TScalar>& localPosition )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::setLocalPosition() >>> wrapped internal-collision is null" );

        m_visualRef->setLocalPosition( numpyToVec3( localPosition ) );
    }

    void PyVisual::setLocalRotation( py::array_t<TScalar>& localRotation )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::setLocalRotation() >>> wrapped internal-collision is null" );

        m_visualRef->setLocalRotation( numpyToMat3( localRotation ) );
    }

    void PyVisual::setLocalQuat( py::array_t<TScalar>& localQuaternion )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::setLocalQuat() >>> wrapped internal-collision is null" );

        m_visualRef->setLocalQuat( numpyToVec4( localQuaternion ) );
    }

    void PyVisual::setLocalTransform( py::array_t<TScalar>& localTransform )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::setLocalTransform() >>> wrapped internal-collision is null" );

        m_visualRef->setLocalTransform( numpyToMat4( localTransform ) );
    }

    void PyVisual::changeSize( py::array_t<TScalar>& newSize )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::changeSize() >>> wrapped internal-collision is null" );

        m_visualRef->changeSize( numpyToVec3( newSize ) );
    }

    void PyVisual::changeColor( py::array_t<TScalar>& newFullColor )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::changeColor() >>> wrapped internal-collision is null" );

        m_visualRef->changeColor( numpyToVec3( newFullColor ) );
    }

    void PyVisual::changeAmbientColor( py::array_t<TScalar>& newAmbientColor )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::changeAmbientColor() >>> wrapped internal-collision is null" );

        m_visualRef->changeAmbientColor( numpyToVec3( newAmbientColor ) );
    }

    void PyVisual::changeDiffuseColor( py::array_t<TScalar>& newDiffuseColor )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::changeDiffuseColor() >>> wrapped internal-collision is null" );

        m_visualRef->changeDiffuseColor( numpyToVec3( newDiffuseColor ) );
    }

    void PyVisual::changeSpecularColor( py::array_t<TScalar>& newSpecularColor )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::changeSpecularColor() >>> wrapped internal-collision is null" );

        m_visualRef->changeSpecularColor( numpyToVec3( newSpecularColor ) );
    }

    void PyVisual::changeShininess( const TScalar& shininess )
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::changeShininess() >>> wrapped internal-collision is null" );

        m_visualRef->changeShininess( shininess );
    }

    std::string PyVisual::name() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::name() >>> wrapped internal-collision is null" );

        return m_visualRef->name();
    }

    bool PyVisual::visible() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::visible() >>> wrapped internal-collision is null" );

        return m_visualRef->isVisible();
    }

    bool PyVisual::wireframe() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::wireframe() >>> wrapped internal-collision is null" );

        return m_visualRef->isWireframe();
    }

    py::array_t<TScalar> PyVisual::pos() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::pos() >>> wrapped internal-collision is null" );

        return vec3ToNumpy( m_visualRef->pos() );
    }

    py::array_t<TScalar> PyVisual::rot() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::rot() >>> wrapped internal-collision is null" );

        return mat3ToNumpy( m_visualRef->rot() );
    }

    py::array_t<TScalar> PyVisual::quat() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::quat() >>> wrapped internal-collision is null" );

        return vec4ToNumpy( m_visualRef->quat() );
    }

    py::array_t<TScalar> PyVisual::tf() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::tf() >>> wrapped internal-collision is null" );

        return mat4ToNumpy( m_visualRef->tf() );
    }

    py::array_t<TScalar> PyVisual::localPos() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::localPos() >>> wrapped internal-collision is null" );

        return vec3ToNumpy( m_visualRef->localPos() );
    }

    py::array_t<TScalar> PyVisual::localRot() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::localRot() >>> wrapped internal-collision is null" );

        return mat3ToNumpy( m_visualRef->localRot() );
    }

    py::array_t<TScalar> PyVisual::localTf() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::localTf() >>> wrapped internal-collision is null" );

        return mat4ToNumpy( m_visualRef->localTf() );
    }

    py::array_t<TScalar> PyVisual::localQuat() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::localQuat() >>> wrapped internal-collision is null" );

        return vec4ToNumpy( m_visualRef->localQuat() );
    }

    tysoc::eShapeType PyVisual::shape() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::shape() >>> wrapped internal-collision is null" );

        return m_visualRef->shape();
    }

    py::array_t<TScalar> PyVisual::size() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::size() >>> wrapped internal-collision is null" );

        return vec3ToNumpy( m_visualRef->size() );
    }

    PyVisualData PyVisual::data() const
    {
        TYSOC_CORE_ASSERT( m_visualRef, "PyVisual::data() >>> wrapped internal-collision is null" );

        PyVisualData _data;

        _data.type = m_visualRef->data().type;
        _data.size = m_visualRef->data().size;
        _data.localTransform = m_visualRef->data().localTransform;
        _data.filename = m_visualRef->data().filename;
        _data.ambient = m_visualRef->data().ambient;
        _data.diffuse = m_visualRef->data().diffuse;
        _data.specular = m_visualRef->data().specular;
        _data.shininess = m_visualRef->data().shininess;

        return _data;
    }

}