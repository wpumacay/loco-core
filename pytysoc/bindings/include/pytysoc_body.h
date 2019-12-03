
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_collision.h>
#include <pytysoc_visual.h>

#include <components/body.h>

namespace py = pybind11;

namespace pytysoc
{

    class PyCollision;
    class PyVisual;

    class PyBody
    {

    public :

        /* @exposed */
        PyBody( const std::string& name,
                const PyBodyData& data,
                py::array_t<TScalar>& xyz,
                py::array_t<TScalar>& rpy );

        /* @notexposed */
        PyBody( tysoc::TBody* bodyPtr );

        /* @notexposed */
        ~PyBody();

        /* @exposed */
        void setCollision( PyCollision* pyCollisionPtr );

        /* @exposed */
        void setVisual( PyVisual* pyVisualPtr );

        /* @exposed */
        void setPosition( py::array_t<TScalar>& position );

        /* @exposed */
        void setRotation( py::array_t<TScalar>& rotation );

        /* @exposed */
        void setEuler( py::array_t<TScalar>& rpy );

        /* @exposed */
        void setQuaternion( py::array_t<TScalar>& quat );

        /* @exposed */
        void setTransform( py::array_t<TScalar>& transform );

        /* @exposed */
        std::string name();

        /* @exposed */
        py::array_t<TScalar> pos();

        /* @exposed */
        py::array_t<TScalar> rot();

        /* @exposed */
        py::array_t<TScalar> quat();

        /* @exposed */
        py::array_t<TScalar> euler();

        /* @exposed */
        py::array_t<TScalar> tf();

        /* @exposed */
        py::array_t<TScalar> pos0();

        /* @exposed */
        py::array_t<TScalar> rot0();

        /* @exposed */
        py::array_t<TScalar> quat0();

        /* @exposed */
        py::array_t<TScalar> euler0();

        /* @exposed */
        py::array_t<TScalar> tf0();

        /* @exposed */
        tysoc::eDynamicsType dyntype();

        /* @exposed */
        PyBodyData data();

        /* @exposed */
        PyVisual* visual() { return m_pyVisual; }

        /* @exposed */
        PyCollision* collision() { return m_pyCollision; }

        /* @notexposed */
        tysoc::TBody* ptr() { return m_bodyPtr; }

    private :

        tysoc::TBody* m_bodyPtr;

        PyCollision* m_pyCollision;
        PyVisual* m_pyVisual;

    };

}

#define PYTYSOC_BODY_BINDINGS(m) \
    py::class_<pytysoc::PyBody>(m, "PyBody") \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, py::array_t<TScalar>&, py::array_t<TScalar>&>() ) \
        .def_property( "pos", &pytysoc::PyBody::pos, &pytysoc::PyBody::setPosition ) \
        .def_property( "rot", &pytysoc::PyBody::rot, &pytysoc::PyBody::setRotation ) \
        .def_property( "quat", &pytysoc::PyBody::quat, &pytysoc::PyBody::setEuler ) \
        .def_property( "euler", &pytysoc::PyBody::euler, &pytysoc::PyBody::setQuaternion ) \
        .def_property( "tf", &pytysoc::PyBody::tf, &pytysoc::PyBody::setTransform ) \
        .def_property_readonly( "dyntype", &pytysoc::PyBody::dyntype ) \
        .def_property_readonly( "name", &pytysoc::PyBody::name ) \
        .def_property_readonly( "data", &pytysoc::PyBody::data ) \
        .def( "setVisual", &pytysoc::PyBody::setVisual ) \
        .def( "setCollision", &pytysoc::PyBody::setCollision ) \
        .def( "visual", &pytysoc::PyBody::visual ) \
        .def( "collision", &pytysoc::PyBody::collision );
