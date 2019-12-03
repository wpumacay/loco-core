
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_body.h>

#include <components/collision.h>

namespace py = pybind11;

namespace pytysoc
{

    class PyBody;

    class PyCollision
    {

    public :

        PyCollision( const std::string& name,
                     const PyCollisionData& data );

        PyCollision( tysoc::TCollision* collisionPtr );

        ~PyCollision();

        void setParentBody( PyBody* parentPyBodyPtr );

        void setVisibility( bool visible );

        void setWireframe( bool wireframe );

        void setLocalPosition( py::array_t<TScalar>& localPosition );

        void setLocalRotation( py::array_t<TScalar>& localRotation );

        void setLocalQuat( py::array_t<TScalar>& localQuaternion );

        void setLocalTransform( py::array_t<TScalar>& localTransform );

        void changeSize( py::array_t<TScalar>& newSize );

        std::string name();

        bool visible();

        bool wireframe();

        py::array_t<TScalar> pos();

        py::array_t<TScalar> rot();

        py::array_t<TScalar> quat();

        py::array_t<TScalar> tf();

        py::array_t<TScalar> localPos();

        py::array_t<TScalar> localRot();

        py::array_t<TScalar> localTf();

        py::array_t<TScalar> localQuat();

        tysoc::eShapeType shape();

        py::array_t<TScalar> size();

        PyCollisionData data();

        PyBody* parent();

        tysoc::TCollision* ptr();

    private :

        PyBody* m_parentPyBodyPtr;

        tysoc::TCollision* m_collisionPtr;

    };

}

#define PYTYSOC_COLLISION_BINDINGS(m) \
    py::class_<pytysoc::PyCollision>(m, "PyCollision") \
        .def( py::init<const std::string&, const pytysoc::PyCollisionData&>() ) \
        .def_property( "parent", &pytysoc::PyCollision::parent, &pytysoc::PyCollision::setParentBody ) \
        .def_property( "visible", &pytysoc::PyCollision::visible, &pytysoc::PyCollision::setVisibility ) \
        .def_property( "wireframe", &pytysoc::PyCollision::wireframe, &pytysoc::PyCollision::setWireframe ) \
        .def_property( "localPos", &pytysoc::PyCollision::localPos, &pytysoc::PyCollision::setLocalPosition ) \
        .def_property( "localRot", &pytysoc::PyCollision::localRot, &pytysoc::PyCollision::setLocalRotation ) \
        .def_property( "localQuat", &pytysoc::PyCollision::localQuat, &pytysoc::PyCollision::setLocalQuat ) \
        .def_property( "localTf", &pytysoc::PyCollision::localTf, &pytysoc::PyCollision::setLocalTransform ) \
        .def_property( "size", &pytysoc::PyCollision::size, &pytysoc::PyCollision::changeSize ) \
        .def_property_readonly( "name", &pytysoc::PyCollision::name ) \
        .def_property_readonly( "pos", &pytysoc::PyCollision::pos ) \
        .def_property_readonly( "rot", &pytysoc::PyCollision::rot ) \
        .def_property_readonly( "quat", &pytysoc::PyCollision::quat ) \
        .def_property_readonly( "tf", &pytysoc::PyCollision::tf ) \
        .def_property_readonly( "shape", &pytysoc::PyCollision::shape ) \
        .def_property_readonly( "data", &pytysoc::PyCollision::data );
