#!/usr/bin/env python

import sys
import loco
import tinymath as tm
import numpy as np

PHYSICS_BACKEND = loco.sim.PHYSICS_NONE
RENDERING_BACKEND = loco.sim.RENDERING_GLVIZ_GLFW

COM_TETRAHEDRON = [ 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 ]

TETRAHEDRON_VERTICES = [ 0.0 - COM_TETRAHEDRON[0], 0.0 - COM_TETRAHEDRON[1], 0.0 - COM_TETRAHEDRON[2],
                         1.0 - COM_TETRAHEDRON[0], 0.0 - COM_TETRAHEDRON[1], 0.0 - COM_TETRAHEDRON[2],
                         0.0 - COM_TETRAHEDRON[0], 1.0 - COM_TETRAHEDRON[1], 0.0 - COM_TETRAHEDRON[2],
                         0.0 - COM_TETRAHEDRON[0], 0.0 - COM_TETRAHEDRON[1], 1.0 - COM_TETRAHEDRON[2] ]
TETRAHEDRON_FACES = [ 0, 1, 3,
                      0, 2, 1,
                      0, 3, 2,
                      1, 2, 3 ]

COM_RAMP = [ 0.0, 7.0 / 9.0, 4.0 / 9.0 ]

RAMP_VERTICES = [  1.0 - COM_RAMP[0], 0.0 - COM_RAMP[1], 0.0 - COM_RAMP[2],
                   1.0 - COM_RAMP[0], 2.0 - COM_RAMP[1], 0.0 - COM_RAMP[2],
                   1.0 - COM_RAMP[0], 1.0 - COM_RAMP[1], 1.0 - COM_RAMP[2],
                   1.0 - COM_RAMP[0], 0.0 - COM_RAMP[1], 1.0 - COM_RAMP[2],
                  -1.0 - COM_RAMP[0], 0.0 - COM_RAMP[1], 0.0 - COM_RAMP[2],
                  -1.0 - COM_RAMP[0], 2.0 - COM_RAMP[1], 0.0 - COM_RAMP[2],
                  -1.0 - COM_RAMP[0], 1.0 - COM_RAMP[1], 1.0 - COM_RAMP[2],
                  -1.0 - COM_RAMP[0], 0.0 - COM_RAMP[1], 1.0 - COM_RAMP[2] ]
RAMP_FACES = [ 0, 1, 2,
               0, 2, 3,
               0, 4, 5,
               0, 5, 1,
               0, 3, 7,
               0, 7, 4,
               2, 6, 7,
               2, 7, 3,
               1, 5, 6,
               1, 6, 2,
               4, 7, 6,
               4, 6, 5 ]

def create_path_part( idx ) :
    height = 1.0
    inner_rad = 2.0
    outer_rad = 3.0
    dtheta = 2.0 * np.pi / 12.0
    ctheta = np.cos( dtheta * idx )
    stheta = np.sin( dtheta * idx )
    ctheta_n = np.cos( dtheta * ( idx + 1 ) )
    stheta_n = np.sin( dtheta * ( idx + 1 ) )

    half_rad = 0.5* ( inner_rad + outer_rad )
    com_position = [ half_rad * np.cos( ( idx + 0.5 ) * dtheta ),
                     half_rad * np.sin( ( idx + 0.5 ) * dtheta ),
                     0.5 * height ]

    vertices = [ inner_rad * ctheta   - com_position[0], inner_rad * stheta   - com_position[1], 0.5 * height,
                 outer_rad * ctheta   - com_position[0], outer_rad * stheta   - com_position[1], 0.5 * height,
                 outer_rad * ctheta_n - com_position[0], outer_rad * stheta_n - com_position[1], 0.5 * height,
                 inner_rad * ctheta_n - com_position[0], inner_rad * stheta_n - com_position[1], 0.5 * height,
                 inner_rad * ctheta   - com_position[0], inner_rad * stheta   - com_position[1], -0.5 * height,
                 outer_rad * ctheta   - com_position[0], outer_rad * stheta   - com_position[1], -0.5 * height,
                 outer_rad * ctheta_n - com_position[0], outer_rad * stheta_n - com_position[1], -0.5 * height,
                 inner_rad * ctheta_n - com_position[0], inner_rad * stheta_n - com_position[1], -0.5 * height ]
    faces = [ 0, 1, 2,
              0, 2, 3,
              0, 4, 5,
              0, 5, 1,
              0, 3, 7,
              0, 7, 4,
              2, 6, 7,
              2, 7, 3,
              1, 5, 6,
              1, 6, 2,
              4, 7, 6,
              4, 6, 5 ]

    return vertices, faces

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

    #### rotation = tm.rotation( tm.Vector3f( [ np.pi / 3, np.pi / 4, np.pi / 6 ] ) )
    #### rotation = tm.rotation( tm.Vector3f( [ np.pi / 2, 0.0, 0.0 ] ) )
    rotation = tm.rotation( tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )

    scenario = loco.sim.Scenario()
    scenario.AddSingleBody( loco.sim.Plane( "floor", 10.0, 10.0, tm.Vector3f(), tm.Matrix3f() ) )
    scenario.AddSingleBody( loco.sim.Sphere( "sphere", 0.1, [ 1.0, -1.0, 2.0 ], rotation ) )
    scenario.AddSingleBody( loco.sim.Mesh( "tetrahedron_0",
                                           TETRAHEDRON_VERTICES,
                                           TETRAHEDRON_FACES,
                                           1.0, [ -1.0, -1.0, 1.0 ], rotation ) )
    scenario.AddSingleBody( loco.sim.Mesh( "tetrahedron_1",
                                           TETRAHEDRON_VERTICES,
                                           TETRAHEDRON_FACES,
                                           0.5, [ -1.0, 1.0, 1.0 ], rotation ) )
    scenario.AddSingleBody( loco.sim.Mesh( "ramp_0",
                                           RAMP_VERTICES,
                                           RAMP_FACES,
                                           0.3, [ 1.0, 1.0, 1.0 ], rotation ) )
    scenario.AddSingleBody( loco.sim.Mesh( "ramp_1",
                                           RAMP_VERTICES,
                                           RAMP_FACES,
                                           0.5, [ 1.0, -1.0, 1.0 ], rotation ) )

    for i in range( 0, 12 ) :
        height = 1.0
        inner_rad = 2.0
        outer_rad = 3.0
        half_rad = 0.5* ( inner_rad + outer_rad )
        dtheta = 2.0 * np.pi / 12.0

        com_position = [ half_rad * np.cos( ( i + 0.5 ) * dtheta ),
                         half_rad * np.sin( ( i + 0.5 ) * dtheta ),
                         0.5 * height ]

        vertices, faces = create_path_part( i )
        scenario.AddSingleBody( loco.sim.Mesh( "path_part_{}".format( i ),
                                               vertices, faces,
                                               1.0, com_position, tm.Matrix3f(),
                                               loco.sim.DynamicsType.STATIC ) )

    runtime = loco.sim.Runtime( PHYSICS_BACKEND, RENDERING_BACKEND )
    simulation = runtime.CreateSimulation( scenario )
    visualizer = runtime.CreateVisualizer( scenario )

    sphere = scenario.GetSingleBodyByName( "sphere" )
    floor = scenario.GetSingleBodyByName( "floor" )
    floor.drawable.texture = 'built_in_chessboard'
    floor.drawable.ambient = [ 0.3, 0.5, 0.7 ]
    floor.drawable.diffuse = [ 0.3, 0.5, 0.7 ]
    floor.drawable.specular = [ 0.3, 0.5, 0.7 ]

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