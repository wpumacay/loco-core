
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

        std::string m_pathTemplatesMjcf;
        std::string m_pathTemplatesUrdf;
        std::string m_pathTemplatesRlsim;

        static TModelLoader* _INSTANCE;

        TModelLoader( const std::string& pathTemplatesMjcf,
                      const std::string& pathTemplatesUrdf,
                      const std::string& pathTemplatesRlsim );

        void _precacheModels();
        void _precacheMjcfModels();
        void _precacheSingleMjcfModel( const std::string& templateFile );
        void _precacheUrdfModels();
        void _precacheSingleUrdfModel( const std::string& templateFile );
        void _precacheRlsimModels();
        void _precacheSingleRlsimModel( const std::string& templateFile );

        public :

        static TModelLoader* GetInstance();
        static TModelLoader* Create( const std::string& pathTemplatesMjcf = TYSOC_PATH_MJCF_TEMPLATES,
                                     const std::string& pathTemplatesUrdf = TYSOC_PATH_URDF_TEMPLATES,
                                     const std::string& pathTemplatesRlsim = TYSOC_PATH_RLSIM_TEMPLATES );
        ~TModelLoader();

        bool hasMjcfModel( const std::string& name );
        bool hasUrdfModel( const std::string& name );
        bool hasRlsimModel( const std::string& name );

        mjcf::GenericElement* getMjcfModel( const std::string& name );
        urdf::UrdfModel* getUrdfModel( const std::string& name );
        rlsim::RlsimModel* getRlsimModel( const std::string& name );

    };

}