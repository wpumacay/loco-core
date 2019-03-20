
#include <runtime.h>

void convertFromto( const tysoc::TVec3& from,
                    const tysoc::TVec3& to,
                    const float& radius,
                    tysoc::TVec3& pos,
                    tysoc::TMat3& rot,
                    tysoc::TVec3& size )
{
    // compute length of cylinder/capsule
    auto _diff = tysoc::TVec3( to.x - from.x,
                               to.y - from.y,
                               to.z - from.z );
    auto _length = tysoc::TVec3::length( _diff );

    // compute the position of the cylinder/capsule
    pos.x = 0.5 * ( from.x + to.x );
    pos.y = 0.5 * ( from.y + to.y );
    pos.z = 0.5 * ( from.z + to.z );

    // compute the rotation mat of the cylinder/capsule
    auto _zAxisWorld = tysoc::TVec3( 0, 0, 1 );
    auto _zAxisLocal = tysoc::TVec3( _diff.x / _length, _diff.y / _length, _diff.z / _length );
    auto _quat = tysoc::shortestArcQuat( _zAxisWorld, _zAxisLocal );
    rot = tysoc::TMat3::fromQuaternion( _quat );

    // and finally the size
    size.x = radius;
    size.y = _length;
}

tysoc::sandbox::TBody* buildGlobalCapsule( const std::string& name,
                                           const tysoc::TVec3& from,
                                           const tysoc::TVec3& to,
                                           const float& radius,
                                           const tysoc::TVec3& bodypos )
{
    tysoc::TVec3 _pos;
    tysoc::TMat3 _rot;
    tysoc::TVec3 _size;

    convertFromto( from, to, radius,
                   _pos, _rot, _size );

    // For global coordinates it seems it should not use the body offset
    // _pos.x += bodypos.x;
    // _pos.y += bodypos.y;
    // _pos.z += bodypos.z;

    auto _body = new tysoc::sandbox::TBody();
    _body->name = name;
    _body->type = "capsule";
    _body->size = _size;
    _body->worldTransform.setPosition( _pos );
    _body->worldTransform.setRotation( _rot );

    return _body;
}

void drawJointsAxis( const std::vector< tysoc::TVec3 >& jointspos,
                     const std::vector< tysoc::TVec3 >& jointsaxis )
{
    for ( size_t q = 0; q < jointspos.size(); q++ )
    {
        
    }
}

int main()
{
    tysoc::TVec3 _pos;
    tysoc::TMat3 _rot;
    tysoc::TVec3 _size;

    // Bodies from walker2d - checking if global is what I think it means ******

    auto _btorso = buildGlobalCapsule( "torso", { 0, 0, 1.45 }, { 0, 0, 1.05 }, 0.05, { 0, 0, 1.25 } );

    auto _brthigh = buildGlobalCapsule( "rthigh", { 0, 0, 1.05 }, { 0, 0, 0.6 }, 0.05, { 0, 0, 1.05 } );
    auto _brleg = buildGlobalCapsule( "rleg", { 0, 0, 0.6 }, { 0, 0, 0.1 }, 0.04, { 0, 0, 0.35 } );
    auto _brfoot = buildGlobalCapsule( "rfoot", { 0, 0, 0.1 }, { 0.2, 0, 0.1 }, 0.06, { 0.1, 0, 0.1 } );

    auto _blthigh = buildGlobalCapsule( "lthigh", { 0, 0, 1.05 }, { 0, 0, 0.6 }, 0.05, { 0, 0, 1.05 } );
    auto _blleg = buildGlobalCapsule( "lleg", { 0, 0, 0.6 }, { 0, 0, 0.1 }, 0.04, { 0, 0, 0.35 } );
    auto _blfoot = buildGlobalCapsule( "lfoot", { 0, 0, 0.1 }, { 0.2, 0, 0.1 }, 0.06, { 0.1, 0, 0.1 } );

    // *************************************************************************

    auto _scenario = new tysoc::TScenario();
    _scenario->addBody( _btorso );
    _scenario->addBody( _brthigh );
    _scenario->addBody( _brleg );
    _scenario->addBody( _brfoot );
    _scenario->addBody( _blthigh );
    _scenario->addBody( _blleg );
    _scenario->addBody( _blfoot );

    auto _runtime = new tysoc::TRuntime( "",
                                         tysoc::config::rendering::GLVIZ );

    auto _visualizer = _runtime->createVisualizer( _scenario );
    _visualizer->initialize();

    while ( _visualizer->isActive() )
    {
        _visualizer->update();
    }

    return 0;
}