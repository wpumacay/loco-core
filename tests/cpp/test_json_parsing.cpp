
#include <string>
#include <utils/parsers/parsers_json.h>


static nlohmann::json SAMPLE_JSON = {
    { "float", 1.2345 },
    { "int", 2 },
    { "vec3", { 1.1, 2.3, 4.6 } },
    { "vec4", { 1.0, 2.0, 3.0, 4.0 } },
    { "string", "foobarfun" }
};

bool test_safeParseInt()
{
    bool _error = true;

    // parsing an int should return the int
    _error = ( tysoc::json::safeParseInt( SAMPLE_JSON, "int" ) == 2 );
    // parsing a float should return the number casted
    _error = ( tysoc::json::safeParseInt( SAMPLE_JSON, "float" ) == (int) 1.2345 ) && _error;
    // _parsing a string should return default
    _error = ( tysoc::json::safeParseInt( SAMPLE_JSON, "string", 123 ) == 123 ) && _error;
    // _parsing a vec3 or vec4 should also return default
    _error = ( tysoc::json::safeParseInt( SAMPLE_JSON, "vec3", 123 ) == 123 ) && _error;
    _error = ( tysoc::json::safeParseInt( SAMPLE_JSON, "vec4", 123 ) == 123 ) && _error;

    return _error;
}

bool test_safeParseFloat()
{
    bool _error = true;

    // parsing a float should return the float
    _error = ( tysoc::json::safeParseFloat( SAMPLE_JSON, "float" ) == 1.2345f );
    // parsing an int should return the number casted
    _error = ( tysoc::json::safeParseFloat( SAMPLE_JSON, "int" ) == 2.0f ) && _error;
    // _parsing a string should return default
    _error = ( tysoc::json::safeParseFloat( SAMPLE_JSON, "string", 10.25f ) == 10.25f ) && _error;
    // _parsing a vec3 or vec4 should also return default
    _error = ( tysoc::json::safeParseFloat( SAMPLE_JSON, "vec3", 10.25f ) == 10.25f ) && _error;
    _error = ( tysoc::json::safeParseFloat( SAMPLE_JSON, "vec4", 10.25f ) == 10.25f ) && _error;

    return _error;
}

bool test_safeParseString()
{
    bool _error = true;

    // parsing a string should return the string
    _error = ( tysoc::json::safeParseString( SAMPLE_JSON, "string" ) == "foobarfun" );
    // parsing an everything else should return default
    _error = ( tysoc::json::safeParseString( SAMPLE_JSON, "int", "default" ) == "default" ) && _error;
    _error = ( tysoc::json::safeParseString( SAMPLE_JSON, "float", "default" ) == "default" ) && _error;
    _error = ( tysoc::json::safeParseString( SAMPLE_JSON, "vec3", "default" ) == "default" ) && _error;
    _error = ( tysoc::json::safeParseString( SAMPLE_JSON, "vec4", "default" ) == "default" ) && _error;

    return _error;
}

bool test_safeParseVec3()
{
    bool _error = true;

    // parsing a vec3 should return the vec3
    _error = ( tysoc::json::safeParseVec3( SAMPLE_JSON, "vec3" ) == tysoc::TVec3( 1.1f, 2.3f, 4.6f ) );
    // parsing an everything else should return default
    tysoc::TVec3 _default( 1.0f, 2.0f, 3.0f );
    _error = ( tysoc::json::safeParseVec3( SAMPLE_JSON, "int", _default ) == _default ) && _error;
    _error = ( tysoc::json::safeParseVec3( SAMPLE_JSON, "float", _default ) == _default ) && _error;
    _error = ( tysoc::json::safeParseVec3( SAMPLE_JSON, "string", _default ) == _default ) && _error;
    _error = ( tysoc::json::safeParseVec3( SAMPLE_JSON, "vec4", _default ) == _default ) && _error;

    return _error;
}

bool test_safeParseVec4()
{
    bool _error = true;

    // parsing a vec4 should return the vec4
    _error = ( tysoc::json::safeParseVec4( SAMPLE_JSON, "vec4" ) == tysoc::TVec4( 1.0f, 2.0f, 3.0f, 4.0f ) );
    // parsing an everything else should return default
    tysoc::TVec4 _default( 11.1f, 22.2f, 33.3f, 44.4f );
    _error = ( tysoc::json::safeParseVec4( SAMPLE_JSON, "int", _default ) == _default ) && _error;
    _error = ( tysoc::json::safeParseVec4( SAMPLE_JSON, "float", _default ) == _default ) && _error;
    _error = ( tysoc::json::safeParseVec4( SAMPLE_JSON, "vec3", _default ) == _default ) && _error;
    _error = ( tysoc::json::safeParseVec4( SAMPLE_JSON, "string", _default ) == _default ) && _error;

    return _error;
}

int main()
{
    int _error = 0;

    _error += ( test_safeParseInt() ? 0 : 1 << 0 );
    _error += ( test_safeParseFloat() ? 0 : 1 << 1 );
    _error += ( test_safeParseString() ? 0 : 1 << 2 );
    _error += ( test_safeParseVec3() ? 0 : 1 << 3 );
    _error += ( test_safeParseVec4() ? 0 : 1 << 4 );

    if ( _error > 0 )
        std::cout << "TEST> FAILED. Code: " << _error << std::endl;
    else
        std::cout << "TEST> PASSED" << std::endl;

    return _error;
}
