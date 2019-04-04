
#pragma once

#include <sandbox/sandbox.h>

#include <simulation_base.h>

namespace tysoc {

    class TISimulation;

    class TBodyWrapper
    {

        protected :

        // Wrapped body
        sandbox::TBody* m_bodyPtr;

        // Directory where to look for resources (meshfiles, heightmaps)
        std::string m_workingDir;

        // Starting position and rotation, in case reset is needed
        TVec3 m_posStart;
        TMat3 m_rotStart;

        // Parent simulation
        TISimulation* m_simulationPtr;

        // Methods to override by backend-specific implementations
        virtual void _initializeInternal() = 0;
        virtual void _resetInternal() = 0;
        virtual void _preStepInternal() = 0;
        virtual void _postStepInternal() = 0;
        virtual void _changePositionInternal() = 0;
        virtual void _changeRotationInternal() = 0;
        virtual void _changeSizeInternal() = 0;

        void _initializeWorldTransforms();
        void _initializeBody( sandbox::TBody* bodyPtr );
        void _initializeJoint( sandbox::TJoint* jointPtr, sandbox::TBody* parentBodyPtr );

        public :

        TBodyWrapper( sandbox::TBody* bodyPtr,
                      const std::string& workingDir );

        virtual ~TBodyWrapper();

        void initialize();
        void reset();
        void preStep();
        void postStep();

        void setParentSimulation( TISimulation* simulationPtr );

        void setPosition( const TVec3& position );
        void setRotMat( const TMat3& rotmat );
        void setRotEuler( const TVec3& euler );
        void setRotQuat( const TVec4& quat );
        void setSize( const TVec3& size );

        std::string name();
        sandbox::TBody* body();
    };

}