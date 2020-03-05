#!/usr/bin/env python

import loco
import numpy as np

def create_heights_data( nWidthSamples, nDepthSamples ) :
    # create the height data (just a paraboloid over [-1,+1])
    x = np.linspace( -1., 1., nWidthSamples )
    y = np.linspace( -1., 1., nDepthSamples )
    xx, yy = np.meshgrid( x, y )
    zz = 1. + np.sqrt( xx ** 2 + yy ** 2 )
    return zz.ravel()

def test_components_hfieldData() :
    hfdata = loco.sim.HeightFieldData()
    hfdata.nWidthSamples = 10
    hfdata.nDepthSamples = 20
    # try assigning (will check for row-major format, so default nparray should work)
    hfdata.heights = create_heights_data( hfdata.nWidthSamples, hfdata.nDepthSamples )
    # try grabbing the data to print it
    print( hfdata.heights )
    print( hfdata )

def test_components_shapeData() :
    shapeData = loco.sim.ShapeData()
    shapeData.type = loco.sim.ShapeType.BOX
    shapeData.size = [0.1, 0.2, 0.3]
    shapeData.localTf = np.identity(4)
    shapeData.mesh_data.filename = ''
    shapeData.hfield_data = loco.sim.HeightFieldData()
    shapeData.hfield_data.nWidthSamples = 10
    shapeData.hfield_data.nDepthSamples = 20
    shapeData.hfield_data.heights = create_heights_data( shapeData.hfield_data.nWidthSamples,
                                                         shapeData.hfield_data.nDepthSamples )
    print( shapeData )
    print( shapeData.hfield_data )

    assert np.allclose( shapeData.hfield_data.heights, create_heights_data( 10, 20 ) ), "test_components_shapeData >>> height values don't match"

if __name__ == '__main__' :
    _ = input( 'Press ENTER to start test : test_components_hfieldData' )
    test_components_hfieldData()
    _ = input( 'Press ENTER to start test : test_components_shapeData' )
    test_components_shapeData()
    _ = input( 'Press ENTER to continue ...' )