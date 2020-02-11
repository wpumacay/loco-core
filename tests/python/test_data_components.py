#!/usr/bin/env python

import loco
import tinymath as tm
import numpy as np
import gc

def test_core_components_data() :
    col = loco.sim.CollisionData()
    col.type = loco.sim.ShapeType.SPHERE
    col.size = [ 0.1, 0.1, 0.1 ]
    col.collisionGroup = 1
    col.collisionMask = 2
    col.friction = [ 1., 0.01, 0.001 ]
    col.density = 900.
    col.localTf = tm.Matrix4f( np.identity( 3 ), np.array( [ 0.1, -0.1, 0.2 ] ) )
    assert col.type == loco.sim.ShapeType.SPHERE, "test_core_components_data >>> couldn't set collider-param 'type'"
    assert np.allclose( col.size, [ 0.1, 0.1, 0.1 ] ), "test_core_components_data >>> couldn't set collider-param 'size'"
    assert col.collisionGroup == 1, "test_core_components_data >>> couldn't set collider-param 'collisionGroup'"
    assert col.collisionMask == 2, "test_core_components_data >>> couldn't set collider-param 'collisionMask'"
    assert np.allclose( col.friction, [ 1., 0.01, 0.001 ] ) , "test_core_components_data >>> couldn't set collider-param 'friction'"
    assert np.allclose( col.density, 900. ), "test_core_components_data >>> couldn't set collider-param 'density'"
    assert np.allclose( col.localTf, tm.Matrix4f( np.identity( 3 ), [ 0.1, -0.1, 0.2 ] ) ), "test_core_components_data >>> couldn't set collider-param 'localTf'"
    #### print( 'collision-data: \n{}'.format( col ) )
    print( 'num_referrers(col): {}'.format( len( gc.get_referrers( col ) ) ) )
    print( 'id(col)', hex( id( col ) ) )

    vis = loco.sim.VisualData()
    vis.type = loco.sim.ShapeType.SPHERE
    vis.size = [ 0.1, 0.1, 0.1 ]
    vis.ambient = [ 0.8, 0.4, 0.2 ]
    vis.diffuse = [ 0.7, 0.3, 0.1 ]
    vis.specular = [ 0.75, 0.35, 0.15 ]
    vis.shininess = 64.
    vis.texture = 'built_in_chessboard'
    vis.localTf = tm.Matrix4f( np.identity( 3 ), np.array( [ 0.1, -0.1, 0.2 ] ) )
    assert vis.type == loco.sim.ShapeType.SPHERE, "test_core_components_data >>> couldn't set visual-param 'type'"
    assert np.allclose( vis.size, [ 0.1, 0.1, 0.1 ] ), "test_core_components_data >>> couldn't set visual-param 'size'"
    assert np.allclose( vis.ambient, [ 0.8, 0.4, 0.2 ] ), "test_core_components_data >>> couldn't set visual-param 'ambient'"
    assert np.allclose( vis.diffuse, [ 0.7, 0.3, 0.1 ] ), "test_core_components_data >>> couldn't set visual-param 'diffuse'"
    assert np.allclose( vis.specular, [ 0.75, 0.35, 0.15 ] ), "test_core_components_data >>> couldn't set visual-param 'specular'"
    assert np.allclose( vis.shininess, 64. ), "test_core_components_data >>> couldn't set visual-param 'shininess'"
    assert vis.texture == 'built_in_chessboard', "test_core_components_data >>> couldn't set visual-param 'texture'"
    assert np.allclose( vis.localTf, tm.Matrix4f( np.identity( 3 ), [ 0.1, -0.1, 0.2 ] ) ), "test_core_components_data >>> couldn't set visual-param 'localTf'"
    #### print( 'visual-data: \n{}'.format( vis ) )
    print( 'num_referrers(vis): {}'.format( len( gc.get_referrers( vis ) ) ) )
    print( 'id(vis)', hex( id( vis ) ) )

    jnt = loco.sim.JointData()
    jnt.type = loco.sim.JointType.REVOLUTE
    jnt.axis = [ 1., 0., 0. ]
    jnt.limits = [ -np.pi / 2., np.pi / 2. ]
    jnt.stiffness = 2.0
    jnt.armature = 0.5
    jnt.damping = 1.0
    jnt.ref = np.pi / 10.
    jnt.localTf = tm.Matrix4f( tm.rotation( tm.Vector3f( [ np.pi / 4., np.pi / 5., np.pi / 6. ] ) ), tm.Vector3f( [ 0.1, 0.2, -0.3 ] ) )
    assert jnt.type == loco.sim.JointType.REVOLUTE, "test_core_components_data >>> couldn't set joint-param 'type'"
    assert np.allclose( jnt.axis, [ 1., 0., 0. ] ), "test_core_components_data >>> couldn't set joint-param 'axis'"
    assert np.allclose( jnt.limits, [ -np.pi / 2., np.pi / 2. ] ), "test_core_components_data >>> couldn't set joint-param 'limits'"
    assert np.allclose( jnt.stiffness, 2.0 ), "test_core_components_data >>> couldn't set joint-param 'stiffness'"
    assert np.allclose( jnt.armature, 0.5 ), "test_core_components_data >>> couldn't set joint-param 'armature'"
    assert np.allclose( jnt.damping, 1.0 ), "test_core_components_data >>> couldn't set joint-param 'damping'"
    assert np.allclose( jnt.ref, np.pi / 10. ), "test_core_components_data >>> couldn't set joint-param 'ref'"
    assert np.allclose( jnt.localTf, tm.Matrix4f( tm.rotation( tm.Vector3f( [ np.pi / 4., np.pi / 5., np.pi / 6. ] ) ),
                                                  tm.Vector3f( [ 0.1, 0.2, -0.3 ] ) ) ), "test_core_components_data >>> couldn't set joint-param 'localTf'"
    #### print( 'jnt.nqpos : {}'.format( jnt.nqpos ) )
    #### print( 'jnt.nqvel : {}'.format( jnt.nqvel ) )
    #### print( 'joint-data: \n{}'.format( jnt ) )

    act = loco.sim.ActuatorData()
    act.type = loco.sim.ActuatorType.PD_CONTROLLER
    act.limits = [ -np.pi / 3., np.pi / 3. ]
    act.gear = [ 150. ]
    act.kp = 100.
    act.kv = 10.
    act.localTf = tm.Matrix4f( tm.rotation( tm.Vector3f( [ np.pi / 4., np.pi / 5., np.pi / 6. ] ) ), tm.Vector3f( [ 0.1, 0.2, -0.3 ] ) )
    assert act.type == loco.sim.ActuatorType.PD_CONTROLLER, "test_core_components_data >>> couldn't set actuator-param 'type'"
    assert np.allclose( act.limits, [ -np.pi / 3., np.pi / 3. ] ), "test_core_components_data >>> couldn't set actuator-param 'limits'"
    assert np.allclose( act.gear, [ 150. ] ), "test_core_components_data >>> couldn't set actuator-param 'gear'"
    assert np.allclose( act.kp, 100. ), "test_core_components_data >>> couldn't set actuator-param 'kp'"
    assert np.allclose( act.kv, 10. ), "test_core_components_data >>> couldn't set actuator-param 'kv'"
    #### print( 'actuator-data: \n{}'.format( act ) )

    sen = loco.sim.SensorData()
    sen.type = loco.sim.SensorType.EXT_CAMERA_RGB
    sen.localTf = tm.Matrix4f( np.identity( 3 ), [ 0.0, 0.1, 0.2 ] )
    assert sen.type == loco.sim.SensorType.EXT_CAMERA_RGB, "test_core_components_data >>> couldn't set sensor-param 'type'"
    assert np.allclose( sen.localTf, tm.Matrix4f( np.identity( 3 ), [ 0.0, 0.1, 0.2 ] ) ), "test_core_components_data >>> couldn't set sensor-param 'localTf'"
    #### print( 'sensor-data: \n{}'.format( sen ) )

    inertia = loco.sim.InertialData()
    inertia.mass = 10.0
    inertia.ixx = 0.1
    inertia.iyy = 0.2
    inertia.izz = 0.1
    inertia.ixy = 0.001
    inertia.ixz = 0.002
    inertia.iyz = 0.003
    inertia.localTf = tm.Matrix4f( np.identity( 3 ), [ -0.0, -0.1, -0.2 ] )
    assert np.allclose( inertia.mass, 10.0 ), "test_core_components_data >>> couldn't set inertia-param 'mass'"
    assert np.allclose( inertia.ixx, 0.1 ), "test_core_components_data >>> couldn't set inertia-param 'ixx'"
    assert np.allclose( inertia.iyy, 0.2 ), "test_core_components_data >>> couldn't set inertia-param 'iyy'"
    assert np.allclose( inertia.izz, 0.1 ), "test_core_components_data >>> couldn't set inertia-param 'izz'"
    assert np.allclose( inertia.ixy, 0.001 ), "test_core_components_data >>> couldn't set inertia-param 'ixy'"
    assert np.allclose( inertia.ixz, 0.002 ), "test_core_components_data >>> couldn't set inertia-param 'ixz'"
    assert np.allclose( inertia.iyz, 0.003 ), "test_core_components_data >>> couldn't set inertia-param 'iyz'"
    assert np.allclose( inertia.localTf, tm.Matrix4f( np.identity( 3 ), [ -0.0, -0.1, -0.2 ] ) ), "test_core_components_data >>> couldn't set inertia-param 'localTf'"
    #### print( 'inertia-data: \n{}'.format( inertia ) )
    print( 'num_referrers(inertia): {}'.format( len( gc.get_referrers( inertia ) ) ) )
    print( 'id(inertia)', hex( id( inertia ) ) )

    body = loco.sim.BodyData()
    body.dyntype = loco.sim.DynamicsType.DYNAMIC
    body.inertialData = inertia
    body.localTf0 = tm.Matrix4f( np.identity( 3 ), [ -0.01, 0.02, -0.03 ] )
    body.collision = col
    body.visual = vis
    assert body.dyntype == loco.sim.DynamicsType.DYNAMIC, "test_core_components_data >>> couldn't set body-param 'dyntype'"
    assert np.allclose( body.inertialData.mass, 10.0 ), "test_core_components_data >>> couldn't set body-param 'inertia.mass'"
    assert np.allclose( body.inertialData.ixx, 0.1 ), "test_core_components_data >>> couldn't set body-param 'inertia.ixx'"
    assert np.allclose( body.inertialData.iyy, 0.2 ), "test_core_components_data >>> couldn't set body-param 'inertia.iyy'"
    assert np.allclose( body.inertialData.izz, 0.1 ), "test_core_components_data >>> couldn't set body-param 'inertia.izz'"
    assert np.allclose( body.inertialData.ixy, 0.001 ), "test_core_components_data >>> couldn't set body-param 'inertia.ixy'"
    assert np.allclose( body.inertialData.ixz, 0.002 ), "test_core_components_data >>> couldn't set body-param 'inertia.ixz'"
    assert np.allclose( body.inertialData.iyz, 0.003 ), "test_core_components_data >>> couldn't set body-param 'inertia.iyz'"
    assert np.allclose( body.localTf0, tm.Matrix4f( np.identity( 3 ), [ -0.01, 0.02, -0.03 ] ) ), "test_core_components_data >>> couldn't set body-param 'localTf0'"
    assert np.allclose( body.collision.size, [ 0.1, 0.1, 0.1 ] ), "test_core_components_data >>> couldn't set body-param 'col.size'"
    assert np.allclose( body.collision.friction, [ 1.0, 0.01, 0.001 ] ), "test_core_components_data >>> couldn't set body-param 'col.friction'"
    assert np.allclose( body.collision.density, 900. ), "test_core_components_data >>> couldn't set body-param 'col.density'"
    assert np.allclose( body.visual.size, [ 0.1, 0.1, 0.1 ] ), "test_core_components_data >>> couldn't set body-param 'vis.size'"
    assert np.allclose( body.visual.ambient, [ 0.8, 0.4, 0.2 ] ), "test_core_components_data >>> couldn't set body-param 'vis.ambient'"
    assert np.allclose( body.visual.diffuse, [ 0.7, 0.3, 0.1 ] ), "test_core_components_data >>> couldn't set body-param 'vis.diffuse'"
    assert np.allclose( body.visual.specular, [ 0.75, 0.35, 0.15 ] ), "test_core_components_data >>> couldn't set body-param 'vis.specular'"
    assert np.allclose( body.visual.shininess, 64. ), "test_core_components_data >>> couldn't set body-param 'vis.shininess'"
    #### print( 'body-data: \n{}'.format( body ) )
    # Get number of referrers to component objects after setting components (seems doesn't go up for readwrite params)
    print( 'num_referrers(col): {}'.format( len( gc.get_referrers( col ) ) ) )
    print( 'num_referrers(vis): {}'.format( len( gc.get_referrers( vis ) ) ) )
    print( 'num_referrers(inertia): {}'.format( len( gc.get_referrers( inertia ) ) ) )
    print( 'num_referrers(body.col): {}'.format( len( gc.get_referrers( body.collision ) ) ) )
    print( 'num_referrers(body.vis): {}'.format( len( gc.get_referrers( body.visual ) ) ) )
    print( 'num_referrers(body.inertia): {}'.format( len( gc.get_referrers( body.inertialData ) ) ) )

    ref_col = body.collision
    ref_vis = body.visual
    ref_int = body.inertialData

    # Get number of referrers to component objects after getting references (should stay the same py_reference)
    print( 'num_referrers(col): {}'.format( len( gc.get_referrers( col ) ) ) )
    print( 'num_referrers(vis): {}'.format( len( gc.get_referrers( vis ) ) ) )
    print( 'num_referrers(inertia): {}'.format( len( gc.get_referrers( inertia ) ) ) )
    print( 'num_referrers(body.col): {}'.format( len( gc.get_referrers( body.collision ) ) ) )
    print( 'num_referrers(body.vis): {}'.format( len( gc.get_referrers( body.visual ) ) ) )
    print( 'num_referrers(body.inertia): {}'.format( len( gc.get_referrers( body.inertialData ) ) ) )
    print( 'num_referrers(body): {}'.format( len( gc.get_referrers( body ) ) ) )

if __name__ == '__main__' :
    _ = input( 'Press ENTER to start test : test_core_components_data' )
    test_core_components_data()

    _ = input( 'Press ENTER to continue ...' )
