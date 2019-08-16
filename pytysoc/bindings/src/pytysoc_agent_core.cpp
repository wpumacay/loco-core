
#include <pytysoc_agent_core.h>

namespace py = pybind11;

namespace pytysoc
{

    /* @TODO|@CHECK: check if can solve potential leaks using std::shared_ptr.
    *                Ownership of the created kintree should belong to the scenario ...
    *                but still one could have situations where the object is not added ...
    *                to a scenario at all (which is weird, as the scenario is used in ...
    *                order to simulate objects).
    */
    PyCoreAgent::PyCoreAgent( const std::string& name,
                              py::array_t<TScalar>& position,
                              const std::string& modelFormat,
                              const std::string& modelName )
    {
        auto _positionVec3 = numpyToVec3( position );
        // Check if the model is cached
        auto _modelLoader = tysoc::TModelLoader::GetInstance();

        if ( modelFormat == "mjcf" )
        {
            tysoc::mjcf::GenericElement* _modelTemplatePtr = NULL;
            if ( _modelLoader->hasMjcfModel( modelName ) )
                _modelTemplatePtr = _modelLoader->getMjcfModel( modelName );
            else
                _modelTemplatePtr = tysoc::mjcf::loadGenericModel( modelName );
            
            // create kintree agent from mjcf data ptr //@TODO: Perhaps use different names and no overloading
            m_agentPtr = new tysoc::agent::TAgent( _modelTemplatePtr, name, _positionVec3, { 0., 0., 0. } );
        }
        else if ( modelFormat == "urdf" )
        {
            tysoc::urdf::UrdfModel* _modelTemplatePtr = NULL;
            if ( _modelLoader->hasUrdfModel( modelName ) )
                _modelTemplatePtr = _modelLoader->getUrdfModel( modelName );
            else
                _modelTemplatePtr = tysoc::urdf::loadGenericModel( modelName );
            
            // create kintree agent from urdf data ptr
            m_agentPtr = new tysoc::agent::TAgent( _modelTemplatePtr, name, _positionVec3, { 0., 0., 0. } );
        }
        else if ( modelFormat == "rlsim" )
        {
            tysoc::rlsim::RlsimModel* _modelTemplatePtr = NULL;
            if ( _modelLoader->hasRlsimModel( modelName ) )
                _modelTemplatePtr = _modelLoader->getRlsimModel( modelName );
            else
                _modelTemplatePtr = tysoc::rlsim::loadGenericModel( modelName );

            // create kintree agent from rlsim data ptr
            m_agentPtr = new tysoc::agent::TAgent( _modelTemplatePtr, name, _positionVec3, { 0., 0., 0. } );
        }
        else
        {
            std::cout << "ERROR> tried to create agent: '"
                      << modelName <<"' with not supported format: " 
                      << modelFormat << std::endl;

            m_agentPtr = NULL;
        }
    }

    PyCoreAgent::PyCoreAgent( tysoc::agent::TAgent* agentPtr )
    {
        m_agentPtr = agentPtr;
    }

    PyCoreAgent::~PyCoreAgent()
    {
        m_agentPtr = NULL;
    }

    void PyCoreAgent::setActions( py::array_t<TScalar>& actions )
    {
        auto _vecArrayActions = numpyToVecArray( actions );
        if ( m_agentPtr )
            m_agentPtr->setActions( _vecArrayActions );
    }

    int PyCoreAgent::getActionDim()
    {
        if ( m_agentPtr )
            return m_agentPtr->getActionDim();

        return -1;
    }

    py::array_t<TScalar> PyCoreAgent::getPosition()
    {
        if ( m_agentPtr )
            return vec3ToNumpy( m_agentPtr->getPosition() );

        return vec3ToNumpy( tysoc::TVec3( 0.0f, 0.0f, 0.0f ) );
    }

    std::string PyCoreAgent::name()
    {
        if ( m_agentPtr )
            return m_agentPtr->name();

        return "undefined";
    }

    tysoc::agent::TAgent* PyCoreAgent::ptr()
    {
        return m_agentPtr;
    }

}