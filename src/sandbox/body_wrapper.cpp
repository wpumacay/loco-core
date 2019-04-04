
#include <sandbox/body_wrapper.h>

namespace tysoc {

    TBodyWrapper::TBodyWrapper( sandbox::TBody* bodyPtr,
                                const std::string& workingDir )
    {
        m_bodyPtr = bodyPtr;
        m_workingDir = workingDir;

        if ( !m_bodyPtr )
        {
            std::cout << "ERROR> trying to wrapp NULL body ptr" << std::endl;
        }
        else
        {
            m_posStart = m_bodyPtr->worldTransform.getPosition();
            m_rotStart = m_bodyPtr->worldTransform.getRotation();
        }

        m_simulationPtr = NULL;
    }

    TBodyWrapper::~TBodyWrapper()
    {
        // Just release the handle to the body resource. 
        // The scenario is in charge of resource deletions
        m_bodyPtr = NULL;
        m_simulationPtr = NULL;
    }

    void TBodyWrapper::initialize()
    {
        _initializeWorldTransforms();
        _initializeInternal();
    }

    void TBodyWrapper::reset()
    {
        _resetInternal();
    }

    void TBodyWrapper::preStep()
    {
        _preStepInternal();
    }

    void TBodyWrapper::postStep()
    {
        _postStepInternal();
    }

    void TBodyWrapper::setParentSimulation( TISimulation* simulationPtr )
    {
        m_simulationPtr = simulationPtr;
    }

    void TBodyWrapper::setPosition( const TVec3& position )
    {
        if ( !m_bodyPtr )
            return;

        m_bodyPtr->worldTransform.setPosition( position );
        _changePositionInternal();
    }

    void TBodyWrapper::setRotMat( const TMat3& rotmat )
    {
        if ( !m_bodyPtr )
            return;

        m_bodyPtr->worldTransform.setRotation( rotmat );
        _changeRotationInternal();
    }

    void TBodyWrapper::setRotEuler( const TVec3& euler )
    {
        if ( !m_bodyPtr )
            return;

        m_bodyPtr->worldTransform.setRotation( TMat3::fromEuler( euler ) );
        _changeRotationInternal();
    }

    void TBodyWrapper::setRotQuat( const TVec4& quat )
    {
        if ( !m_bodyPtr )
            return;

        m_bodyPtr->worldTransform.setRotation( TMat3::fromQuaternion( quat ) );
        _changeRotationInternal();
    }

    void TBodyWrapper::setSize( const TVec3& size )
    {
        if ( !m_bodyPtr )
            return;

        m_bodyPtr->size = size;
        _changeSizeInternal();
    }

    std::string TBodyWrapper::name()
    {
        if ( !m_bodyPtr )
            return "undefined";

        return m_bodyPtr->name;
    }

    sandbox::TBody* TBodyWrapper::body()
    {
        return m_bodyPtr;
    }

    void TBodyWrapper::_initializeWorldTransforms()
    {
        if ( !m_bodyPtr )
            return;

        _initializeBody( m_bodyPtr );
    }

    void TBodyWrapper::_initializeBody( sandbox::TBody* bodyPtr )
    {
        if ( !bodyPtr )
            return;

        if ( bodyPtr->parentBodyPtr )
        {
            bodyPtr->worldTransform = bodyPtr->parentBodyPtr->worldTransform *
                                      bodyPtr->relTransform;
        }

        for ( size_t q = 0; q < bodyPtr->joints.size(); q++ )
            _initializeJoint( bodyPtr->joints[q], bodyPtr );

        for ( size_t q = 0; q < bodyPtr->bodies.size(); q++ )
            _initializeBody( bodyPtr->bodies[q] );
    }

    void TBodyWrapper::_initializeJoint( sandbox::TJoint* jointPtr, sandbox::TBody* parentBodyPtr )
    {
        if ( !jointPtr )
            return;

        jointPtr->worldTransform = parentBodyPtr->worldTransform * jointPtr->relTransform;
    }


}