#pragma once

// main functionality from pybind11
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
// common includes of loco-core
#include <loco_common.h>
// helpers from tinymath (nparray<->vector|matrix)
#include <bindings_vector_t.h>
#include <bindings_matrix_t.h>

namespace loco
{
    // Conversion from std-vector (representing a 2D grid of row-major data) to numpy array
    template< typename Scalar_T >
    py::array_t<Scalar_T> vecgrid_to_nparray( const std::vector<Scalar_T>& vecGrid )
    {
        auto nparray = py::array_t<Scalar_T>( vecGrid.size() );
        auto bufferInfo = nparray.request();
        auto bufferData = bufferInfo.ptr;
        memcpy( bufferData, vecGrid.data(), sizeof( Scalar_T ) * vecGrid.size() );
        return nparray;
    }

    // Conversion from numpy array (representing a 2D grid of row-major data) to std-vector
    template< typename Scalar_T >
    std::vector<Scalar_T> nparray_to_vecgrid( const py::array_t<Scalar_T>& arr_vecGrid )
    {
        auto bufferInfo = arr_vecGrid.request();
        auto vecGrid = std::vector<Scalar_T>( bufferInfo.size, 0.0 );
        if ( bufferInfo.ndim > 1 && ( bufferInfo.strides[0] < bufferInfo.strides[1] ) )
            throw std::runtime_error( "loco::nparray_to_vecgrid >>> expected row-major np-array, but got column-major" );

        auto bufferData = bufferInfo.ptr;
        memcpy( vecGrid.data(), bufferData, sizeof( Scalar_T ) * vecGrid.size() );
        return vecGrid;
    }

    // Conversion from TSizef to numpy array
    py::array_t<TScalar> sizef_to_nparray( const TSizef& size );

    // Conversion from numpy array to TSizef
    TSizef nparray_to_sizef( const py::array_t<TScalar>& arr_size );

    // Conversion from std-vector to numpy array
    template< typename Scalar_T >
    py::array_t<Scalar_T> stdvec_to_nparray( const std::vector<Scalar_T>& stdvec )
    {
        auto nparray = py::array_t<Scalar_T>( stdvec.size() );
        auto bufferInfo = nparray.request();
        auto bufferData = bufferInfo.ptr;
        memcpy( bufferData, stdvec.data(), sizeof( Scalar_T ) * stdvec.size() );
        return nparray;
    }

    // Conversion from numpy array to std-vector
    template< typename Scalar_T >
    std::vector<Scalar_T> nparray_to_stdvec( const py::array_t<Scalar_T>& arr_stdvec )
    {
        auto bufferInfo = arr_stdvec.request();
        auto bufferData = bufferInfo.ptr;
        auto stdvec = std::vector<Scalar_T>( bufferInfo.size, 0.0 );
        memcpy( stdvec.data(), bufferData, sizeof( Scalar_T ) * bufferInfo.size );
        return stdvec;
    }

    // Returns string-representation of an std-vector
    std::string ToString( const std::vector<TScalar>& stdvec );
}