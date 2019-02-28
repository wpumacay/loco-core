
#pragma once

#include <terrain/terrain.h>

namespace tysoc {

    /**
    *   Terrain-generator Wrapper Interface for the terrain generators. The ...
    *   objects from this class are in charge of wrapping the core terrain ...
    *   data structures and instantiate the required data structures in the ...
    *   specific backend.
    *
    *   Backend specific terrain generators should extend this class, and ...
    *   implement the necessary code in the following virtual methods:
    *
    *   > _initializeInternal: Here we can add the initialization code to ...
    *                          instantiate the wrapped terraingen into the ...
    *                          specific backend, e.g. creating the mjcf ...
    *                          resources for the terrain inmujoco, and ...
    *                          injecting into a given xml.
    *
    *   > _resetInternal: Here we should place code that reset the wrapper and ...
    *                     the wrapped terraingen as well to a desired state given ...
    *                     by the user, e.g. placing all primitives from a ...
    *                     terraingen into the appropriate restart positions.
    *
    *   > _preStep: Here we should place the code that is called previous to ...
    *               a simulation step executed by the specific backend. Here ...
    *               can extract info from the user (stored in parts of these ...
    *               exposed adapters) and load it to the simulator for a sim step.
    *   
    *   > _postStep: Here we should place code to grab information back from ...
    *                the simulator in order to update the core terraingen. Recall ...
    *                that the core terraingen is a common point of information for ...
    *                all components of the framework.
    *   
    */
    class TTerrainGenWrapper
    {
        protected :

        // terrain generator to wrap
        terrain::TITerrainGenerator* m_terrainGenPtr;
        // directory where the assets are
        std::string m_workingDir;

        virtual void _initializeInternal() = 0;
        virtual void _resetInternal() = 0;

        virtual void _preStepInternal() = 0;
        virtual void _postStepInternal() = 0;

        public :

        /**
        *   Creates a terraingen wrapper for a given terrain generator
        *
        *   @param terrainGenPtr    terrain generator to wrap
        *   @param workingDir       directory where the assets(meshes,etc) are located
        */
        TTerrainGenWrapper( terrain::TITerrainGenerator* terrainGenPtr,
                            const std::string& workingDir );

        /**
        *   Destroy wrapping functionality by removing all wrapping data, ...
        *   except the core object (just releases the reference and leave ...
        *   the core structures to be reused by the user or other components).
        */
        virtual ~TTerrainGenWrapper();

        void initialize();
        void reset();

        void preStep();
        void postStep();

        std::string name();
        terrain::TITerrainGenerator* terrainGenerator();
    };

}