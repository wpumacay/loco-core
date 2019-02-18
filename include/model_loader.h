
#pragma once

#include <utils/parsers/mjcf/mjcf.h>
#include <utils/parsers/urdf/urdf.h>
#include <utils/parsers/rlsim/rlsim.h>

#include <dirent.h>

namespace tysoc
{

    class TModelLoader
    {

        private :

        std::vector< std::string > m_templateMjcfModelFiles;
        std::vector< std::string > m_templateUrdfModelFiles;
        std::vector< std::string > m_templateRlsimModelFiles;

        std::map< std::string, mjcf::GenericElement* >  m_cachedMjcfModels;
        std::map< std::string, urdf::UrdfModel* >       m_cachedUrdfModels;
        std::map< std::string, rlsim::RlsimModel* >     m_cachedRlsimModels;

        static TModelLoader* _INSTANCE;

        TModelLoader();

        void _precacheModels();
        void _precacheMjcfModels();
        void _precacheSingleMjcfModel( const std::string& templateFile );
        void _precacheUrdfModels();
        void _precacheSingleUrdfModel( const std::string& templateFile );
        void _precacheRlsimModels();
        void _precacheSingleRlsimModel( const std::string& templateFile );

        public :

        static TModelLoader* GetInstance();
        ~TModelLoader();

        mjcf::GenericElement* getMjcfModel( const std::string& name );
        urdf::UrdfModel* getUrdfModel( const std::string& name );
        rlsim::RlsimModel* getRlsimModel( const std::string& name );

    };

}