
#pragma once

#include <pytysoc_common.h>

#include <terrain/terrain.h>

namespace py = pybind11;

namespace pytysoc
{


    class PyTerrainGen
    {

        protected :

        tysoc::terrain::TITerrainGenerator* m_terrainGenPtr;


        public :

        /**
        *   Creates a python wrapper for an abstract terrain generator.
        *   The wrapped agent is created internally.
        *
        *   @param name         The name of the terrain generator
        *   @exposed            Exposed through Python API
        */
        PyTerrainGen( const std::string& name );

        /**
        *   Destructor for this wrapper. It only NULLs the references wrapped.
        *   The actual deletion of the wrapped resources should be made by ...
        *   the scenario, which has control over all kintrees, terraingens and sensors.
        */
        ~PyTerrainGen();

        /**
        *   Gets the name of the wrapped terrainGen
        *
        *   @exposed    Exposed through python API
        */
        std::string name();

        tysoc::terrain::TITerrainGenerator* ptr();
    };

    class PyStaticTerrainGen : public PyTerrainGen
    {

        public :

        /**
        *   Creates a python wrapper for an abstract staticTerrain 
        *   generator. The static terrain generator is created
        *   internally.
        *
        *   @param name         The name of the static terrain generator
        *   @exposed            Exposed through Python API
        */
        PyStaticTerrainGen( const std::string& name );

        /**
        *   Creates a terrain primitive for this terrain generator,
        *   
        *   @param type         Type of primitive (plane|box|cylinder|capsule|sphere|mesh)
        *   @param size         Dimensions of the primitive (depends of type)
        *   @param position     Position as 3-element array
        *   @param rotation     Rotation as 3-element euler angles
        *   @param rgb          Color as 3-element rgb
        *   @param filename     Filename for mesh types
        *   @exposed            Exposed through Python API
        */
        void createPrimitive( const std::string& type,
                              py::array_t<TScalar>& size,
                              py::array_t<TScalar>& position,
                              py::array_t<TScalar>& rotation,
                              py::array_t<TScalar>& rgb,
                              const std::string& texturename = "",
                              const std::string& filename = "" );


    };

}

#define PYTYSOC_CORE_TERRAINGEN_BINDINGS(m) \
    py::class_<pytysoc::PyTerrainGen>( m, "PyTerrainGen" ) \
        .def( py::init<const std::string&>() ); \
    py::class_<pytysoc::PyStaticTerrainGen, pytysoc::PyTerrainGen>( m, "PyStaticTerrainGen" ) \
        .def( py::init<const std::string&>() ) \
        .def( "createPrimitive", \
              &pytysoc::PyStaticTerrainGen::createPrimitive, \
              py::arg( "type" ), py::arg( "size" ), \
              py::arg( "position" ), py::arg( "rotation" ), \
              py::arg( "rgb" ), py::arg( "texturename" ) = "", py::arg( "filename" ) = "" );
