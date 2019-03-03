
import os

import tysoc_bindings

from pytysoc.common import assets
from pytysoc.runtime import backends

PATHS = assets.CreatePathsData()
AGENTS = assets.CreateAgentsData()

BACKENDS = backends.CreateBackendsData()

def initialize() :
    tysoc_bindings.initializeMjcfSchemaPath( os.path.join( PATHS.RESOURCES_DIR, 'xml/schema.xml' ) )
    tysoc_bindings.initializeLoader( PATHS.TEMPLATES_MJCF_DIR,
                                     PATHS.TEMPLATES_URDF_DIR,
                                     PATHS.TEMPLATES_RLSIM_DIR )

def createRuntime( physicsBackend = '', 
                   renderingBackend = '',
                   workingDir = PATHS.WORKING_DIR ) :
    print( 'renderingBackend: ', renderingBackend )
    print( 'physicsBackend', physicsBackend )
    ## ipdb.set_trace()
    return tysoc_bindings.PyRuntime( physicsBackend, renderingBackend, workingDir )

def createAgent() :
    pass

def createScenario() :
    pass

initialize()