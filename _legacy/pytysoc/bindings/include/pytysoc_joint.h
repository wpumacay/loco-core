#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <pytysoc_compound_body.h>

#include <components/joint.h>

namespace py = pybind11;

namespace pytysoc
{

    class PyBody;
    class PyCompoundBody;

    class PyJoint
    {

    public :

        /* @exposed */
        PyJoint( const std::string& name,
                 const PyJointData& jointData );

        /* @notexposed */
        PyJoint( tysoc::TJoint* jointRef );

        /* @notexposed */
        ~PyJoint();

        /* @exposed */
        void setOwner( PyBody* ownerRef );

        /* @exposed */
        void setOwnerParent( PyBody* ownerParentRef );

        /* @exposed */
        void setLocalTransform( py::array_t<TScalar>& localTransform );

        /* @exposed */
        void setQpos( py::array_t<TScalar>& qpos );

        /* @exposed */
        void setQvel( py::array_t<TScalar>& qvel );

        /* @exposed */
        PyJointData data() const;

    protected :

        tysoc::TJoint* m_joint;

        PyBody* m_ownerRef;
        PyBody* m_ownerParentRef;

    };

}


#define PYTYSOC_JOINT_BINDINGS(m) \
    py::class_<pytysoc::PyJoint>(m, "PyJoint") \
        .def( py::init<const std::string&, const pytysoc::PyJointData&>() ) \
        .def( "setOwner", &pytysoc::PyJoint::setOwner ) \
        .def( "setOwnerParent", &pytysoc::PyJoint::setOwnerParent ) \
        .def_property_readonly( "data", &pytysoc::PyJoint::data );
