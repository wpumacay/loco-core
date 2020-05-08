#!/usr/bin/env python

import sys
import loco
import tinymath as tm
import tinyutils as tu
import numpy as np

PHYSICS_BACKEND = loco.sim.PHYSICS_NONE
RENDERING_BACKEND = loco.sim.RENDERING_GLVIZ_GLFW

nx_samples_hfield_1 = 25
ny_samples_hfield_1 = 25
x_extent_hfield_1 = 10.0
y_extent_hfield_1 = 10.0
def create_hfield_paraboloid() :
    x = np.linspace( -0.5 * x_extent_hfield_1, 0.5 * x_extent_hfield_1, nx_samples_hfield_1, endpoint=False )
    y = np.linspace( -0.5 * y_extent_hfield_1, 0.5 * y_extent_hfield_1, ny_samples_hfield_1, endpoint=False )
    xx, yy = np.meshgrid( x, y )
    zz = 10.0 * ( xx ** 2 + yy ** 2 ) / ( x_extent_hfield_1 ** 2 + y_extent_hfield_1 ** 2 )
    return zz.ravel()

nx_samples_hfield_2 = 25
ny_samples_hfield_2 = 25
x_extent_hfield_2 = 10.0
y_extent_hfield_2 = 10.0
hfield_octaves = 4
hfield_persistance = 0.5
hfield_lacunarity = 2.0
hfield_noise_scale = 5.0
def create_hfield_perlin_noise() :
    tu.PerlinNoise.Config( hfield_octaves, hfield_persistance, hfield_lacunarity, hfield_noise_scale )

    x = np.linspace( -0.5 * x_extent_hfield_2, 0.5 * x_extent_hfield_2, nx_samples_hfield_2, endpoint=False )
    y = np.linspace( -0.5 * y_extent_hfield_2, 0.5 * y_extent_hfield_2, ny_samples_hfield_2, endpoint=False )
    xx, yy = np.meshgrid( x, y )
    zz = np.zeros_like( xx )
    for j in range( 0, nx_samples_hfield_2 ) :
        for i in range( 0, ny_samples_hfield_2 ) :
            zz[i,j] = max( 0.0, tu.PerlinNoise.Sample2d( xx[i,j], yy[i,j] ) )
    return zz.ravel()


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

    rotation = tm.rotation( tm.Vector3f( [ np.pi / 3, np.pi / 4, np.pi / 6 ] ) )
    #### rotation = tm.rotation( tm.Vector3f( [ np.pi / 2, 0.0, 0.0 ] ) )
    #### rotation = tm.rotation( tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )

    scenario = loco.sim.Scenario()
    floor = scenario.AddSingleBody( loco.sim.Plane( "floor", 40.0, 40.0, tm.Vector3f( [ 0.0, 0.0, -0.001 ] ), tm.Matrix3f() ) )
    floor.drawable.ambient = [ 0.3, 0.5, 0.7 ]
    floor.drawable.diffuse = [ 0.3, 0.5, 0.7 ]
    floor.drawable.specular = [ 0.3, 0.5, 0.7 ]

    sphere = scenario.AddSingleBody( loco.sim.Sphere( "sphere", 0.1, [ -1.0, -1.0, 3.0 ], tm.Matrix3f() ) )
    hfield_1 = scenario.AddSingleBody( loco.sim.Heightfield( "hfield_1",
                                                             nx_samples_hfield_1, ny_samples_hfield_1,
                                                             x_extent_hfield_1, y_extent_hfield_1,
                                                             create_hfield_paraboloid(),
                                                             [ -5.0, -5.0, 0.0 ], tm.Matrix3f() ) )
    hfield_1.drawable.ambient = [ 0.5, 0.5, 0.5 ]
    hfield_1.drawable.diffuse = [ 0.5, 0.5, 0.5 ]
    hfield_1.drawable.specular = [ 0.5, 0.5, 0.5 ]
    hfield_2 = scenario.AddSingleBody( loco.sim.Heightfield( "hfield_2",
                                                             nx_samples_hfield_2, ny_samples_hfield_2,
                                                             x_extent_hfield_2, y_extent_hfield_2,
                                                             create_hfield_perlin_noise(),
                                                             [ 5.0, 5.0, 0.0 ], tm.Matrix3f() ) )
    hfield_2.drawable.ambient = [ 0.5, 0.5, 0.5 ]
    hfield_2.drawable.diffuse = [ 0.5, 0.5, 0.5 ]
    hfield_2.drawable.specular = [ 0.5, 0.5, 0.5 ]

    runtime = loco.sim.Runtime( PHYSICS_BACKEND, RENDERING_BACKEND )
    simulation = runtime.CreateSimulation( scenario )
    visualizer = runtime.CreateVisualizer( scenario )

    floor.drawable.texture = 'built_in_chessboard'
    hfield_1.drawable.texture = 'built_in_chessboard'
    hfield_2.drawable.texture = 'built_in_chessboard'

    while visualizer.IsActive() :
        if visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_ESCAPE ) :
            break
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_R ) :
            simulation.Reset()
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_P ) :
            simulation.Pause() if simulation.running else simulation.Resume()
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_SPACE ) :
            sphere.AddForceCOM( [ 0.0, 0.0, 1000.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_UP ) :
            sphere.AddForceCOM( [ 0.0, 200.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_DOWN ) :
            sphere.AddForceCOM( [ 0.0, -200.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_RIGHT ) :
            sphere.AddForceCOM( [ 200.0, 0.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_LEFT ) :
            sphere.AddForceCOM( [ -200.0, 0.0, 0.0 ] )

        simulation.Step()
        visualizer.Update()

    runtime.DestroySimulation()
    runtime.DestroyVisualizer()