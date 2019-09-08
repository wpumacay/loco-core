
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_body.h>

#include <components/visuals.h>

namespace py = pybind11;

namespace pytysoc
{

    class PyBody;

    class PyVisual
    {

    public :

        PyVisual( const std::string& name,
                  const PyVisualData& visualData );

        PyVisual( tysoc::TVisual* visualPtr );

        ~PyVisual();

        void setParentBody( PyBody* parenPyBodyPtr );

        void show( bool visible );

        void setLocalPosition( py::array_t<TScalar>& localPosition );

        void setLocalRotation( py::array_t<TScalar>& rotation );

        void setLocalQuat( py::array_t<TScalar>& localQuaternion );

        void setLocalTransform( py::array_t<TScalar>& transform );

        void changeSize( py::array_t<TScalar>& newSize );

        void changeColor( py::array_t<TScalar>& newFullColor );

        void changeAmbientColor( py::array_t<TScalar>& newAmbientColor );

        void changeDiffuseColor( py::array_t<TScalar>& newDiffuseColor );

        void changeSpecularColor( py::array_t<TScalar>& newSpecularColor );

        void changeShininess( const TScalar& shininess );

        std::string name();

        py::array_t<TScalar> pos();

        py::array_t<TScalar> rot();

        py::array_t<TScalar> quat();

        py::array_t<TScalar> tf();

        py::array_t<TScalar> localPos();

        py::array_t<TScalar> localRot();

        py::array_t<TScalar> localQuat();

        py::array_t<TScalar> localTf();

        tysoc::eShapeType shape();

        py::array_t<TScalar> size();

        PyVisualData data();

        PyBody* parent();

        tysoc::TVisual* ptr();

    private :

        PyBody* m_parentPyBodyPtr;

        tysoc::TVisual* m_visualPtr;

    };

}