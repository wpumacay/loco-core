
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_collision.h>
#include <pytysoc_visual.h>

#include <components/bodies.h>

namespace py = pybind11;

namespace pytysoc
{

    class PyCollision;
    class PyVisual;

    class PyBody
    {

    public :

        PyBody( const std::string& name,
                const PyBodyData& data,
                py::array_t<TScalar>& xyz,
                py::array_t<TScalar>& rpy );

        PyBody( tysoc::TBody* bodyPtr );

        ~PyBody();

        void addCollision( PyCollision* pyCollisionPtr );

        void addVisual( PyVisual* pyVisualPtr );

        void setPosition( py::array_t<TScalar>& position );

        void setRotation( py::array_t<TScalar>& rotation );

        void setEuler( py::array_t<TScalar>& rpy );

        void setQuaternion( py::array_t<TScalar>& quat );

        void setTransform( py::array_t<TScalar>& transform );

        std::string name();

        py::array_t<TScalar> pos();

        py::array_t<TScalar> rot();

        py::array_t<TScalar> quat();

        py::array_t<TScalar> euler();

        py::array_t<TScalar> tf();

        py::array_t<TScalar> pos0();

        py::array_t<TScalar> rot0();

        py::array_t<TScalar> quat0();

        py::array_t<TScalar> euler0();

        py::array_t<TScalar> tf0();

        tysoc::eDynamicsType dyntype();

        PyBodyData data();

        std::vector< PyVisual* > visuals() { return m_pyVisuals; }

        std::vector< PyCollision* > collisions() { return m_pyCollisions; }

        tysoc::TBody* ptr() { return m_bodyPtr; }

    private :

        tysoc::TBody* m_bodyPtr;

        std::vector< PyCollision* > m_pyCollisions;
        std::vector< PyVisual* > m_pyVisuals;

    };


}