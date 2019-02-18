
#include <model_loader.h>


namespace tysoc 
{

    TModelLoader* TModelLoader::_INSTANCE = NULL;

    TModelLoader* TModelLoader::GetInstance()
    {
        if ( !TModelLoader::_INSTANCE )
            TModelLoader::_INSTANCE = new TModelLoader();

        return TModelLoader::_INSTANCE;
    }

    TModelLoader::TModelLoader()
    {
        _precacheModels();
    }

    TModelLoader::~TModelLoader()
    {
        m_templateMjcfModelFiles.clear();
        m_templateUrdfModelFiles.clear();
        m_templateRlsimModelFiles.clear();

        for ( auto it = m_cachedMjcfModels.begin();
              it != m_cachedMjcfModels.end();
              it++ )
        {
            delete it->second;
        }
        m_cachedMjcfModels.clear();

        for ( auto it = m_cachedUrdfModels.begin();
              it != m_cachedUrdfModels.end();
              it++ )
        {
            delete it->second;
        }
        m_cachedUrdfModels.clear();

        for ( auto it = m_cachedRlsimModels.begin();
              it != m_cachedRlsimModels.end();
              it++ )
        {
            delete it->second;
        }
        m_cachedRlsimModels.clear();
    }

    void TModelLoader::_precacheModels()
    {
        _precacheMjcfModels();
        _precacheUrdfModels();
        _precacheRlsimModels();
    }

    void TModelLoader::_precacheMjcfModels()
    {
        std::vector< std::string > _filesToCache;

        // Get the template files from the templates folder *****************
        DIR* _directoryPtr;
        struct dirent* _direntPtr;

        std::string _templatesFolderPath;
        _templatesFolderPath += TYSOCCORE_RESOURCES_PATH;
        _templatesFolderPath += "templates/mjcf/";

        _directoryPtr = opendir( _templatesFolderPath.c_str() );
        if ( _directoryPtr )
        {
            while ( _direntPtr = readdir( _directoryPtr ) )
            {
                std::string _fname = _direntPtr->d_name;
                if ( _fname.find( ".xml" ) != std::string::npos )
                {
                    m_templateMjcfModelFiles.push_back( _fname );
                }
            }
        }
        closedir( _directoryPtr );

        // ******************************************************************

        for ( size_t i = 0; i < m_templateMjcfModelFiles.size(); i++ )
        {
            _precacheSingleMjcfModel( m_templateMjcfModelFiles[ i ] );
        }
    }

    void TModelLoader::_precacheUrdfModels()
    {
        std::vector< std::string > _filesToCache;

        // Get the template files from the templates folder *****************
        DIR* _directoryPtr;
        struct dirent* _direntPtr;

        std::string _templatesFolderPath;
        _templatesFolderPath += TYSOCCORE_RESOURCES_PATH;
        _templatesFolderPath += "templates/urdf/";

        _directoryPtr = opendir( _templatesFolderPath.c_str() );
        if ( _directoryPtr )
        {
            while ( _direntPtr = readdir( _directoryPtr ) )
            {
                std::string _fname = _direntPtr->d_name;
                if ( _fname.find( ".urdf" ) != std::string::npos )
                {
                    m_templateUrdfModelFiles.push_back( _fname );
                }
            }
        }
        closedir( _directoryPtr );

        // ******************************************************************

        for ( size_t i = 0; i < m_templateUrdfModelFiles.size(); i++ )
        {
            _precacheSingleUrdfModel( m_templateUrdfModelFiles[ i ] );
        }
    }

