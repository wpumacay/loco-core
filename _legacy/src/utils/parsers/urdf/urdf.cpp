
#include <utils/parsers/urdf/urdf.h>


namespace tysoc {
namespace urdf {

    std::map< std::string, UrdfMaterial > UrdfMaterial::MATERIALS;

    UrdfModel* loadGenericModel( const std::string& modelfile )
    {
        tinyxml2::XMLDocument _doc;
        _doc.LoadFile( modelfile.c_str() );

        if ( _doc.Error() )
        {
            parsing::logError( _doc.ErrorStr() );
            _doc.Clear();

            return nullptr;
        }

        auto _robotElement = _doc.FirstChildElement( "robot" );
        if ( !_robotElement )
        {
            parsing::logError( "expected a robot element" );
            return nullptr;
        }

        // Define some general settings to fix model discrepancies
        auto _worldUp = std::string( "z" );
        auto _zeroPos = TVec3( 0, 0, 0 );
        auto _zeroRot = TVec3( 0, 0, 0 );
        auto _settingsElement = _robotElement->FirstChildElement( "settings" );
        
        if ( _settingsElement )
        {
            _worldUp = xml::safeParseString( _settingsElement, "worldUp", "z" );
            _zeroPos = xml::safeParseVec3( _settingsElement, "zeroPos", { 0.0f, 0.0f, 0.0f } );
            _zeroRot = xml::safeParseVec3( _settingsElement, "zeroRot", { 0.0f, 0.0f, 0.0f } );
        }

        auto _urdfModel = new UrdfModel();
        // set first the zero-configuration compensation
        _urdfModel->zeroCompensation = TMat4::fromPositionAndRotation( _zeroPos,
                                                                       TMat3::fromEuler( _zeroRot ) );

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
            _urdfLink->collectAttribs( _linkElement, _worldUp );

            // cache this link for later initialization
            if ( _urdfModel->links.find( _urdfLink->name ) != _urdfModel->links.end() )
                parsing::logError( std::string( "Link with name: " ) + _urdfLink->name + "already exists" );
            else
                _urdfModel->links[_urdfLink->name] = _urdfLink;
        }

        // Get all Joint elements
        for ( auto _jointElement = _robotElement->FirstChildElement( "joint" ); 
                   _jointElement != NULL; 
                   _jointElement = _jointElement->NextSiblingElement( "joint" ) )
        {
            // create a joint
            auto _urdfJoint = new UrdfJoint();
            _urdfJoint->collectAttribs( _jointElement, _worldUp );

            // cache this joint for later initialization
            if ( _urdfModel->joints.find( _urdfJoint->name ) != _urdfModel->joints.end() )
                parsing::logError( std::string( "Joint with name: " ) + _urdfJoint->name + " already exists" );
            else
                _urdfModel->joints[_urdfJoint->name] = _urdfJoint;
        }

        // Get all exclusion elements
        for ( auto _exclusionElement = _robotElement->FirstChildElement( "exclude" );
                   _exclusionElement != NULL;
                   _exclusionElement = _exclusionElement->NextSiblingElement( "exclude" ) )
        {
            // Grab the names of the pair of bodies to exclude
            auto _link1Name = xml::safeParseString( _exclusionElement, "link1", "" );
            auto _link2Name = xml::safeParseString( _exclusionElement, "link2", "" );

            if ( _link1Name == "" || _link2Name == "" )
                continue;

            _urdfModel->exclusionPairs.push_back( std::make_pair( _link1Name, _link2Name ) );
        }

        if ( !_initTreeAndRoot( _urdfModel ) )
            return nullptr;

        // Copy the materials cache into this model
        _urdfModel->materials = UrdfMaterial::MATERIALS;

        // store path references for later usage (when handling meshes)
        _urdfModel->filepath = modelfile;
        _urdfModel->filename = tysoc::getFilenameFromFilePath( modelfile );
        _urdfModel->folderpath = tysoc::getFolderpathFromFilePath( modelfile );

        return _urdfModel;
    }

