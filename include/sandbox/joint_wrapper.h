
#pragma once

#include <sandbox/sandbox.h>

namespace tysoc {

    class TJointWrapper
    {

        protected :

        // Wrapped joint
        sandbox::TJoint* m_jointPtr;

        // Directory where to look for resources (meshfiles, heightmaps)
        std::string m_workingDir;

        // Methods to override by backend-specific implementations
        virtual void _initializeInternal() = 0;
        virtual void _resetInternal() = 0;
        virtual void _preStepInternal() = 0;
        virtual void _postStepInternal() = 0;

        public :

        TJointWrapper( sandbox::TJoint* jointPtr,
                       const std::string& workingDir );
        
        virtual ~TJointWrapper();

        void initialize();
        void reset();
        void preStep();
        void postStep();

        std::string name();
        sandbox::TJoint* joint();

    };


}