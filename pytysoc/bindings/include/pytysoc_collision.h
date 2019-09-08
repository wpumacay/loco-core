
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_body.h>

#include <components/collisions.h>

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

        void show( bool visible );

        void setLocalPosition( py::array_t<TScalar>& localPosition );

        void setLocalRotation( py::array_t<TScalar>& localRotation );

        void setLocalQuat( py::array_t<TScalar>& localQuaternion );

        void setLocalTransform( py::array_t<TScalar>& localTransform );

        void changeSize( py::array_t<TScalar>& newSize );

        std::string name();

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