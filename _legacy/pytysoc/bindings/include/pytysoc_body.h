#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_collision.h>
#include <pytysoc_visual.h>

#include <components/body.h>

namespace py = pybind11;

namespace pytysoc
{

    // @todo: change usage of raw-pointers to unique_ptrs or shared_ptrs (check pybind constraints)

    class PyCollision;
    class PyVisual;

    class PyBody
    {

    public :

        /* @notexposed */
        PyBody( const std::string& name,
                const PyBodyData& data );

        /* @notexposed */
        virtual ~PyBody();

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
        py::array_t<TScalar> pos() const;

        /* @exposed */
        py::array_t<TScalar> rot() const;

        /* @exposed */
        py::array_t<TScalar> quat() const;

        /* @exposed */
        py::array_t<TScalar> euler() const;

        /* @exposed */
        py::array_t<TScalar> tf() const;

        /* @exposed */
        py::array_t<TScalar> pos0() const;

        /* @exposed */
        py::array_t<TScalar> rot0() const;

        /* @exposed */
        py::array_t<TScalar> quat0() const;

        /* @exposed */
        py::array_t<TScalar> euler0() const;

        /* @exposed */
        py::array_t<TScalar> tf0() const;

        /* @exposed */
        std::string name() const;

        /* @exposed */
        tysoc::eBodyClassType classType() const;

        /* @exposed */
        tysoc::eDynamicsType dyntype() const;

        /* @exposed */
        PyBodyData data() const;

        /* @exposed */
        PyVisual* visual() const { return m_pyVisual; }

        /* @exposed */
        PyCollision* collision() const { return m_pyCollision; }

        /* @notexposed */
        tysoc::TIBody* ptr() const { return m_bodyRef; }

    protected :

        tysoc::TIBody* m_bodyRef;

        PyCollision* m_pyCollision;

        PyVisual* m_pyVisual;
    };

}

#define PYTYSOC_INTERFACE_BODY_BINDINGS(m) \
    py::class_<pytysoc::PyBody>(m, "PyBody") \
        .def_property( "pos", &pytysoc::PyBody::pos, &pytysoc::PyBody::setPosition ) \
        .def_property( "rot", &pytysoc::PyBody::rot, &pytysoc::PyBody::setRotation ) \
        .def_property( "quat", &pytysoc::PyBody::quat, &pytysoc::PyBody::setQuaternion ) \
        .def_property( "euler", &pytysoc::PyBody::euler, &pytysoc::PyBody::setEuler ) \
        .def_property( "tf", &pytysoc::PyBody::tf, &pytysoc::PyBody::setTransform ) \
        .def_property_readonly( "name", &pytysoc::PyBody::name ) \
        .def_property_readonly( "classType", &pytysoc::PyBody::classType ) \
        .def_property_readonly( "dyntype", &pytysoc::PyBody::dyntype ) \
        .def_property_readonly( "data", &pytysoc::PyBody::data ) \
        .def_property( "visual", &pytysoc::PyBody::visual, &pytysoc::PyBody::setVisual ) \
        .def_property( "collision", &pytysoc::PyBody::collision, &pytysoc::PyBody::setCollision );