    void TModelLoader::_precacheRlsimModels()
    {
        std::vector< std::string > _filesToCache;

        // Get the template files from the templates folder *****************
        DIR* _directoryPtr;
        struct dirent* _direntPtr;

        std::string _templatesFolderPath;
        _templatesFolderPath += TYSOCCORE_RESOURCES_PATH;
        _templatesFolderPath += "templates/rlsim/";

        _directoryPtr = opendir( _templatesFolderPath.c_str() );
        if ( _directoryPtr )
        {
            while ( _direntPtr = readdir( _directoryPtr ) )
            {
                std::string _fname = _direntPtr->d_name;
                if ( _fname.find( ".json" ) != std::string::npos )
                {
                    m_templateRlsimModelFiles.push_back( _fname );
                }
            }
        }
        closedir( _directoryPtr );

        // ******************************************************************

        for ( size_t i = 0; i < m_templateRlsimModelFiles.size(); i++ )
        {
            _precacheSingleRlsimModel( m_templateRlsimModelFiles[ i ] );
        }
    }

    void TModelLoader::_precacheSingleMjcfModel( const std::string& templateFile )
    {
        std::cout << "INFO> trying to load template: " << templateFile << std::endl;
        // Gran the model into a mjcf::GenericElement node
        auto _root = mjcf::loadGenericModel( std::string( TYSOCCORE_RESOURCES_PATH ) + "templates/mjcf/" + templateFile );

        // Extract the name to use as key in the cache dictionary
        size_t _xmlTagPos = templateFile.find( ".xml" );
        std::string _modelName = templateFile.substr( 0, _xmlTagPos );

        // cache the model
        m_cachedMjcfModels[ _modelName ] = _root;

        std::cout << "INFO> Precached mjcf template model: " << _modelName << std::endl;
    }

    void TModelLoader::_precacheSingleUrdfModel( const std::string& templateFile )
    {
        std::cout << "INFO> trying to load template: " << templateFile << std::endl;
        // Gran the model into a mjcf::GenericElement node
        auto _urdfModel = urdf::loadGenericModel( std::string( TYSOCCORE_RESOURCES_PATH ) + "templates/urdf/" + templateFile );

        // Extract the name to use as key in the cache dictionary
        size_t _xmlTagPos = templateFile.find( ".urdf" );
        std::string _modelName = templateFile.substr( 0, _xmlTagPos );

        // cache the model
        m_cachedUrdfModels[ _modelName ] = _urdfModel;

        std::cout << "INFO> Precached urdf template model: " << _modelName << std::endl;
    }

    void TModelLoader::_precacheSingleRlsimModel( const std::string& templateFile )
    {
        std::cout << "INFO> trying to load template: " << templateFile << std::endl;
        // Gran the model into a mjcf::GenericElement node
        auto _rlsimModel = rlsim::loadGenericModel( std::string( TYSOCCORE_RESOURCES_PATH ) + "templates/rlsim/" + templateFile );

        // Extract the name to use as key in the cache dictionary
        size_t _jsonTagPos = templateFile.find( ".json" );
        std::string _modelName = templateFile.substr( 0, _jsonTagPos );

        // cache the model
        m_cachedRlsimModels[ _modelName ] = _rlsimModel;

        std::cout << "INFO> Precached rlsim template model: " << _modelName << std::endl;
    }

    mjcf::GenericElement* TModelLoader::getMjcfModel( const std::string& name )
    {
        if ( m_cachedMjcfModels.find( name ) == m_cachedMjcfModels.end() )
        {
            std::cout << "ERROR> Requested mjcf model: " << name << " has not been loaded" << std::endl;
            return NULL;
        }

        return m_cachedMjcfModels[name];
    }

    urdf::UrdfModel* TModelLoader::getUrdfModel( const std::string& name )
    {
        if ( m_cachedUrdfModels.find( name ) == m_cachedUrdfModels.end() )
        {
            std::cout << "ERROR> Requested urdf model: " << name << " has not been loaded" << std::endl;
            return NULL;
        }

        return m_cachedUrdfModels[name];
    }

    rlsim::RlsimModel* TModelLoader::getRlsimModel( const std::string& name )
    {
        if ( m_cachedRlsimModels.find( name ) == m_cachedRlsimModels.end() )
        {
            std::cout << "ERROR> Requested rlsim model: " << name << " has not been loaded" << std::endl;
            return NULL;
        }

        return m_cachedRlsimModels[name];
    }


}