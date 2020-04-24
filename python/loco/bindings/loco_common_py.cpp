
#include <loco_common_py.h>

namespace py = pybind11;

namespace loco
{
    py::array_t<TScalar> sizef_to_nparray( const TSizef& size )
    {
        auto nparray = py::array_t<TScalar>( size.ndim );
        auto bufferInfo = nparray.request();
        auto bufferData = bufferInfo.ptr;
        memcpy( bufferData, size.data(), sizeof( TScalar ) * size.ndim );
        return nparray;
    }

    TSizef nparray_to_sizef( const py::array_t<TScalar>& arr_size )
    {
        auto bufferInfo = arr_size.request();
        if ( bufferInfo.size > 10 )
            throw std::runtime_error( "nparray_to_sizef >>> given nparray exceeds max. number of elements in TSizef (10)" );

        auto bufferData = bufferInfo.ptr;
        auto size = TSizef();
        size.ndim = bufferInfo.size;
        memcpy( size.data(), bufferData, sizeof( TScalar ) * size.ndim );
        return size;
    }

    std::string ToString( const std::vector<TScalar>& stdvec )
    {
        std::string _strrep = "( ";
        for ( size_t i = 0; i < stdvec.size(); i++ )
            _strrep += std::to_string( stdvec[i] ) + ( i < ( stdvec.size() - 1 ) ? ", " : " )" );
        return _strrep;
    }

    void bindings_common( py::module& m )
    {
        m.attr( "MAX_NUM_QPOS" ) = loco::MAX_NUM_QPOS;
        m.attr( "MAX_NUM_QVEL" ) = loco::MAX_NUM_QVEL;
        m.attr( "NUM_QPOS_JOINT_PRISMATIC" ) = loco::NUM_QPOS_JOINT_PRISMATIC;
        m.attr( "NUM_QPOS_JOINT_REVOLUTE" ) = loco::NUM_QPOS_JOINT_REVOLUTE;
        m.attr( "NUM_QPOS_JOINT_SPHERICAL" ) = loco::NUM_QPOS_JOINT_SPHERICAL;
        m.attr( "NUM_QPOS_JOINT_FREE" ) = loco::NUM_QPOS_JOINT_FREE;
        m.attr( "DEFAULT_DENSITY" ) = loco::DEFAULT_DENSITY;
        m.attr( "PATH_RESOURCES" ) = loco::PATH_RESOURCES;
    }

    void bindings_utils( py::module& m )
    {
        py::class_< TNoiseGenerator >( m, "NoiseGenerator" )
            .def_static( "Init", &TNoiseGenerator::Init )
            .def_static( "Release", &TNoiseGenerator::Release )
            .def_static( "Config", &TNoiseGenerator::Config )
            .def_static( "GetNoise1d", &TNoiseGenerator::GetNoise1d )
            .def_static( "GetNoise2d", &TNoiseGenerator::GetNoise2d );
    }
}