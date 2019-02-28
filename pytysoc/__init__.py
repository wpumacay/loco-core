
import os

# @DEBUG: for testing purposes only (to avoid installation) ####################
import sys
_bindingsDir = os.path.abspath( os.path.join( os.path.dirname( __file__ ), '../build/pytysoc/bindings' ) )
sys.path.insert( 0, _bindingsDir )
print( 'sys.path: ', sys.path )
# ##############################################################################

import tysoc_bindings

from pytysoc.common import assets

PATHS = assets.CreatePathsData()
AGENTS = assets.CreateAgentsData()

def initialize() :
    tysoc_bindings.initializeMjcfSchemaPath( os.path.join( PATHS.RESOURCES_DIR, 'xml/schema.xml' ) )
    tysoc_bindings.initializeLoader( PATHS.TEMPLATES_MJCF_DIR,
                                     PATHS.TEMPLATES_URDF_DIR,
                                     PATHS.TEMPLATES_RLSIM_DIR )
    print( 'Initialized internals' )

def createRuntime( physicsBackendLib = '', renderingBackendLib = '' ) :
    pass

def createAgent() :
    pass

def createScenario() :
    pass

initialize()