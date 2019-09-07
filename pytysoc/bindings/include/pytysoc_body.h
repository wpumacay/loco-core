
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_data.h>
#include <components/bodies.h>

namespace py = pybind11;

namespace pytysoc
{

    class PyBody
    {

    public :

        PyBody( const std::string& name,
                py::array_t<TScalar>& position,
                py::array_t<TScalar>& rotation );


    private :





    };


}