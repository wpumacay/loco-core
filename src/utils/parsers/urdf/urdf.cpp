
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
                               std::string( " has no parent link" ) );

                    continue;
                }

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
                               std::string( " has no child link" ) );

                    continue;
                }

                _childLink->parentJoint = _jointPtr;
                _childLink->parentLink  = _parentLink;

                _parentLink->childJoints.push_back( _jointPtr );
                _parentLink->childLinks.push_back( _childLink );
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

}}