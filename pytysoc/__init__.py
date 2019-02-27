
import os

import sample_cpp_wrapper

_ROOT_DIR = os.path.abspath( os.path.dirname( __file__ ) )
_ASSETS_DIR = os.path.join( os.path.dirname( _ROOT_DIR ), 'res' )

_ASSETS_DIR_XML = os.path.join( _ASSETS_DIR, 'xml' )
_ASSETS_DIR_JSON = os.path.join( _ASSETS_DIR, 'json' )
_ASSETS_DIR_IMGS = os.path.join( _ASSETS_DIR, 'imgs' )

_FILENAMES_XML = [ 'hello_world' ]
_FILENAMES_JSON = [ 'hello_world' ]
_FILENAMES_IMGS = [ 'img_wpumacay_logo' ]

class AssetInfo:
    
    def __init__( self ) :
        self.XML = {}
        self.JSON = {}
        self.IMGS = {}

ASSETS = AssetInfo()

ASSETS.XML = { filename : os.path.join( _ASSETS_DIR_XML, filename ) + '.xml'
                    for filename in _FILENAMES_XML }
ASSETS.JSON = { filename : os.path.join( _ASSETS_DIR_JSON, filename ) + '.json'
                    for filename in _FILENAMES_JSON }
ASSETS.IMGS = { filename : os.path.join( _ASSETS_DIR_IMGS, filename ) + '.png'
                    for filename in _FILENAMES_IMGS }

def GetContentFromXML( xmlfile ) :
    return sample_cpp_wrapper.GetContentFromXML( xmlfile )

def GetContentFromJSON( jsonfile ) :
    return sample_cpp_wrapper.GetContentFromJSON( jsonfile )