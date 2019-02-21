
#include <utils/parsers/urdf/urdf.h>


namespace tysoc {
namespace urdf {

    std::map< std::string, UrdfMaterial* > UrdfMaterial::MATERIALS;

    UrdfModel* loadGenericModel( const std::string& modelfile )
    {
        tinyxml2::XMLDocument _doc;
        _doc.LoadFile( modelfile.c_str() );

        if ( _doc.Error() )
        {
            parsing::logError( _doc.ErrorStr() );
            _doc.ClearError();

            return NULL;
        }

        auto _robotElement = _doc.FirstChildElement( "robot" );
        if ( !_robotElement )
        {
            parsing::logError( "expected a robot element" );
            return NULL;
        }

        auto _urdfModel = new UrdfModel();

        // Clear materials cache for new materials
        UrdfMaterial::MATERIALS.clear();

        // Grab the name
        _urdfModel->name = xml::safeParseString( _robotElement, "name" );

        for ( auto _linkElement = _robotElement->FirstChildElement( "link" ); 
                   _linkElement != NULL; 
                   _linkElement = _linkElement->NextSiblingElement( "link" ) )
        {
            // create a link
            auto _urdfLink = new UrdfLink();
            _urdfLink->collectAttribs( _linkElement );

            // cache this link for later initialization
            if ( _urdfModel->links.find( _urdfLink->name ) !=
                 _urdfModel->links.end() )
            {
                std::string _msg;
                _msg += "Link with name: ";
                _msg += _urdfLink->name + " ";
                _msg += "already exists!";

                parsing::logError( _msg );
            }
            else
            {
                _urdfModel->links[ _urdfLink->name ] = _urdfLink;
            }
        }

        // Get all Joint elements
        for ( auto _jointElement = _robotElement->FirstChildElement( "joint" ); 
                   _jointElement != NULL; 
                   _jointElement = _jointElement->NextSiblingElement( "joint" ) )
        {
            // create a joint
            auto _urdfJoint = new UrdfJoint();
            _urdfJoint->collectAttribs( _jointElement );

            // cache this joint for later initialization
            if ( _urdfModel->joints.find( _urdfJoint->name ) !=
                 _urdfModel->joints.end() )
            {
                std::string _msg;
                _msg += "Joint with name: ";
                _msg += _urdfJoint->name + " ";
                _msg += "already exists!";

                parsing::logError( _msg );
            }
            else
            {
                _urdfModel->joints[ _urdfJoint->name ] = _urdfJoint;
            }
        }

        if ( !_initTreeAndRoot( _urdfModel ) )
        {
            return NULL;
        }

        // Copy the materials cache into this model
        _urdfModel->materials = UrdfMaterial::MATERIALS;

        return _urdfModel;
    }

    bool _initTreeAndRoot( UrdfModel* urdfModelPtr )
    {
        // loop through all joints, for every link, assign children links and children joints
        for ( auto it = urdfModelPtr->joints.begin(); 
                   it != urdfModelPtr->joints.end(); 
                   it++ )
        {
            auto _jointPtr = it->second;

            if ( _jointPtr )
            {
                UrdfLink* _childLink = NULL;

                if ( urdfModelPtr->links.find( _jointPtr->childLinkName ) !=
                     urdfModelPtr->links.end() )
                {
                    _childLink = urdfModelPtr->links[ _jointPtr->childLinkName ];
                }
                else
                {
                    parsing::logError( std::string( "joint: " ) + 
                                       _jointPtr->name +
                                       std::string( " requested non-existent child link: " ) +
                                       _jointPtr->childLinkName );

                    continue;
                }

                // Check if using "world" extension to fix root body to the world
                if ( _jointPtr->type == "world" )
                {
                    _childLink->parentJoint = _jointPtr;
                    _childLink->parentLink  = NULL;
                    continue;
                }

                UrdfLink* _parentLink = NULL;
                
                if ( urdfModelPtr->links.find( _jointPtr->parentLinkName ) !=
                     urdfModelPtr->links.end() )
                {
                    _parentLink = urdfModelPtr->links[ _jointPtr->parentLinkName ];
                }
                else
                {
                    parsing::logError( std::string( "joint: " ) + 
                                       _jointPtr->name +
                                       std::string( " requested non-existent parent link: " ) +
                                       _jointPtr->parentLinkName );

                    continue;
                }

                _childLink->parentJoint = _jointPtr;
                _childLink->parentLink  = _parentLink;

                // Skip if trying to connect to the WORLD
                if ( _jointPtr->parentLinkName != "WORLD" )
                {
                    _parentLink->childJoints.push_back( _jointPtr );
                    _parentLink->childLinks.push_back( _childLink );
                }
            }
        }

        //search for children that have no parent, those are 'root'
        for ( auto it = urdfModelPtr->links.begin(); 
                   it != urdfModelPtr->links.end(); 
                   it++ )
        {
            auto _linkPtr = it->second;

            if ( _linkPtr )
            {
                if ( !_linkPtr->parentLink )
                {
                    urdfModelPtr->rootLinks.push_back( _linkPtr );
                }
            }
        }

        if ( urdfModelPtr->rootLinks.size() > 1 )
        {
            parsing::logWarning( "URDF file with multiple root links found" );
        }

        if ( urdfModelPtr->rootLinks.size() == 0 )
        {
            parsing::logError( "URDF without root link found" );
            return false;
        }

        return true;
    }

