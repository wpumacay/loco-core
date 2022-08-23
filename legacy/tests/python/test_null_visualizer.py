#!/usr/bin/env python

import loco
import tinymath as tm
import numpy as np
import gc

def test_null_visualizer_functionality() :
    vis_data = loco.sim.VisualData()
    vis_data.type = loco.sim.ShapeType.CAPSULE
    vis_data.size = [ 0.1, 0.2, 0.1 ]
    col_data = loco.sim.CollisionData()
    col_data.type = loco.sim.ShapeType.CAPSULE
    col_data.size = [ 0.1, 0.2, 0.1 ]

    body_data = loco.sim.BodyData()
    body_data.dyntype = loco.sim.DynamicsType.DYNAMIC
    body_data.collision = col_data
    body_data.visual = vis_data

    body_obj = loco.sim.SingleBody( 'body_0', body_data, [ 1.0, 1.0, 1.0 ], np.identity( 3 ) )
    scenario = loco.sim.Scenario()
    scenario.AddSingleBody( body_obj )

    visualizer = loco.sim.NullVisualizer( scenario )
    assert ( visualizer.GetCameraByName( 'cam_orbit_0' ) == None )
    assert ( visualizer.GetLightByName( 'light_point_0' ) == None )

    camera = visualizer.CreateCamera( 'cam_orbit_0',
                                      loco.sim.VizCameraType.ORBIT,
                                      [ 3.0, 3.0, 3.0 ],
                                      [ 0.0, 0.0, 0.0 ] )
    light = visualizer.CreateLight( 'light_point_0',
                                    loco.sim.VizLightType.POINT,
                                    [ 0.4, 0.4, 0.4 ],
                                    [ 0.8, 0.8, 0.8 ],
                                    [ 0.8, 0.8, 0.8 ] )
    visualizer.Initialize()
    visualizer.Update()
    visualizer.Reset()

    camera.position = [ 5.0, 5.0, 5.0 ]
    camera.target = [ 0.0, 0.0, 1.0 ]

    light.position = [ 0.0, 0.0, 7.0 ]
    light.intensity = 0.9

    assert ( visualizer.HasCameraNamed( 'cam_orbit_0' ) == True )
    assert ( visualizer.HasLightNamed( 'light_point_0' ) == True )
    assert ( visualizer.GetCameraByName( 'cam_orbit_0' ) != None )
    assert ( visualizer.GetLightByName( 'light_point_0' ) != None )
    assert np.allclose( visualizer.GetCameraByName( 'cam_orbit_0' ).position, camera.position )
    assert np.allclose( visualizer.GetLightByName( 'light_point_0' ).ambient, light.ambient )

    assert ( camera.type == loco.sim.VizCameraType.ORBIT )
    assert np.allclose( camera.position, [ 5.0, 5.0, 5.0 ] )
    assert np.allclose( camera.target, [ 0.0, 0.0, 1.0 ] )

    assert ( light.type == loco.sim.VizLightType.POINT )
    assert np.allclose( light.position, [ 0.0, 0.0, 7.0 ] )
    assert np.allclose( light.ambient, [ 0.4, 0.4, 0.4 ] )
    assert np.allclose( light.diffuse, [ 0.8, 0.8, 0.8 ] )
    assert np.allclose( light.specular, [ 0.8, 0.8, 0.8 ] )
    assert np.allclose( light.intensity, 0.9 )
    del visualizer
    del scenario

if __name__ == '__main__' :
    _ = input( 'Press ENTER to start test : test_null_visualizer_functionality' )
    test_null_visualizer_functionality()

    _ = input( 'Press ENTER to continue ...' )