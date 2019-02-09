
#include <utils/parsers/rlsim/rlsim.h>


namespace tysoc {
namespace rlsim {

    RlsimModel* loadGenericModel( const std::string& modelfile )
    {
        RlsimModel* _model = NULL;

        std::ifstream _modelFileHandle;
        _modelFileHandle.open( modelfile.c_str(), std::ifstream::in );

        nlohmann::json _modelJson;
        try 
        {
            _modelFileHandle >> _modelJson;
        }
        catch ( nlohmann::json::parse_error& e )
        {
            std::cout << "ERROR> Failed to parse json file: " << modelfile << std::endl;
            std::cout << "ERROR> error message: " << e.what() << std::endl;
            _modelFileHandle.close();

            return NULL;
        }
        _modelFileHandle.close();

        // Some sanity checks
        if ( _modelJson.find( JSON_KEY_SKELETON ) == _modelJson.end() )
        {
            std::cout << "ERROR> there is no \"Skeleton\" information in model: " << modelfile << std::endl;
            return NULL;
        }
        if ( _modelJson[JSON_KEY_SKELETON].find( JSON_KEY_JOINTS ) ==
             _modelJson[JSON_KEY_SKELETON].end() )
        {
            std::cout << "ERROR> there is no \"Joints\" information in model: " << modelfile << std::endl;
            return NULL;
        }
        if ( _modelJson.find( JSON_KEY_BODIES ) == _modelJson.end() )
        {
            std::cout << "ERROR> there is no \"BodyDefs\" information in model: " << modelfile << std::endl;
            return NULL;
        }
        if ( _modelJson.find( JSON_KEY_VISUALS ) == _modelJson.end() )
        {
            std::cout << "ERROR> there is no \"DrawShapeDefs\" information in model: " << modelfile << std::endl;
            return NULL;
        }

        RlsimModel* _simModel = new RlsimModel();

        auto _jointsJson    = _modelJson[JSON_KEY_SKELETON][JSON_KEY_JOINTS];
        for ( size_t q = 0; q < _jointsJson.size(); q++ )
        {
            auto _simJoint = new RlsimJoint();
            _simJoint->collectAttribs( _jointsJson[q] );

            _simModel->joints.push_back( _simJoint );
        }

        auto _bodiesJson    = _modelJson[JSON_KEY_BODIES];
        for ( size_t q = 0; q < _bodiesJson.size(); q++ )
        {
            auto _simBody = new RlsimBody();
            _simBody->collectAttribs( _bodiesJson[q] );

            _simModel->bodies.push_back( _simBody );
        }

        auto _visualsJson   = _modelJson[JSON_KEY_VISUALS];
        for ( size_t q = 0; q < _visualsJson.size(); q++ )
        {
            auto _simVisual = new RlsimVisual();
            _simVisual->collectAttribs( _visualsJson );

            _simModel->visuals.push_back( _simVisual );
        }

        if ( !_initTreeAndRoot( _simModel ) )
        {
            std::cout << "ERROR> there was an error initializing the tree structure of model: " << modelfile << std::endl;
            delete _simModel;

            return NULL;
        }

        return _simModel;
    }

    bool _initTreeAndRoot( RlsimModel* modelDataPtr )
    {




        return true;
    }

}}