    bool _initTreeAndRoot( UrdfModel* urdfModelPtr )
    {
        // loop through all joints, for every link, assign children links and children joints
        for ( auto pair : urdfModelPtr->joints )
        {
            auto _jointPtr = pair.second;

            if ( _jointPtr )
            {
                UrdfLink* _childLink = nullptr;

                if ( urdfModelPtr->links.find( _jointPtr->childLinkName ) != urdfModelPtr->links.end() )
                {
                    _childLink = urdfModelPtr->links[_jointPtr->childLinkName];
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
                    _childLink->parentLink  = nullptr;
                    continue;
                }

                UrdfLink* _parentLink = nullptr;
                
                if ( urdfModelPtr->links.find( _jointPtr->parentLinkName ) != urdfModelPtr->links.end() )
                {
                    _parentLink = urdfModelPtr->links[_jointPtr->parentLinkName];
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
                    _parentLink->joints.push_back( _jointPtr );
                    _parentLink->children.push_back( _childLink );
                }
            }
        }

        //search for children that have no parent, those are 'root'
        for ( auto pair : urdfModelPtr->links )
        {
            auto _linkPtr = pair.second;
            if ( _linkPtr && !_linkPtr->parentLink )
                    urdfModelPtr->rootLinks.push_back( _linkPtr );
        }

        if ( urdfModelPtr->rootLinks.size() > 1 )
            parsing::logWarning( "URDF file with multiple root links found" );

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
        for ( auto pair : source->materials )
        {
            // Just copy everything directly (there are no pointer references here)
            UrdfMaterial _targetMaterial = pair.second;
            // and store it into the target's materials dictionary
            target->materials[_targetMaterial.name] = _targetMaterial;
        }

        // Create links from dictionary of links in the source model
        for ( auto pair : source->links )
        {
            auto _targetLink = new UrdfLink();
            auto _sourceLink = pair.second;

            _deepCopyLink( _targetLink, _sourceLink, agentName );

            // and store it into the target's links dictionary
            target->links[_targetLink->name] = _targetLink;
        }

        // Create joints from dictionary of joints in the source model
        for ( auto pair : source->joints )
        {
            auto _targetJoint = new UrdfJoint();
            auto _sourceJoint = pair.second;

            _deepCopyJoint( _targetJoint, _sourceJoint, agentName );

            // and store it into the target's joints dictionary
            target->joints[_targetJoint->name] = _targetJoint;
        }

        // Change the exclusion contact pairs names appropriately
        for ( auto pair : source->exclusionPairs )
        {
            auto _newName1 = computeUrdfName( "link", pair.first, agentName );
            auto _newName2 = computeUrdfName( "link", pair.second, agentName );

            target->exclusionPairs.push_back( std::make_pair( _newName1, _newName2 ) );
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
        targetLink->name = computeUrdfName( "link", sourceLink->name, agentName );

        // and the inertia
        targetLink->inertia = sourceLink->inertia;

        // and the visuals
        for ( auto _visualSource : sourceLink->visuals )
        {
            // copy everything from the source's visual
            UrdfVisual _visualTarget = _visualSource;
            // modify the visual name accordingly
            _visualTarget.name = computeUrdfName( "visual", _visualSource.name, agentName );

            targetLink->visuals.push_back( _visualTarget );
            CURRENT_VISUALS_COUNTER++;
        }

        // and the collisions
        for ( auto _collisionSource : sourceLink->collisions )
        {
            // copy everything from the source's collision
            UrdfCollision _collisionTarget = _collisionSource;
            // modify the collision name accordingly
            _collisionTarget.name = computeUrdfName( "collision", _collisionSource.name, agentName );

            targetLink->collisions.push_back( _collisionTarget );
            CURRENT_COLLISIONS_COUNTER++;
        }
    }

    void _deepCopyJoint( UrdfJoint* targetJoint,
                         UrdfJoint* sourceJoint,
                         const std::string& agentName )
    {
        // Copy everything directly (no pointer references here)
        *( targetJoint ) = *( sourceJoint );

        // modify some names accordingly (for generic/unique representation)
        targetJoint->name = computeUrdfName( "joint", sourceJoint->name, agentName );
        targetJoint->parentLinkName = computeUrdfName( "link", sourceJoint->parentLinkName, agentName );
        targetJoint->childLinkName = computeUrdfName( "link", sourceJoint->childLinkName, agentName );
    }

    std::string computeUrdfName( const std::string& type, 
                                 const std::string& elementName, 
                                 const std::string& agentName )
    {
        std::string _res;

        if ( type == "link" )
        {
            _res += PREFIX_BODY + agentName + std::string( "_" ) + elementName;
        }
        else if ( type == "joint" )
        {
            _res += PREFIX_JOINT + agentName + std::string( "_" ) + elementName;
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

                _res += PREFIX_GEOM + agentName + std::string( "_" ) + _fixedElementName;
            }
            else
            {
                _res += PREFIX_GEOM + agentName + std::string( "_" ) + elementName;
            }
        }
        else if ( type == "actuator" )
        {
            _res += PREFIX_ACTUATOR + agentName + std::string( "_" ) + elementName;
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