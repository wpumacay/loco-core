
#include <utils/parsers/rlsim/rlsim_elements.h>

namespace tysoc {
namespace rlsim {

    TVec3 rearrange( const TVec3& vec, const std::string& worldUp )
    {
        if ( worldUp == "y" )
            return TVec3( vec.z, vec.x, vec.y );
        else if ( worldUp == "x" )
            return TVec3( vec.y, vec.z, vec.x );

        return vec;
    }

    void RlsimVisual::collectAttribs( const nlohmann::json& jsonElement, const std::string& worldUp )
    {
        this->name  = tysoc::json::safeParseString( jsonElement, "Name", "undefined" );
        this->shape = tysoc::json::safeParseString( jsonElement, "Shape", "box" );
        this->parentJointId = tysoc::json::safeParseInt( jsonElement, "ParentJoint", -1 );

        // size from param 0-2
        this->size.x = tysoc::json::safeParseFloat( jsonElement, "Param0", 0.1f );
        this->size.y = tysoc::json::safeParseFloat( jsonElement, "Param1", 0.1f );
        this->size.z = tysoc::json::safeParseFloat( jsonElement, "Param2", 0.1f );
        this->size = rearrange( this->size, worldUp );
        // local position from attach x-z
        this->localPos.x = tysoc::json::safeParseFloat( jsonElement, "AttachX", 0.0f );
        this->localPos.y = tysoc::json::safeParseFloat( jsonElement, "AttachY", 0.0f );
        this->localPos.z = tysoc::json::safeParseFloat( jsonElement, "AttachZ", 0.0f );
        this->localPos = rearrange( this->localPos, worldUp );
        // local orientation from attachTheta x-z
        this->localEuler.x = tysoc::json::safeParseFloat( jsonElement, "AttachThetaX", 0.0f );
        this->localEuler.y = tysoc::json::safeParseFloat( jsonElement, "AttachThetaY", 0.0f );
        this->localEuler.z = tysoc::json::safeParseFloat( jsonElement, "AttachThetaZ", 0.0f );
        this->localEuler = rearrange( this->localEuler, worldUp );
        // color from color r-g-b-a
        this->rgba.x = tysoc::json::safeParseFloat( jsonElement, "ColorR", 0.3f );
        this->rgba.y = tysoc::json::safeParseFloat( jsonElement, "ColorG", 0.4f );
        this->rgba.z = tysoc::json::safeParseFloat( jsonElement, "ColorB", 0.5f );
        this->rgba.w = tysoc::json::safeParseFloat( jsonElement, "ColorA", 1.0f );
    }

    void RlsimBody::collectAttribs( const nlohmann::json& jsonElement, const std::string& worldUp )
    {
        this->name  = tysoc::json::safeParseString( jsonElement, "Name", "undefined" );
        this->shape = tysoc::json::safeParseString( jsonElement, "Shape", "box" );
        this->mass  = tysoc::json::safeParseFloat( jsonElement, "Mass", 1.0f );
        this->collGroup = tysoc::json::safeParseInt( jsonElement, "ColGroup", -1 );

        // size from param 0-2
        this->size.x = tysoc::json::safeParseFloat( jsonElement, "Param0", 0.1f );
        this->size.y = tysoc::json::safeParseFloat( jsonElement, "Param1", 0.1f );
        this->size.z = tysoc::json::safeParseFloat( jsonElement, "Param2", 0.1f );
        this->size = rearrange( this->size, worldUp );
        // local position from attach x-z
        this->localPos.x = tysoc::json::safeParseFloat( jsonElement, "AttachX", 0.0f );
        this->localPos.y = tysoc::json::safeParseFloat( jsonElement, "AttachY", 0.0f );
        this->localPos.z = tysoc::json::safeParseFloat( jsonElement, "AttachZ", 0.0f );
        this->localPos = rearrange( this->localPos, worldUp );
        // local orientation from attachTheta x-z
        this->localEuler.x = tysoc::json::safeParseFloat( jsonElement, "AttachThetaX", 0.0f );
        this->localEuler.y = tysoc::json::safeParseFloat( jsonElement, "AttachThetaY", 0.0f );
        this->localEuler.z = tysoc::json::safeParseFloat( jsonElement, "AttachThetaZ", 0.0f );
        this->localEuler = rearrange( this->localEuler, worldUp );
        // color from color r-g-b-a
        this->rgba.x = tysoc::json::safeParseFloat( jsonElement, "ColorR", 0.3f );
        this->rgba.y = tysoc::json::safeParseFloat( jsonElement, "ColorG", 0.4f );
        this->rgba.z = tysoc::json::safeParseFloat( jsonElement, "ColorB", 0.5f );
        this->rgba.w = tysoc::json::safeParseFloat( jsonElement, "ColorA", 1.0f );
    }

    void RlsimJoint::collectAttribs( const nlohmann::json& jsonElement, const std::string& worldUp )
    {
        this->name = tysoc::json::safeParseString( jsonElement, "Name", "undefined" );
        this->type = tysoc::json::safeParseString( jsonElement, "Type", "none" );
        this->parentJointId = tysoc::json::safeParseInt( jsonElement, "Parent", -1 );
        this->torqueLimit = tysoc::json::safeParseFloat( jsonElement, "TorqueLim", 0.0f );

        // angle limits, number of low-high pairs depends on the joint type
        size_t _numDof = ( this->type == "spherical" ) ? 3 : 1;
        for ( size_t q = 0; q < _numDof; q++ )
        {
            float _limLow   = tysoc::json::safeParseFloat( jsonElement, std::string( "LimLow" ) + std::to_string( q ), 0.0f );
            float _limHigh  = tysoc::json::safeParseFloat( jsonElement, std::string( "LimHigh" ) + std::to_string( q ), 0.0f );

            this->limitsPerDof.push_back( { _limLow, _limHigh } );
        }

        // local position from attach x-z
        this->localPos.x = tysoc::json::safeParseFloat( jsonElement, "AttachX", 0.0f );
        this->localPos.y = tysoc::json::safeParseFloat( jsonElement, "AttachY", 0.0f );
        this->localPos.z = tysoc::json::safeParseFloat( jsonElement, "AttachZ", 0.0f );
        this->localPos = rearrange( this->localPos, worldUp );
        // local orientation from attachTheta x-z
        this->localEuler.x = tysoc::json::safeParseFloat( jsonElement, "AttachThetaX", 0.0f );
        this->localEuler.y = tysoc::json::safeParseFloat( jsonElement, "AttachThetaY", 0.0f );
        this->localEuler.z = tysoc::json::safeParseFloat( jsonElement, "AttachThetaZ", 0.0f );
        this->localEuler = rearrange( this->localEuler, worldUp );
    }

}}