
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_collision.h>
#include <pytysoc_visual.h>

#include <primitives/single_body.h>

namespace py = pybind11;

namespace pytysoc
{

    // @todo: change usage of raw-pointers to unique_ptrs or shared_ptrs (check pybind constraints)

    class PyCollision;
    class PyVisual;

    class PySingleBody
    {

    public :

        /* @exposed */
        PySingleBody( const std::string& name,
                      const PyBodyData& data,
                      py::array_t<TScalar>& xyz,
                      py::array_t<TScalar>& rpy );

        /* @notexposed */
        PySingleBody( tysoc::TSingleBody* bodyObj );

        /* @notexposed */
        ~PySingleBody();

        /* @exposed */
        void setCollision( PyCollision* pyCollisionObj );

        /* @exposed */
        void setVisual( PyVisual* pyVisualObj );

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
        tysoc::TSingleBody* ptr() { return m_body; }

    private :

        tysoc::TSingleBody* m_body;

        PyCollision* m_pyCollision;
        PyVisual* m_pyVisual;
    };

}

#define PYTYSOC_BODY_BINDINGS(m) \
    py::class_<pytysoc::PySingleBody>(m, "PySingleBody") \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, py::array_t<TScalar>&, py::array_t<TScalar>&>() ) \
        .def_property( "pos", &pytysoc::PySingleBody::pos, &pytysoc::PySingleBody::setPosition ) \
        .def_property( "rot", &pytysoc::PySingleBody::rot, &pytysoc::PySingleBody::setRotation ) \
        .def_property( "quat", &pytysoc::PySingleBody::quat, &pytysoc::PySingleBody::setEuler ) \
        .def_property( "euler", &pytysoc::PySingleBody::euler, &pytysoc::PySingleBody::setQuaternion ) \
        .def_property( "tf", &pytysoc::PySingleBody::tf, &pytysoc::PySingleBody::setTransform ) \
        .def_property_readonly( "dyntype", &pytysoc::PySingleBody::dyntype ) \
        .def_property_readonly( "name", &pytysoc::PySingleBody::name ) \
        .def_property_readonly( "data", &pytysoc::PySingleBody::data ) \
        .def( "setVisual", &pytysoc::PySingleBody::setVisual ) \
        .def( "setCollision", &pytysoc::PySingleBody::setCollision ) \
        .def( "visual", &pytysoc::PySingleBody::visual ) \
        .def( "collision", &pytysoc::PySingleBody::collision );
