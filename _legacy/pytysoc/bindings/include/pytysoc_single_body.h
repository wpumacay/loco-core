
#pragma once

#include <pytysoc_body.h>

#include <primitives/single_body.h>

namespace py = pybind11;

namespace pytysoc
{

    // @todo: change usage of raw-pointers to unique_ptrs or shared_ptrs (check pybind constraints)

    class PyCollision;
    class PyVisual;

    class PySingleBody : public PyBody
    {

    public :

        /* @exposed */
        PySingleBody( const std::string& name,
                      const PyBodyData& data,
                      py::array_t<TScalar>& xyz,
                      py::array_t<TScalar>& rpy );

        /* @notexposed */
        ~PySingleBody();
    };

}

#define PYTYSOC_SINGLE_BODY_BINDINGS(m) \
    py::class_<pytysoc::PySingleBody, pytysoc::PyBody>(m, "PySingleBody") \
        .def( py::init<const std::string&, const pytysoc::PyBodyData&, py::array_t<TScalar>&, py::array_t<TScalar>&>() );
