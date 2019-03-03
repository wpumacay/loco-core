
import os
from collections import namedtuple

# path to the root of the whole package
_PACKAGE_DIR = os.path.abspath( os.path.dirname( os.path.dirname( os.path.dirname( __file__ ) ) ) ) + '/'
# search for libraries in this folder
_renderingBackends = []
_physicsBackends = []

def extractLibName( fullLibraryName, commonTag ) :
    # filter the name first by the common tag (tysocRendering|tysocPhysics)
    _trimpos = fullLibraryName.find( commonTag ) + len( commonTag )
    _nameWithExtension = fullLibraryName[_trimpos:]
    # filter out the library name by removing the extension
    _libname, _ = _nameWithExtension.split( '.' )
    return _libname.upper()

_elements = os.listdir( _PACKAGE_DIR )
for _elm in _elements :
    if os.path.isdir( _elm ) :
        continue

    # grab all physics backends
    if 'tysocPhysics' in _elm :
        _libpath = os.path.join( _PACKAGE_DIR, _elm )
        _libname = extractLibName( _elm, 'tysocPhysics' )
        _physicsBackends.append( { 'path': _libpath, 'name': _libname } )

    # and the rendering backends as well
    if 'tysocRendering' in _elm :
        _libpath = os.path.join( _PACKAGE_DIR, _elm )
        _libname = extractLibName( _elm, 'tysocRendering' )
        _renderingBackends.append( { 'path': _libpath, 'name': _libname } )



PhysicsBackendsData = namedtuple( 'PhysicsBackendsData',
                                  [ libdata['name'] for libdata in _physicsBackends ] )
RenderingBackendsData = namedtuple( 'RenderingBackendsData',
                                    [ libdata['name'] for libdata in _renderingBackends ] )

RuntimesData = namedtuple( 'BackendsData',
                           [ 'PHYSICS', 'RENDERING' ] )

_physicsBackendsData = PhysicsBackendsData( *[ libdata['path'] for libdata in _physicsBackends ] )
_renderingBackendsData = RenderingBackendsData( *[ libdata['path'] for libdata in _renderingBackends ] )

# expose function to initialize RUNTIMES data
def CreateBackendsData() :
    return RuntimesData( _physicsBackendsData, _renderingBackendsData )