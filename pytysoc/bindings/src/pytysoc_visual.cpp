
#include <pytysoc_visual.h>

namespace py = pybind11;

namespace pytysoc
{

    PyVisual::PyVisual( const std::string& name,
                        const PyVisualData& data )
    {
        m_parentPyBodyPtr = nullptr;
        m_visualPtr = new tysoc::TVisual( name, data );
    }

    PyVisual::PyVisual( tysoc::TVisual* visualPtr )
    {
        m_parentPyBodyPtr = nullptr;
        m_visualPtr = visualPtr;
    }

    PyVisual::~PyVisual()
    {
        m_visualPtr = nullptr;
        m_parentPyBodyPtr = nullptr;
    }

    void PyVisual::setParentBody( PyBody* parentPyBodyPtr )
    {
        assert( m_visualPtr );

        m_parentPyBodyPtr = parentPyBodyPtr;
        m_visualPtr->setParentBody( m_parentPyBodyPtr->ptr() );
    }

    void PyVisual::show( bool visible )
    {
        assert( m_visualPtr );

        m_visualPtr->show( visible );
    }

    void PyVisual::setLocalPosition( py::array_t<TScalar>& localPosition )
    {
        assert( m_visualPtr );

        m_visualPtr->setLocalPosition( numpyToVec3( localPosition ) );
    }

    void PyVisual::setLocalRotation( py::array_t<TScalar>& localRotation )
    {
        assert( m_visualPtr );

        m_visualPtr->setLocalRotation( numpyToMat3( localRotation ) );
    }

    void PyVisual::setLocalQuat( py::array_t<TScalar>& localQuaternion )
    {
        assert( m_visualPtr );

        m_visualPtr->setLocalQuat( numpyToVec4( localQuaternion ) );
    }

    void PyVisual::setLocalTransform( py::array_t<TScalar>& localTransform )
    {
        assert( m_visualPtr );

        m_visualPtr->setLocalTransform( numpyToMat4( localTransform ) );
    }

    void PyVisual::changeSize( py::array_t<TScalar>& newSize )
    {
        assert( m_visualPtr );

        m_visualPtr->changeSize( numpyToVec3( newSize ) );
    }

    void PyVisual::changeColor( py::array_t<TScalar>& newFullColor )
    {
        assert( m_visualPtr );

        m_visualPtr->changeColor( numpyToVec3( newFullColor ) );
    }

    void PyVisual::changeAmbientColor( py::array_t<TScalar>& newAmbientColor )
    {
        assert( m_visualPtr );

        m_visualPtr->changeAmbientColor( numpyToVec3( newAmbientColor ) );
    }

    void PyVisual::changeDiffuseColor( py::array_t<TScalar>& newDiffuseColor )
    {
        assert( m_visualPtr );

        m_visualPtr->changeDiffuseColor( numpyToVec3( newDiffuseColor ) );
    }

    void PyVisual::changeSpecularColor( py::array_t<TScalar>& newSpecularColor )
    {
        assert( m_visualPtr );

        m_visualPtr->changeSpecularColor( numpyToVec3( newSpecularColor ) );
    }

    void PyVisual::changeShininess( const TScalar& shininess )
    {
        assert( m_visualPtr );

        m_visualPtr->changeShininess( shininess );
    }

    std::string PyVisual::name()
    {
        assert( m_visualPtr );

        return m_visualPtr->name();
    }

    py::array_t<TScalar> PyVisual::pos()
    {
        assert( m_visualPtr );

        return vec3ToNumpy( m_visualPtr->pos() );
    }

    py::array_t<TScalar> PyVisual::rot()
    {
        assert( m_visualPtr );

        return mat3ToNumpy( m_visualPtr->rot() );
    }

    py::array_t<TScalar> PyVisual::quat()
    {
        assert( m_visualPtr );

        return vec4ToNumpy( m_visualPtr->quat() );
    }

    py::array_t<TScalar> PyVisual::tf()
    {
        assert( m_visualPtr );

        return mat4ToNumpy( m_visualPtr->tf() );
    }

    py::array_t<TScalar> PyVisual::localPos()
    {
        assert( m_visualPtr );

        return vec3ToNumpy( m_visualPtr->localPos() );
    }

    py::array_t<TScalar> PyVisual::localRot()
    {
        assert( m_visualPtr );

        return mat3ToNumpy( m_visualPtr->localRot() );
    }

    py::array_t<TScalar> PyVisual::localTf()
    {
        assert( m_visualPtr );

        return mat4ToNumpy( m_visualPtr->localTf() );
    }

    py::array_t<TScalar> PyVisual::localQuat()
    {
        assert( m_visualPtr );

        return vec4ToNumpy( m_visualPtr->localQuat() );
    }

    tysoc::eShapeType PyVisual::shape()
    {
        assert( m_visualPtr );

        return m_visualPtr->shape();
    }

    py::array_t<TScalar> PyVisual::size()
    {
        assert( m_visualPtr );

        return vec3ToNumpy( m_visualPtr->size() );
    }

    PyVisualData PyVisual::data()
    {
        assert( m_visualPtr );

        PyVisualData _data;

        _data.type = m_visualPtr->data().type;
        _data.size = m_visualPtr->data().size;
        _data.localPos = m_visualPtr->data().localPos;
        _data.localRot = m_visualPtr->data().localRot;
        _data.filename = m_visualPtr->data().filename;
        _data.ambient = m_visualPtr->data().ambient;
        _data.diffuse = m_visualPtr->data().diffuse;
        _data.specular = m_visualPtr->data().specular;
        _data.shininess = m_visualPtr->data().shininess;

        return _data;
    }

    PyBody* PyVisual::parent()
    {
        return m_parentPyBodyPtr;
    }

    tysoc::TVisual* PyVisual::ptr()
    {
        return m_visualPtr;
    }

}