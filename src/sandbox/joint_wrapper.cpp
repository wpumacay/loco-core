
#include <sandbox/joint_wrapper.h>

namespace tysoc {

    TJointWrapper::TJointWrapper( sandbox::TJoint* jointPtr,
                                  const std::string& workingDir )
    {
        m_jointPtr = jointPtr;
        m_workingDir = workingDir;

        if ( !m_jointPtr )
            std::cout << "ERROR> trying to wrapp NULL joint ptr" << std::endl;
    }

    TJointWrapper::~TJointWrapper()
    {
        // Just release the handle to the joint resource. 
        // The scenario is in charge of resource deletions
        m_jointPtr = NULL;
    }

    void TJointWrapper::initialize()
    {
        _initializeInternal();
    }

    void TJointWrapper::reset()
    {
        _resetInternal();
    }

    void TJointWrapper::preStep()
    {
        _preStepInternal();
    }

    void TJointWrapper::postStep()
    {
        _postStepInternal();
    }

    std::string TJointWrapper::name()
    {
        if ( !m_jointPtr )
            return "undefined";

        return m_jointPtr->name;
    }

    sandbox::TJoint* TJointWrapper::joint()
    {
        return m_jointPtr;
    }

}