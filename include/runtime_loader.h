
#pragma once

#include <runtime_base.h>


namespace tysoc {


    class TRuntime
    {

        protected :

        // Fcn pointers to the library functionality
        void _loadLibraryFcns();

        FcnCreateSim* m_fcnCreateSim;

        FcnCreateAgentFromAbstract* m_fcnCreateAgentFromAbstract;
        FcnCreateAgentFromFile*     m_fcnCreateAgentFromFile;
        FcnCreateAgentFromId*       m_fcnCreateAgentFromId;

        FcnCreateTerrainGenFromAbstract*    m_fcnCreateTerrainGenFromAbstract;
        FcnCreateTerrainGenFromParams*      m_fcnCreateTerrainGenFromParams;

        public :

        /**
        *   Creates a runtime with a specific type. It loads the library .so ...
        *   (or .dll) accordingly
        */
        TRuntime( const std::string& type,
                  const std::string& dlpath );
        ~TRuntime();

        TISimulation* createSimulation();

        TKinTreeAgentWrapper* createAgent( const std::string& name,
                                           agent::TAgentKinTree* kinTreeAgentPtr );
        TKinTreeAgentWrapper* createAgent( const std::string& name,
                                           const std::string& filename );
        TKinTreeAgentWrapper* createAgent( const std::string& name,
                                           const std::string& format,
                                           const std::string& id );
        
        TTerrainGenWrapper* createTerrainGen( const std::string& name,
                                              terrain::TITerrainGenerator* terrainGenPtr );
        TTerrainGenWrapper* createTerrainGen( const std::string& name,
                                              const TGenericParams& params );
        
    };


}