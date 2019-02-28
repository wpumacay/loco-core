
#include <terrain_wrapper.h>


namespace tysoc {


    TTerrainGenWrapper::TTerrainGenWrapper( terrain::TITerrainGenerator* terrainGenPtr,
                                            const std::string& workingDir )
    {
        m_terrainGenPtr = terrainGenPtr;
        m_workingDir    = workingDir;
    }
        
    TTerrainGenWrapper::~TTerrainGenWrapper()
    {
        m_terrainGenPtr = NULL;
    }

    void TTerrainGenWrapper::initialize()
    {
        _initializeInternal();
    }

    void TTerrainGenWrapper::reset()
    {
        _resetInternal();
    }

    void TTerrainGenWrapper::preStep()
    {
        _preStepInternal();
    }

    void TTerrainGenWrapper::postStep()
    {
        _postStepInternal();
    }

    std::string TTerrainGenWrapper::name()
    {
        if ( m_terrainGenPtr )
            return m_terrainGenPtr->name();

        return "undefined";
    }

    terrain::TITerrainGenerator* TTerrainGenWrapper::terrainGenerator()
    {
        return m_terrainGenPtr;
    }

}