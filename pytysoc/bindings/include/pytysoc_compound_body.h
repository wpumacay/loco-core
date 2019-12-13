#pragma once

#include <pytysoc_body.h>
#include <pytysoc_joint.h>

#include <compounds/compound_body.h>

namespace py = pybind11;

namespace pytysoc
{

    // @todo: change usage of raw-pointers to unique_ptrs or shared_ptrs (check pybind constraints)

    class PyCollision;
    class PyVisual;
    class PyJoint;

    class PyCompoundBody : public PyBody
    {

    public :

        /* @exposed */
        PyCompoundBody( const std::string& name,
                        const PyBodyData& data,
                        const PyJointData& jointData,
                        PyCompoundBody* parentRef,
                        py::array_t<TScalar>& localXyz,
                        py::array_t<TScalar>& localRpy );

        /* @exposed */
        PyCompoundBody( const std::string& name,
                        const PyBodyData& data,
                        const PyJointData& jointData,
                        py::array_t<TScalar>& xyz,
                        py::array_t<TScalar>& rpy );

        /* @exposed */
        PyCompoundBody( const std::string& name,
                        const PyBodyData& data,
                        py::array_t<TScalar>& xyz,
                        py::array_t<TScalar>& rpy,
                        const tysoc::eDynamicsType& dyntype );

        /* @notexposed */
        ~PyCompoundBody();

        /* @exposed */
        PyJoint* joint() const { return m_pyJoint; }

        /* @exposed */
        PyCompoundBody* parent() const { return m_pyParentRef; }

    private :

        PyJoint* m_pyJoint;

        PyCompoundBody* m_pyParentRef;
    };

}

#define PYTYSOC_COMPOUND_BODY_BINDINGS(m) \
    py::class_<pytysoc::PyCompoundBody, pytysoc::PyBody>(m, "PyCompoundBody") \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, const pytysoc::PyJointData&, pytysoc::PyCompoundBody*, py::array_t<TScalar>&, py::array_t<TScalar>&>() ) \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, const pytysoc::PyJointData&, py::array_t<TScalar>&, py::array_t<TScalar>&>() ) \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, py::array_t<TScalar>&, py::array_t<TScalar>&, const tysoc::eDynamicsType&>() ) \
        .def( "joint", &pytysoc::PyCompoundBody::joint, py::return_value_policy::reference ) \
        .def( "parent", &pytysoc::PyCompoundBody::parent, py::return_value_policy::reference );
