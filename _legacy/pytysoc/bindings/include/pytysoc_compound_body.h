#pragma once

#include <pytysoc_body.h>
#include <pytysoc_joint.h>
#include <pytysoc_compound.h>

#include <compounds/compound_body.h>
#include <compounds/compound.h>

namespace py = pybind11;

namespace pytysoc
{

    // @todo: change usage of raw-pointers to unique_ptrs or shared_ptrs (check pybind constraints)

    class PyCollision;
    class PyVisual;
    class PyJoint;
    class PyCompound;

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
        PyCompoundBody( tysoc::TCompoundBody* bodyRef );

        /* @notexposed */
        ~PyCompoundBody();

        /* @exposed */
        void setCompound( PyCompound* pyCompoundRef );

        /* @exposed */
        void setParent( PyCompoundBody* pyParentRef );

        /* @exposed */
        std::pair<PyCompoundBody*, PyJoint* > addBodyJointPair( const std::string& name,
                                                                const PyBodyData& bodyData,
                                                                const PyJointData& jointData,
                                                                py::array_t<TScalar>& bodyLocalTransform );

        /* @exposed */
        std::pair<PyCompoundBody*, PyJoint* > addBodyJointPair( const std::string& name,
                                                                const tysoc::eShapeType& bodyShape,
                                                                py::array_t<TScalar>& bodySize,
                                                                py::array_t<TScalar>& bodyColor,
                                                                py::array_t<TScalar>& bodyLocalTransform,
                                                                const tysoc::eJointType& jointType,
                                                                py::array_t<TScalar>& jointAxis,
                                                                py::array_t<TScalar>& jointLimits,
                                                                py::array_t<TScalar>& jointLocalTransform );

        bool isRoot() const { return m_pyParentRef == nullptr; }

        /* @exposed */
        PyCompound* compound() const { return m_pyCompoundRef; }

        /* @exposed */
        PyJoint* joint() const { return m_pyJoint; }

        /* @exposed */
        PyCompoundBody* parent() const { return m_pyParentRef; }

        /* @exposed */
        std::vector<PyCompoundBody*> children() const { return m_pyChildrenRefs; }

    private :

        PyCompound* m_pyCompoundRef;

        PyJoint* m_pyJoint;

        PyCompoundBody* m_pyParentRef;

        std::vector< PyCompoundBody* > m_pyChildrenRefs;
    };

}

#define PYTYSOC_COMPOUND_BODY_BINDINGS(m) \
    py::class_<pytysoc::PyCompoundBody, pytysoc::PyBody>(m, "PyCompoundBody") \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, const pytysoc::PyJointData&, pytysoc::PyCompoundBody*, py::array_t<TScalar>&, py::array_t<TScalar>&>() ) \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, const pytysoc::PyJointData&, py::array_t<TScalar>&, py::array_t<TScalar>&>() ) \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, py::array_t<TScalar>&, py::array_t<TScalar>&, const tysoc::eDynamicsType&>() ) \
        .def( "setCompound", &pytysoc::PyCompoundBody::setCompound ) \
        .def( "setParent", &pytysoc::PyCompoundBody::setParent ) \
        .def( "addBodyJointPair", ( std::pair<pytysoc::PyCompoundBody*, pytysoc::PyJoint*> (pytysoc::PyCompoundBody::*)\
                                    (const std::string&, const pytysoc::PyBodyData&, const pytysoc::PyJointData&, py::array_t<TScalar>&) )\
                                        &pytysoc::PyCompoundBody::addBodyJointPair ) \
        .def( "addBodyJointPair", ( std::pair<pytysoc::PyCompoundBody*, pytysoc::PyJoint*> (pytysoc::PyCompoundBody::*)\
                                    (const std::string&, const tysoc::eShapeType&, py::array_t<TScalar>&, py::array_t<TScalar>&, py::array_t<TScalar>&,\
                                     const tysoc::eJointType&, py::array_t<TScalar>&, py::array_t<TScalar>&, py::array_t<TScalar>&) )\
                                        &pytysoc::PyCompoundBody::addBodyJointPair ) \
        .def( "isRoot", &pytysoc::PyCompoundBody::isRoot ) \
        .def( "compound", &pytysoc::PyCompoundBody::compound, py::return_value_policy::reference ) \
        .def( "joint", &pytysoc::PyCompoundBody::joint, py::return_value_policy::reference ) \
        .def( "parent", &pytysoc::PyCompoundBody::parent, py::return_value_policy::reference ) \
        .def( "children", &pytysoc::PyCompoundBody::children );
