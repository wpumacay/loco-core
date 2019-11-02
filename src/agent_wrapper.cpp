
#include <agent_wrapper.h>


namespace tysoc {

    TAgentWrapper::TAgentWrapper( TAgent* agentPtr,
                                  const std::string& workingDir )
    {
        // save a reference to the abstract kintree agent for later usage
        m_agentPtr = agentPtr;
        // and the path to the assets directory
        m_workingDir = workingDir;
        // and set to NULL (default) the parent simulation
        m_simulationPtr = NULL;

        if ( !agentPtr )
        {
            std::cout << "ERROR> passed null reference to kintreeagent" << std::endl;
            m_mjcfModelTemplatePtr = NULL;
            m_urdfModelTemplatePtr = NULL;
            m_rlsimModelTemplatePtr = NULL;
        }
        else
        {
            // and grab the model data for usage in specific backends (if needed)
            auto _modelTemplateType = agentPtr->format();
            if ( _modelTemplateType == eModelFormat::MJCF )
            {
                m_mjcfModelTemplatePtr = m_agentPtr->getModelDataMjcf();
                m_urdfModelTemplatePtr = NULL;
                m_rlsimModelTemplatePtr = NULL;
            }
            else if ( _modelTemplateType == eModelFormat::URDF )
            {
                m_urdfModelTemplatePtr = m_agentPtr->getModelDataUrdf();
                m_mjcfModelTemplatePtr = NULL;
                m_rlsimModelTemplatePtr = NULL;
            }
            else if ( _modelTemplateType == eModelFormat::RLSIM )
            {
                m_rlsimModelTemplatePtr = m_agentPtr->getModelDataRlsim();
                m_urdfModelTemplatePtr = NULL;
                m_mjcfModelTemplatePtr = NULL;
            }
        }
    }

    TAgentWrapper::~TAgentWrapper()
    {
        m_simulationPtr = NULL;
        m_agentPtr = NULL;
        m_mjcfModelTemplatePtr = NULL;
        m_urdfModelTemplatePtr = NULL;
        m_rlsimModelTemplatePtr = NULL;
    }

    void TAgentWrapper::setParentSimulation( TISimulation* simulationPtr )
    {
        m_simulationPtr = simulationPtr;
    }

    std::string TAgentWrapper::name()
    {
        if ( m_agentPtr )
            return m_agentPtr->name();

        return "undefined";
    }

    TAgent* TAgentWrapper::agent()
    {
        return m_agentPtr;
    }

    void TAgentWrapper::initialize()
    {
        _initializeInternal();
    }

    void TAgentWrapper::reset()
    {
        _resetInternal();
    }

    void TAgentWrapper::preStep()
    {
        _preStepInternal();
    }

    void TAgentWrapper::postStep()
    {
        _postStepInternal();
    }

}