
#include <string>
#include <iostream>

#include <utils/parsers/rlsim/rlsim.h>

#ifndef TYSOCCORE_RESOURCES_PATH
    // default in case of out-of-place build
    #define TYSOCCORE_RESOURCES_PATH "../../res/"
#endif

static std::string RLSIM_MODEL = "biped3d";
static int RLSIM_JOINT_TEST_INDEX = 0;

int main( int argc, const char** argv )
{
    if ( argc > 1 )
    {
        try
        {
            RLSIM_MODEL = argv[1];
        }
        catch ( const std::exception& e )
        {
            std::cout << "ERROR> Should pass a string for the rlsim model name" << std::endl;
            std::cerr << e.what() << "\n";
            return 1;
        }

        if ( argc > 2 )
        {
            try
            {
                RLSIM_JOINT_TEST_INDEX = std::stoi( argv[2] );
            }
            catch ( const std::exception& e )
            {
                std::cout << "ERROR> Test index for joint should be an int"
                          << ", but passed " << argv[2] << " instead" << std::endl;
                std::cerr << e.what() << "\n";
                return 1;
            }
        }
    }

    std::string _filepath;
    _filepath += TYSOCCORE_RESOURCES_PATH;
    _filepath += "rlsim/";
    // _filepath += "biped3d";
    // _filepath += "dog3d";
    // _filepath += "raptor3d";
    _filepath += RLSIM_MODEL;
    _filepath += ".json";

    std::cout << "INFO> started parsing model: " << _filepath << std::endl;

    auto _model = tysoc::rlsim::loadGenericModel( _filepath );

    if ( !_model )
    {
        return 1;
    }

    std::cout << "INFO> finished parsing" << std::endl;

    std::cout << "TESTING SOME INFO FROM URDF" << std::endl;

    std::cout << "rootJoint: "  << _model->rootJoint->name << std::endl;
    std::cout << "numBodies: "  << _model->bodies.size() << std::endl;
    std::cout << "numJoints: "  << _model->joints.size() << std::endl;
    std::cout << "numVisuals: " << _model->visuals.size() << std::endl;

    if ( RLSIM_JOINT_TEST_INDEX >= _model->joints.size() ||
         RLSIM_JOINT_TEST_INDEX < 0 )
    {
        std::cout << "WARNING> Test joint index "
                  << RLSIM_JOINT_TEST_INDEX
                  << " is out of range" << std::endl;
        RLSIM_JOINT_TEST_INDEX = 0;
    }

    auto _joint = _model->joints[RLSIM_JOINT_TEST_INDEX];
    std::cout << "joint->name: " << _joint->name << std::endl;
    std::cout << "joint->type: " << _joint->type << std::endl;
    std::cout << "joint->parentJointId: " << _joint->parentJointId << std::endl;
    std::cout << "joint->localPos: " << tysoc::TVec3::toString( _joint->localPos ) << std::endl;
    std::cout << "joint->localEuler: " << tysoc::TVec3::toString( _joint->localEuler ) << std::endl;
    std::cout << "joint->childJoints.size(): " << _joint->childJoints.size() << std::endl;
    std::cout << "joint->childVisuals.size(): " << _joint->childVisuals.size() << std::endl;
    std::cout << "joint->childBodies.size(): " << _joint->childBodies.size() << std::endl;
    std::cout << "joint->torqueLimit: " << _joint->torqueLimit << std::endl;

    return 0;
}