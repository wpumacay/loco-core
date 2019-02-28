
#include <agent_wrapper.h>


namespace tysoc {


    TKinTreeAgentWrapper::TKinTreeAgentWrapper( agent::TAgentKinTree* kinTreeAgentPtr,
                                                const std::string& workingDir )
    {
        // save a reference to the abstract kintree agent for later usage
        m_kinTreeAgentPtr = kinTreeAgentPtr;
        // and the path to the assets directory
        m_workingDir = workingDir;

        if ( !kinTreeAgentPtr )
        {
            std::cout << "ERROR> passed null reference to kintreeagent" << std::endl;
            m_mjcfModelTemplatePtr = NULL;
            m_urdfModelTemplatePtr = NULL;
            m_rlsimModelTemplatePtr = NULL;
        }
        else
        {
            // and grab the model data for usage in specific backends (if needed)
            auto _modelTemplateType = kinTreeAgentPtr->getModelTemplateType();
            if ( _modelTemplateType == agent::MODEL_TEMPLATE_TYPE_MJCF )
            {
                m_mjcfModelTemplatePtr = reinterpret_cast< agent::TAgentKinTreeMjcf* >
                                            ( m_kinTreeAgentPtr )->getMjcfModelDataPtr();
                m_urdfModelTemplatePtr = NULL;
                m_rlsimModelTemplatePtr = NULL;
            }
            else if ( _modelTemplateType == agent::MODEL_TEMPLATE_TYPE_URDF )
            {
                m_urdfModelTemplatePtr = reinterpret_cast< agent::TAgentKinTreeUrdf* >
                                            ( m_kinTreeAgentPtr )->getUrdfModelDataPtr();
                m_mjcfModelTemplatePtr = NULL;
                m_rlsimModelTemplatePtr = NULL;
            }
            else if ( _modelTemplateType == agent::MODEL_TEMPLATE_TYPE_RLSIM )
            {
                m_rlsimModelTemplatePtr = reinterpret_cast< agent::TAgentKinTreeRlsim* >
                                            ( m_kinTreeAgentPtr )->getRlsimModelDataPtr();
                m_urdfModelTemplatePtr = NULL;
                m_mjcfModelTemplatePtr = NULL;
            }
        }
    }

    TKinTreeAgentWrapper::~TKinTreeAgentWrapper()
    {
        m_kinTreeAgentPtr = NULL;
        m_mjcfModelTemplatePtr = NULL;
        m_urdfModelTemplatePtr = NULL;
        m_rlsimModelTemplatePtr = NULL;
    }

    std::string TKinTreeAgentWrapper::name()
    {
        if ( m_kinTreeAgentPtr )
            return m_kinTreeAgentPtr->name();

        return "undefined";
    }

    agent::TAgentKinTree* TKinTreeAgentWrapper::agent()
    {
        return m_kinTreeAgentPtr;
    }

    void TKinTreeAgentWrapper::initialize()
    {
        _initializeInternal();
    }

    void TKinTreeAgentWrapper::reset()
    {
        _resetInternal();
    }

    void TKinTreeAgentWrapper::preStep()
    {
        _preStepInternal();
    }

    void TKinTreeAgentWrapper::postStep()
    {
        _postStepInternal();
    }

}