    void deepCopy( UrdfModel* target, 
                   UrdfModel* source,
                   const std::string& agentName )
    {
        target->name = agentName;

        // Create materials from dictionary of material in the source model
        for ( auto it = source->materials.begin();
                   it != source->materials.end();
                   it++ )
        {
            auto _targetMaterial = new UrdfMaterial();
            auto _sourceMaterial = it->second;

            // Just copy everything directly (no pointer references here)
            *( _targetMaterial ) = *( _sourceMaterial );

            // and store it into the target's materials dictionary
            target->materials[ _targetMaterial->name ] = _targetMaterial;
        }

        // Create links from dictionary of links in the source model
        for ( auto it = source->links.begin();
                   it != source->links.end();
                   it++ )
        {
            auto _targetLink = new UrdfLink();
            auto _sourceLink = it->second;

            _deepCopyLink( _targetLink, _sourceLink, agentName );

            // and store it into the target's links dictionary
            target->links[ _targetLink->name ] = _targetLink;
        }

        // Create joints from dictionary of joints in the source model
        for ( auto it = source->joints.begin();
                   it != source->joints.end();
                   it++ )
        {
            auto _targetJoint = new UrdfJoint();
            auto _sourceJoint = it->second;

            _deepCopyJoint( _targetJoint, _sourceJoint, agentName );

            // and store it into the target's joints dictionary
            target->joints[ _targetJoint->name ] = _targetJoint;
        }

        // Assemble the links and joints
        if ( !_initTreeAndRoot( target ) )
        {
            std::cout << "WARNING> There was an issue copying the model for "
                      << "the agent with name: " << agentName << std::endl;
        }
    }

    void _deepCopyLink( UrdfLink* targetLink,
                        UrdfLink* sourceLink,
                        const std::string& agentName )
    {
        // grab the name of the source accordingly
        targetLink->name = computeUrdfName( "link",
                                            sourceLink->name,
                                            agentName );

        // and the inertia
        if ( sourceLink->inertia )
        {
            targetLink->inertia         = new UrdfInertia();
            *( targetLink->inertia )    = *( sourceLink->inertia );
        }

        // and the visuals
        auto _visualsSource = sourceLink->visuals;
        for ( size_t i = 0; i < _visualsSource.size(); i++ )
        {
            auto _visualSource = _visualsSource[i];
            auto _visualTarget = new UrdfVisual();

            // grab the visual name accordingly
            _visualTarget->name = computeUrdfName( "visual",
                                                   _visualSource->name,
                                                   agentName );

            // and continue copyting everything else
            _visualTarget->localTransform   = _visualSource->localTransform;
            *( _visualTarget->material )    = *( _visualSource->material );
            *( _visualTarget->geometry )    = *( _visualSource->geometry );

            targetLink->visuals.push_back( _visualTarget );

            CURRENT_VISUALS_COUNTER++;
        }

        // and the collisions
        auto _collisionsSource = sourceLink->collisions;
        for ( size_t i = 0; i < _collisionsSource.size(); i++ )
        {
            auto _collisionSource = _collisionsSource[i];
            auto _collisionTarget = new UrdfCollision();

            // grab the collision name accordingly
            _collisionTarget->name = computeUrdfName( "collision",
                                                      _collisionSource->name,
                                                      agentName );

            // and continue copyting everything else
            _collisionTarget->localTransform   = _collisionSource->localTransform;
            *( _collisionTarget->geometry )    = *( _collisionSource->geometry );

            targetLink->collisions.push_back( _collisionTarget );

            CURRENT_COLLISIONS_COUNTER++;
        }
    }

    void _deepCopyJoint( UrdfJoint* targetJoint,
                         UrdfJoint* sourceJoint,
                         const std::string& agentName )
    {
        // Firstly, copy everything directly (no pointer references here)
        *( targetJoint ) = *( sourceJoint );

        // and then modify some names accordingly (for generic/unique representation)
        targetJoint->name = computeUrdfName( "joint",
                                             sourceJoint->name,
                                             agentName );

        targetJoint->parentLinkName = computeUrdfName( "link",
                                                       sourceJoint->parentLinkName,
                                                       agentName );

        targetJoint->childLinkName = computeUrdfName( "link",
                                                      sourceJoint->childLinkName,
                                                      agentName );
    }

    std::string computeUrdfName( const std::string& type, 
                                 const std::string& elementName, 
                                 const std::string& agentName )
    {
        std::string _res;

        if ( type == "link" )
        {
            _res += std::string( TYSOC_PREFIX_BODY ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "joint" )
        {
            _res += std::string( TYSOC_PREFIX_JOINT ) + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "visual" || type == "collision" )
        {
            if ( elementName == "" )
            {
                std::string _fixedElementName;

                if ( type == "visual" )
                    _fixedElementName = std::to_string( CURRENT_VISUALS_COUNTER );
                else
                    _fixedElementName = std::to_string( CURRENT_COLLISIONS_COUNTER );

                _res += std::string( TYSOC_PREFIX_GEOM ) + agentName + std::string( "_" ) + _fixedElementName;
            }
            else
            {
                _res += std::string( TYSOC_PREFIX_GEOM ) + agentName + std::string( "_" ) + elementName;
            }
        }
        else if ( type == "actuator" )
        {
            _res += std::string( TYSOC_PREFIX_ACTUATOR ) + agentName + std::string( "_" ) + elementName;
        }
        else
        {
            std::cout << "WARNING> not supported urdf type: " 
                      << type << " for urdfname generation" << std::endl;

            _res += elementName;
        }

        return _res;
    }

}}