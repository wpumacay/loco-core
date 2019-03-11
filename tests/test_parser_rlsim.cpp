
#include <string>
#include <iostream>

#include <utils/parsers/rlsim/rlsim.h>

static std::string RLSIM_MODEL = "biped3d";
static int RLSIM_JOINT_TEST_INDEX = 0;

void printJointInfo( tysoc::rlsim::RlsimJoint* jointPtr );

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
    _filepath += TYSOC_PATH_RLSIM_TEMPLATES;
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

    std::cout << "CHECKING SOME INFO FROM RLSIM" << std::endl;

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
    printJointInfo( _joint );

    std::cout << "CHECKING DEEPCOPY FUNCTIONALITY" << std::endl;

    auto _modelCopy = new tysoc::rlsim::RlsimModel();
    tysoc::rlsim::deepCopy( _modelCopy, _model, "gregor" );

    std::cout << "name: "       << _modelCopy->name << std::endl;
    std::cout << "rootJoint: "  << _modelCopy->rootJoint->name << std::endl;
    std::cout << "numBodies: "  << _modelCopy->bodies.size() << std::endl;
    std::cout << "numJoints: "  << _modelCopy->joints.size() << std::endl;
    std::cout << "numVisuals: " << _modelCopy->visuals.size() << std::endl;

    _joint = _modelCopy->joints[RLSIM_JOINT_TEST_INDEX];
    printJointInfo( _joint );

    return 0;
}

void printJointInfo( tysoc::rlsim::RlsimJoint* jointPtr )
{
    std::cout << "joint->name: " << jointPtr->name << std::endl;
    std::cout << "joint->type: " << jointPtr->type << std::endl;
    std::cout << "joint->parentJointId: " << jointPtr->parentJointId << std::endl;
    std::cout << "joint->localPos: " << tysoc::TVec3::toString( jointPtr->localPos ) << std::endl;
    std::cout << "joint->localEuler: " << tysoc::TVec3::toString( jointPtr->localEuler ) << std::endl;
    std::cout << "joint->childJoints.size(): " << jointPtr->childJoints.size() << std::endl;
    std::cout << "joint->childVisuals.size(): " << jointPtr->childVisuals.size() << std::endl;
    std::cout << "joint->childBodies.size(): " << jointPtr->childBodies.size() << std::endl;
    std::cout << "joint->torqueLimit: " << jointPtr->torqueLimit << std::endl;
}