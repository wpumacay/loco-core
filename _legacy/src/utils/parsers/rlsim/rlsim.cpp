
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

        // Grab the up direction (an extension to normalize into our format where z is Up)
        auto _worldUp = tysoc::json::safeParseString( _modelJson, "WorldUp", "y" );
        auto _worldScale = tysoc::json::safeParseFloat( _modelJson, "WorldScale", 1.0 );

        RlsimModel* _simModel = new RlsimModel();
        _simModel->worldUp = _worldUp;
        _simModel->worldScale = _worldScale;

        auto _jointsJson    = _modelJson[JSON_KEY_SKELETON][JSON_KEY_JOINTS];
        for ( size_t q = 0; q < _jointsJson.size(); q++ )
        {
            auto _simJoint = new RlsimJoint();
            _simJoint->collectAttribs( _jointsJson[q], _worldUp );

            _simModel->joints.push_back( _simJoint );
        }

        auto _bodiesJson    = _modelJson[JSON_KEY_BODIES];
        for ( size_t q = 0; q < _bodiesJson.size(); q++ )
        {
            auto _simBody = new RlsimBody();
            _simBody->collectAttribs( _bodiesJson[q], _worldUp );
            // bodies must have a one-to-one relation with joints
            _simBody->parentJointId = q;

            _simModel->bodies.push_back( _simBody );
        }

        auto _visualsJson   = _modelJson[JSON_KEY_VISUALS];
        auto _size = _visualsJson.size();
        for ( size_t q = 0; q < _visualsJson.size(); q++ )
        {
            RlsimVisual _simVisual;
            _simVisual.collectAttribs( _visualsJson[q], _worldUp );

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
        for ( auto _visual : _visuals )
        {
            // validate that the parentJointId is in range
            if ( _visual.parentJointId < 0 || _visual.parentJointId >= _joints.size() )
            {
                std::cout << "ERROR> visual: " << _visual.name 
                          << " has an invalid parentJointId: "
                          << _visual.parentJointId << std::endl;
                return false;
            }

            auto _parentJointId = _visual.parentJointId;
            auto _parentJoint = _joints[_parentJointId];
            _parentJoint->childVisuals.push_back( _visual );
        }

        // Link bodies to their parent joints
        for ( auto _body : _bodies )
        {
            // validate that the parentJointId is in range
            if ( _body->parentJointId < 0 || _body->parentJointId >= _joints.size() )
            {
                std::cout << "ERROR> body: " << _body->name 
                          << " has an invalid parentJointId: "
                          << _body->parentJointId << std::endl;
                return false;
            }

            auto _parentJoint = _joints[ _body->parentJointId ];
            _parentJoint->childBodies.push_back( _body );
        }

        // Link joints between them
        for ( auto _joint : _joints )
        {
            // Check if root
            if ( _joint->parentJointId == -1 )
            {
                if ( modelDataPtr->rootJoint )
                {
                    std::cout << "ERROR> There seems to be more than one root joint" << std::endl;
                    std::cout << "ERROR> previous root has name: " << modelDataPtr->rootJoint->name << std::endl;
                    std::cout << "ERROR> extra root has name: " << _joint->name << std::endl;
                    return false;
                }
                else
                {
                    modelDataPtr->rootJoint = _joint;
                }
            }
            else
            {
                // Validate joint connection
                // @WEIRD: didn't know that int vs uint could lead to ...
                // bad checks. parentjointid was -1 int, and joints.size() was uint
                if ( _joint->parentJointId >= _joints.size() )
                {
                    std::cout << "ERROR> joint: " << _joint->name
                              << " has an invalid parentJointId: "
                              << _joint->parentJointId << std::endl;
                    return false;
                }

                auto _parentJoint = _joints[ _joint->parentJointId ];
                _parentJoint->childJoints.push_back( _joint );
            }
        }

        return true;
    }

    void deepCopy( RlsimModel* target, 
                   RlsimModel* source,
                   const std::string& agentName )
    {
        if ( agentName != "" )
            target->name = agentName;

        for ( auto _sourceBody : source->bodies )
        {
            auto _targetBody = new RlsimBody();
            // Just copy everything directly (no pointer references here)
            *( _targetBody ) = *( _sourceBody );
            // and modify the name accordingly
            _targetBody->name = computeName( "body", _sourceBody->name, agentName );

            target->bodies.push_back( _targetBody );
        }

        for ( auto _sourceVisual : source->visuals )
        {
            // Just copy everything directly (no pointer references here)
            RlsimVisual _targetVisual = _sourceVisual;
            // and modify the name accordingly
            _targetVisual.name = computeName( "visual", _sourceVisual.name, agentName );

            target->visuals.push_back( _targetVisual );
        }

        for ( auto _sourceJoint : source->joints )
        {
            auto _targetJoint = new RlsimJoint();
            // First copy all contents from source to target
            *( _targetJoint ) = *( _sourceJoint );
            // and then clear the child containers
            _targetJoint->childVisuals.clear();
            _targetJoint->childJoints.clear();
            _targetJoint->childBodies.clear();
            // and modify the name accordingly
            _targetJoint->name = computeName( "joint", _sourceJoint->name, agentName );

            target->joints.push_back( _targetJoint );
        }

        if ( !_initTreeAndRoot( target ) )
        {
            std::cout << "WARNING> There was an issue copying the model for "
                      << "the agent with name: " << agentName << std::endl;
        }
    }

    std::string computeName( const std::string& type, 
                             const std::string& elementName, 
                             const std::string& agentName )
    {
        // In case no agentname is given (no replacement needed) then just skip
        if ( agentName == "" )
            return elementName;

        std::string _res;

        if ( type == "body" )
        {
            _res += PREFIX_BODY + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "collision" )
        {
            _res += PREFIX_GEOM + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "joint" )
        {
            _res += PREFIX_JOINT + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "actuator" )
        {
            _res += PREFIX_ACTUATOR + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "visual" )
        {
            _res += PREFIX_GEOM + agentName + std::string( "_" ) + elementName;
        }
        else
        {
            std::cout << "WARNING> not supported rlsim type: " 
                      << type << " for name generation" << std::endl;

            _res += elementName;
        }

        return _res;
    }

}}