
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
                   renderingBackend = '' ) :
    print( 'LOG> renderingBackend: ', renderingBackend )
    print( 'LOG> physicsBackend', physicsBackend )
    ## ipdb.set_trace()
    return tysoc_bindings.PyRuntime( physicsBackend, renderingBackend )

def createAgent( agentName = 'agent_0', 
                 modelName = 'humanoid', 
                 modelFormat = 'mjcf',
                 startpos = [0, 0, 2] ) :

    return tysoc_bindings.PyCoreAgent( agentName, startpos, modelFormat, modelName )

def createStaticTerrainGen( terrainGenName ) :
    return tysoc_bindings.PyStaticTerrainGen( terrainGenName )

def createSensorIntrinsics( sensorName, agent ) :
    return tysoc_bindings.PySensorIntrinsics( sensorName, agent )

def createScenario() :
    return tysoc_bindings.PyScenario()

initialize()