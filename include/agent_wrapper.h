
#pragma once

#include <agent/types/agent_kintree.h>
#include <agent/types/agent_kintree_mjcf.h>
#include <agent/types/agent_kintree_urdf.h>
#include <agent/types/agent_kintree_rlsim.h>

namespace tysoc { 

    /**
    *   Agent Wrapper Interface for the kintree agents. The objects ...
    *   from this class are in charge of wrapping the core agent data  ...
    *   structures and instantiate the required data structures in the ...
    *   specific backend.
    *
    *   Backend specific agents should extend this class, and implement the ...
    *   necessary code in the following virtual methods:
    *
    *   > _initializeInternal: Here we can add the initialization code to ...
    *                          instantiate the wrapped kintree into the ...
    *                          specific backend, e.g. creating the mjcf ...
    *                          resources for the mujoco agents, and injecting ...
    *                          into a given xml.
    *
    *   > _resetInternal: Here we should place code that reset the wrapper and ...
    *                     the wrapped kintree as well to a desired state given ...
    *                     by the user, e.g. placing the kintree agent into a ...
    *                     starting pose for a new episode.
    *
    *   > _preStep: Here we should place the code that is called previous to ...
    *               a simulation step executed by the specific backend. Here ...
    *               can extract info from the user (stored in parts of these ...
    *               exposed adapters) and load it to the simulator for a sim step.
    *   
    *   > _postStep: Here we should place code to grab information back from ...
    *                the simulator in order to update the core kintree. Recall ...
    *                that the core kintree is a common point of information for ...
    *                all components of the framework.
    *   
    */
    class TKinTreeAgentWrapper
    {

        protected :

        // mjcf resource: a copy of the mjcf model passed for construction
        mjcf::GenericElement* m_mjcfModelTemplatePtr;
        // urdf resource: a reference to the cached urdf model passed for construction
        urdf::UrdfModel* m_urdfModelTemplatePtr;
        // rlsim resource: a copy of the rlsim model passed for construction
        rlsim::RlsimModel* m_rlsimModelTemplatePtr;

        // underlying kinematic tree
        agent::TAgentKinTree*   m_kinTreeAgentPtr;

        // directory where the assets are
        std::string m_workingDir;

        virtual void _initializeInternal() = 0;
        virtual void _resetInternal() = 0;

        virtual void _preStepInternal() = 0;
        virtual void _postStepInternal() = 0;

        public :

        /**
        *   Creates a kintree wrapper for a given kintree agent
        *
        *   @param kinTreeAgentPtr  kintree agent to wrap
        *   @param workingDir       directory where the assets(meshes,etc) are located
        */
        TKinTreeAgentWrapper( agent::TAgentKinTree* kinTreeAgentPtr,
                              const std::string& workingDir );

        /**
        *   Destroy wrapping functionality by removing all wrapping data, ...
        *   except the core object (just releases the reference and leave ...
        *   the core structures to be reused by the user or other components).
        */
        virtual ~TKinTreeAgentWrapper();

        std::string name();
        agent::TAgentKinTree* agent();

        void initialize();
        void reset();

        void preStep();
        void postStep();

    };



}