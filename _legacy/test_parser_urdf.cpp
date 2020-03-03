
#include <string>
#include <iostream>

#include <utils/parsers/urdf/urdf.h>

static std::string URDF_MODEL = "laikago";

int main( int argc, const char** argv )
{
    if ( argc > 1 )
    {
        try
        {
            URDF_MODEL = argv[1];
        }
        catch ( const std::exception& e )
        {
            std::cout << "ERROR> Should pass a string for the urdf model name" << std::endl;
            std::cerr << e.what() << '\n';
            return 1;
        }
    }

    std::cout << "INFO> started parsing" << std::endl;

    std::string _filepath;
    _filepath += TYSOC_PATH_URDF_TEMPLATES;
    // _filepath += "atlas.urdf";
    // _filepath += "baxter.urdf";
    // _filepath += "cassie.urdf";
    // _filepath += "laikago.urdf";
    _filepath += URDF_MODEL;
    _filepath += ".urdf";


    auto _model = tysoc::urdf::loadGenericModel( _filepath );

    std::cout << "INFO> finished parsing" << std::endl;

    std::cout << "TESTING SOME INFO FROM URDF" << std::endl;

    std::cout << "modelName: "      << _model->name << std::endl;
    std::cout << "numRootLinks: "   << _model->rootLinks.size() << std::endl;
    std::cout << "numLinks: "       << _model->links.size() << std::endl;
    std::cout << "numJoints: "      << _model->joints.size() << std::endl;
    std::cout << "numMaterials: "   << _model->materials.size() << std::endl;

    // show root links (sanity check, in case there are more than 1)
    for ( size_t i = 0; i < _model->rootLinks.size(); i++ )
    {
        auto _rootLink = _model->rootLinks[i];
        std::cout << "rootName(" << ( i ) << "): " << _rootLink->name << std::endl;
    }

    return 0;
}