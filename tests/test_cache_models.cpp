
#include <model_loader.h>

int main()
{

    std::cout << "STARTED TEST" << std::endl;

    auto _loader = tysoc::TModelLoader::Create( TYSOC_PATH_MJCF_TEMPLATES,
                                                TYSOC_PATH_URDF_TEMPLATES,
                                                TYSOC_PATH_RLSIM_TEMPLATES );
    
    auto _mjcfModel = _loader->getMjcfModel( "humanoid" );
    auto _urdfModel = _loader->getUrdfModel( "laikago" );
    auto _rlsimModel = _loader->getRlsimModel( "dog3d" );

    if ( _mjcfModel )
        _mjcfModel->print();

    if ( _urdfModel )
        std::cout << "urdf::name> " << _urdfModel->name << std::endl;

    if ( _rlsimModel )
        std::cout << "rlsim::numbodies> " << _rlsimModel->bodies.size() << std::endl;

    std::cout << "FINISHED TEST" << std::endl;

    return 0;
}