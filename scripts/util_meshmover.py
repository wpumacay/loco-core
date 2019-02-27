
import os
import subprocess as sp

RESOURCES_FOLDER = os.path.abspath( os.path.join( os.path.dirname( __file__ ), '../res/xml' ) )
WORKING_MESHES_FOLDER = os.path.join( RESOURCES_FOLDER, 'laikago_meshes' )

def moveResourceToRoot( rootFolder, filename, folder ) :
    _extensionPos = -1
    if '.stl' in filename :
        _extensionPos = filename.find( '.stl' )
    elif '.STL' in filename :
        _extensionPos = filename.find( '.STL' )
    elif '.obj' in filename :
        _extensionPos = filename.find( '.obj' )
    elif '.OBJ' in filename :
        _extensionPos = filename.find( '.OBJ' )

    if _extensionPos == -1 :
        return

    _normFilename = filename[:_extensionPos] + filename[_extensionPos:].lower()
    _srcFilepath = os.path.join( folder, filename )
    _dstFilepath = os.path.join( rootFolder, _normFilename )

    if _srcFilepath != _dstFilepath :
        print( 'moving: %s to %s' % ( _srcFilepath, _dstFilepath ) )
        sp.call( ['mv', _srcFilepath, _dstFilepath] )

def grabFromSubfolder( rootFolder, currentFolder ) :
    _elements = os.listdir( currentFolder )
    _elements = sorted( _elements, key=lambda x : os.path.isdir( x ) )

    for _elem in _elements :
        if os.path.isdir( os.path.join( currentFolder, _elem ) ) :
            _subfolder = os.path.join( currentFolder, _elem )

            grabFromSubfolder( rootFolder, _subfolder )
            ## sp.call( ['rm', '-r', _subfolder] )
        else :
            moveResourceToRoot( rootFolder, _elem, currentFolder )

if __name__ == '__main__' :
    print( 'resfolder: ', RESOURCES_FOLDER )
    print( 'meshesfolder: ', WORKING_MESHES_FOLDER )

    grabFromSubfolder( WORKING_MESHES_FOLDER, WORKING_MESHES_FOLDER )



