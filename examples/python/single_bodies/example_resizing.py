#!/usr/bin/env python

import sys
import loco
import tinymath as tm
import numpy as np

PHYSICS_BACKEND = loco.sim.PHYSICS_NONE
RENDERING_BACKEND = loco.sim.RENDERING_GLVIZ_GLFW

if __name__ == '__main__' :

    if len( sys.argv ) > 1 :
        choice_backend = sys.argv[1]
        if choice_backend == 'mujoco' :
            PHYSICS_BACKEND = loco.sim.PHYSICS_MUJOCO
        elif choice_backend == 'bullet' :
            PHYSICS_BACKEND = loco.sim.PHYSICS_BULLET
        elif choice_backend == 'dart' :
            PHYSICS_BACKEND = loco.sim.PHYSICS_DART
        elif choice_backend == 'raisim' :
            PHYSICS_BACKEND = loco.sim.PHYSICS_RAISIM
    print( 'Physics backend: {}'.format( PHYSICS_BACKEND ) )
    print( 'Rendering backend: {}'.format( RENDERING_BACKEND ) )

    pos_box = [ -3.0, 0.0, 1.0 ]
    pos_sphere = [ -2.0, 0.0, 1.0 ]
    pos_cylinder = [ -1.0, 0.0, 1.0 ]
    pos_capsule = [ 1.0, 0.0, 1.0 ]
    pos_ellipsoid = [ 2.0, 0.0, 1.0 ]
    pos_mesh = [ 3.0, 0.0, 1.0 ]

    scenario = loco.sim.Scenario()
    floor = scenario.AddSingleBody( loco.sim.Plane( "floor", 10.0, 10.0, np.zeros(3), np.identity(3) ) )
    box = scenario.AddSingleBody( loco.sim.Box( "box", [ 0.2, 0.3, 0.4 ], pos_box, np.identity(3) ) )
    sphere = scenario.AddSingleBody( loco.sim.Sphere( "sphere", 0.1, pos_sphere, np.identity(3) ) )
    cylinder = scenario.AddSingleBody( loco.sim.Cylinder( "cylinder", 0.2, 0.5, pos_cylinder, np.identity(3) ) )
    capsule = scenario.AddSingleBody( loco.sim.Capsule( "capsule", 0.2, 0.5, pos_capsule, np.identity(3) ) )
    ellipsoid = scenario.AddSingleBody( loco.sim.Ellipsoid( "ellipsoid", [ 0.2, 0.3, 0.4 ], pos_ellipsoid, np.identity(3) ) )
    mesh = scenario.AddSingleBody( loco.sim.Mesh( "monkey",
                                                  loco.sim.PATH_RESOURCES + 'meshes/monkey.stl',
                                                  loco.sim.PATH_RESOURCES + 'meshes/monkey.obj',
                                                  0.2, pos_mesh, np.identity(3) ) )

    user_sphere = scenario.AddSingleBody( loco.sim.Sphere( 'user_sphere', 0.1, [0.0, 0.0, 1.0], np.identity(3) ) )

    runtime = loco.sim.Runtime( PHYSICS_BACKEND, RENDERING_BACKEND )
    simulation = runtime.CreateSimulation( scenario )
    visualizer = runtime.CreateVisualizer( scenario )

    floor.drawable.texture = 'built_in_chessboard'
    floor.drawable.ambient = [ 0.3, 0.5, 0.7 ]
    floor.drawable.diffuse = [ 0.3, 0.5, 0.7 ]
    floor.drawable.specular = [ 0.3, 0.5, 0.7 ]
    user_sphere.drawable.texture = 'built_in_chessboard'
    user_sphere.drawable.ambient = [0.1, 0.1, 0.8]
    user_sphere.drawable.diffuse = [0.1, 0.1, 0.8]
    user_sphere.drawable.specular = [0.1, 0.1, 0.8]

    increasing = True

    while visualizer.IsActive() :
        if visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_ESCAPE ) :
            break
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_R ) :
            simulation.Reset()
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_P ) :
            simulation.Pause() if simulation.running else simulation.Resume()
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_SPACE ) :
            user_sphere.AddForceCOM( [ 0.0, 0.0, 1000.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_UP ) :
            user_sphere.AddForceCOM( [ 0.0, 200.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_DOWN ) :
            user_sphere.AddForceCOM( [ 0.0, -200.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_RIGHT ) :
            user_sphere.AddForceCOM( [ 200.0, 0.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_LEFT ) :
            user_sphere.AddForceCOM( [ -200.0, 0.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_0 ) :
            increasing = not increasing
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_1 ) :
            box.width = max( 0.01, box.width + ( 0.1 if increasing else -0.1 ) )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_2 ) :
            box.depth = max( 0.01, box.depth + ( 0.1 if increasing else -0.1 ) )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_3 ) :
            box.height = max( 0.01, box.height + ( 0.1 if increasing else -0.1 ) )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_4 ) :
            sphere.radius = max( 0.01, sphere.radius + ( 0.1 if increasing else -0.1 ) )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_5 ) :
            cylinder.radius = max( 0.01, cylinder.radius + ( 0.1 if increasing else -0.1 ) )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_6 ) :
            cylinder.height = max( 0.01, cylinder.height + ( 0.1 if increasing else -0.1 ) )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_7 ) :
            capsule.radius = max( 0.01, capsule.radius + ( 0.1 if increasing else -0.1 ) )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_8 ) :
            capsule.height = max( 0.01, capsule.height + ( 0.1 if increasing else -0.1 ) )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_9 ) :
            mesh.scale = max( 0.01, mesh.scale + ( 0.1 if increasing else -0.1 ) )

        simulation.Step(1. / 60.)
        visualizer.Render(loco.sim.RenderMode.NORMAL)

    runtime.DestroySimulation()
    runtime.DestroyVisualizer()