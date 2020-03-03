#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_joint.h>
#include <pytysoc_compound_body.h>

#include <compounds/compound.h>

namespace py = pybind11;

namespace pytysoc
{

    // @todo: change usage of raw-pointers to unique_ptrs or shared_ptrs (check pybind constraints)
    class PyJoint;
    class PyCompoundBody;

    class PyCompound
    {

    public :

        /* @exposed */
        PyCompound( const std::string& name,
                    py::array_t<TScalar>& xyz,
                    py::array_t<TScalar>& rpy,
                    const tysoc::eDynamicsType& dyntype );

        /* @notexposed */
        ~PyCompound();

        /* @exposed */
        PyCompoundBody* createRootBody( const std::string& name,
                                        const PyBodyData& bodyData,
                                        py::array_t<TScalar>& localTransform );

        /* @exposed */
        PyCompoundBody* createRootBody( const std::string& name,
                                        const tysoc::eShapeType& shape,
                                        py::array_t<TScalar>& size,
                                        py::array_t<TScalar>& color,
                                        py::array_t<TScalar>& localTransform );

        /* @exposed */
        std::pair< PyCompoundBody*, PyJoint* > createRootBodyJointPair( const std::string& bodyName,
                                                                        const PyBodyData& bodyData,
                                                                        const PyJointData& jointData,
                                                                        py::array_t<TScalar>& localTransform );

        /* @exposed */
        std::pair< PyCompoundBody*, PyJoint* > createRootBodyJointPair( const std::string& bodyName,
                                                                        const tysoc::eShapeType& bodyShape,
                                                                        py::array_t<TScalar>& bodySize,
                                                                        py::array_t<TScalar>& bodyColor,
                                                                        py::array_t<TScalar>& bodyLocalTransform,
                                                                        const tysoc::eJointType& jointType,
                                                                        py::array_t<TScalar>& jointAxis,
                                                                        py::array_t<TScalar>& jointLimits,
                                                                        py::array_t<TScalar>& jointLocalTransform );

        /* @exposed */
        void addCompoundBody( PyCompoundBody* pyCompoundBody );

        /* @exposed */
        void setPosition( py::array_t<TScalar>& xyz );

        /* @exposed */
        void setRotationMat( py::array_t<TScalar>& rotmat );

        /* @exposed */
        void setRotationEuler( py::array_t<TScalar>& rpy );

        /* @exposed */
        void setRotationQuaternion( py::array_t<TScalar>& quat );

        /* @exposed */
        void setTransform( py::array_t<TScalar>& tf );

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
        tysoc::eDynamicsType dyntype() const;

        /* @exposed */
        PyCompoundBody* getBodyByName( const std::string& name );

        /* @exposed */
        std::vector< PyCompoundBody* > bodies() const { return m_pyBodiesRefs; }

        /* @exposed */
        PyCompoundBody* root() const { return m_pyRootBodyRef; }

        /* @notexposed */
        tysoc::TCompound* ptr() const { return m_compoundRef; }

    private :

        tysoc::TCompound* m_compoundRef;

        PyCompoundBody* m_pyRootBodyRef;

        std::vector< PyCompoundBody* > m_pyBodiesRefs;
    };
}

#define PYTYSOC_COMPOUND_BINDINGS(m) \
    py::class_<pytysoc::PyCompound>(m, "PyCompound" ) \
        .def( py::init<const std::string&, py::array_t<TScalar>&, py::array_t<TScalar>&, const tysoc::eDynamicsType&>() ) \
        .def( "createRootBody", ( pytysoc::PyCompoundBody* (pytysoc::PyCompound::*)\
                                    (const std::string&, const pytysoc::PyBodyData&, py::array_t<TScalar>&) )\
                                        &pytysoc::PyCompound::createRootBody ) \
        .def( "createRootBody", ( pytysoc::PyCompoundBody* (pytysoc::PyCompound::*)\
                                    (const std::string& , const tysoc::eShapeType& , py::array_t<TScalar>&, py::array_t<TScalar>&, py::array_t<TScalar>&) )\
                                        &pytysoc::PyCompound::createRootBody ) \
        .def( "createRootBodyJointPair", ( std::pair<pytysoc::PyCompoundBody*,pytysoc::PyJoint*> (pytysoc::PyCompound::*)\
                                            (const std::string&, const pytysoc::PyBodyData&, const pytysoc::PyJointData&, py::array_t<TScalar>&) )\
                                                &pytysoc::PyCompound::createRootBodyJointPair ) \
        .def( "createRootBodyJointPair", ( std::pair<pytysoc::PyCompoundBody*,pytysoc::PyJoint*> (pytysoc::PyCompound::*)\
                                            (const std::string&, const tysoc::eShapeType&, py::array_t<TScalar>&, \
                                             py::array_t<TScalar>&, py::array_t<TScalar>&, const tysoc::eJointType&, \
                                             py::array_t<TScalar>&, py::array_t<TScalar>&, py::array_t<TScalar>&) )\
                                                &pytysoc::PyCompound::createRootBodyJointPair ) \
        .def( "addCompoundBody", &pytysoc::PyCompound::addCompoundBody ) \
        .def_property( "pos", &pytysoc::PyCompound::pos, &pytysoc::PyCompound::setPosition ) \
        .def_property( "rot", &pytysoc::PyCompound::rot, &pytysoc::PyCompound::setRotationMat ) \
        .def_property( "euler", &pytysoc::PyCompound::euler, &pytysoc::PyCompound::setRotationEuler ) \
        .def_property( "quat", &pytysoc::PyCompound::quat, &pytysoc::PyCompound::setRotationQuaternion ) \
        .def( "getBodyByName", &pytysoc::PyCompound::getBodyByName, py::return_value_policy::reference ) \
        .def( "bodies", &pytysoc::PyCompound::bodies ) \
        .def_property_readonly( "name", &pytysoc::PyCompound::name ) \
        .def_property_readonly( "dyntype", &pytysoc::PyCompound::dyntype ) \
        .def( "root", &pytysoc::PyCompound::root, py::return_value_policy::reference );
