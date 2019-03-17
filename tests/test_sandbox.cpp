
#include <runtime.h>

int main()
{

    auto _bbox = new tysoc::sandbox::TBody();
    _bbox->name = "bbox";
    _bbox->type = "box";
    _bbox->size = { 0.2, 0.2, 0.4 };
    _bbox->worldTransform.setPosition( { 1.0, 1.0, 1.0 } );

    auto _bsphere = new tysoc::sandbox::TBody();
    _bsphere->name = "bsphere";
    _bsphere->type = "sphere";
    _bsphere->size = { 0.2, 0.2, 0.2 };
    _bsphere->worldTransform.setPosition( { 1.0, -1.0, 1.0 } );

    auto _bcapsule = new tysoc::sandbox::TBody();
    _bcapsule->name = "bcapsule";
    _bcapsule->type = "capsule";
    _bcapsule->size = { 0.1, 0.4, 0.0 };
    _bcapsule->worldTransform.setPosition( { -1.0, 1.0, 1.0 } );

    auto _bcylinder = new tysoc::sandbox::TBody();
    _bcylinder->name = "bcylinder";
    _bcylinder->type = "cylinder";
    _bcylinder->size = { 0.1, 0.4, 0.0 };
    _bcylinder->worldTransform.setPosition( { -1.0, -1.0, 1.0 } );

    auto _scenario = new tysoc::TScenario();
    _scenario->addBody( _bbox );
    _scenario->addBody( _bsphere );
    _scenario->addBody( _bcapsule );
    _scenario->addBody( _bcylinder );

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