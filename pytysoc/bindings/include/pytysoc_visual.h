
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_body.h>

#include <components/visual.h>

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

        void setVisibility( bool visible );

        void setWireframe( bool wireframe );

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

        bool visible();

        bool wireframe();

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

#define PYTYSOC_VISUAL_BINDINGS(m) \
    py::class_<pytysoc::PyVisual>(m, "PyVisual") \
        .def( py::init<const std::string&, const pytysoc::PyVisualData&>() ) \
        .def_property( "parent", &pytysoc::PyVisual::parent, &pytysoc::PyVisual::setParentBody ) \
        .def_property( "visible", &pytysoc::PyVisual::visible, &pytysoc::PyVisual::setVisibility ) \
        .def_property( "wireframe", &pytysoc::PyVisual::wireframe, &pytysoc::PyVisual::setWireframe ) \
        .def_property( "localPos", &pytysoc::PyVisual::localPos, &pytysoc::PyVisual::setLocalPosition ) \
        .def_property( "localRot", &pytysoc::PyVisual::localRot, &pytysoc::PyVisual::setLocalRotation ) \
        .def_property( "localQuat", &pytysoc::PyVisual::localQuat, &pytysoc::PyVisual::setLocalQuat ) \
        .def_property( "localTf", &pytysoc::PyVisual::localTf, &pytysoc::PyVisual::setLocalTransform ) \
        .def_property( "size", &pytysoc::PyVisual::size, &pytysoc::PyVisual::changeSize ) \
        .def_property_readonly( "name", &pytysoc::PyVisual::name ) \
        .def_property_readonly( "pos", &pytysoc::PyVisual::pos ) \
        .def_property_readonly( "rot", &pytysoc::PyVisual::rot ) \
        .def_property_readonly( "quat", &pytysoc::PyVisual::quat ) \
        .def_property_readonly( "tf", &pytysoc::PyVisual::tf ) \
        .def_property_readonly( "shape", &pytysoc::PyVisual::shape ) \
        .def_property_readonly( "data", &pytysoc::PyVisual::data ) \
        .def( "changeColor", &pytysoc::PyVisual::changeColor ) \
        .def( "changeAmbientColor", &pytysoc::PyVisual::changeAmbientColor ) \
        .def( "changeDiffuseColor", &pytysoc::PyVisual::changeDiffuseColor ) \
        .def( "changeSpecularColor", &pytysoc::PyVisual::changeSpecularColor ) \
        .def( "changeShininess", &pytysoc::PyVisual::changeShininess );
