
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
            // bodies must have a one-to-one relation with joints
            _simBody->parentJointId = q;

            _simModel->bodies.push_back( _simBody );
        }

        auto _visualsJson   = _modelJson[JSON_KEY_VISUALS];
        for ( size_t q = 0; q < _visualsJson.size(); q++ )
        {
            auto _simVisual = new RlsimVisual();
            _simVisual->collectAttribs( _visualsJson[q] );

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
        // This tree representation has the joints as the nodes ...
        // and the visuals and bodies as children, so to initialize ...
        // this structure start by linking each body and visual to ...
        // the corresponding joint node (use its parentJointId).
        //
        // This tree kind of resembles the urdf format. Each link defines ...
        // the reference frame for each body, as bodies here represent a ...
        // combination of the "bodies-links" and "collisions".

        auto _joints    = modelDataPtr->joints;
        auto _bodies    = modelDataPtr->bodies;
        auto _visuals   = modelDataPtr->visuals;

        // Link visuals to their parent joints
        for ( size_t q = 0; q < _visuals.size(); q++ )
        {
            // validate that the parentJointId is in range
            if ( _visuals[q]->parentJointId < 0 ||
                 _visuals[q]->parentJointId >= _joints.size() )
            {
                std::cout << "ERROR> visual: " << _visuals[q]->name 
                          << " has an valid parentJointId: "
                          << _visuals[q]->parentJointId << std::endl;
                return false;
            }

            auto _parentJoint = _joints[ _visuals[q]->parentJointId ];
            _parentJoint->childVisuals.push_back( _visuals[q] );
        }

        // Link bodies to their parent joints
        for ( size_t q = 0; q < _bodies.size(); q++ )
        {
            // validate that the parentJointId is in range
            if ( _bodies[q]->parentJointId < 0 ||
                 _bodies[q]->parentJointId >= _joints.size() )
            {
                std::cout << "ERROR> body: " << _bodies[q]->name 
                          << " has an valid parentJointId: "
                          << _bodies[q]->parentJointId << std::endl;
                return false;
            }

            auto _parentJoint = _joints[ _bodies[q]->parentJointId ];
            _parentJoint->childBodies.push_back( _bodies[q] );
        }

        // Link joints between them
        for ( size_t q = 0; q < _joints.size(); q++ )
        {
            // Check if root
            if ( _joints[q]->parentJointId == -1 )
            {
                if ( modelDataPtr->rootJoint )
                {
                    std::cout << "ERROR> There seems to be more than one root joint" << std::endl;
                    std::cout << "ERROR> previous root has name: " << modelDataPtr->rootJoint->name << std::endl;
                    std::cout << "ERROR> extra root has name: " << _joints[q]->name << std::endl;
                    return false;
                }
                else
                {
                    modelDataPtr->rootJoint = _joints[q];
                }
            }
            else
            {
                // Validate joint connection
                // @WEIRD: didn't know that int vs uint could lead to ...
                // bad checks. parentjointid was -1 int, and joints.size() was uint
                if ( _joints[q]->parentJointId >= _joints.size() )
                {
                    std::cout << "ERROR> joint: " << _joints[q]->name
                              << " has an valid parentJointId: "
                              << _joints[q]->parentJointId << std::endl;
                    return false;
                }

                auto _parentJoint = _joints[ _joints[q]->parentJointId ];
                _parentJoint->childJoints.push_back( _joints[q] );
            }
        }

        return true;
    }

}}