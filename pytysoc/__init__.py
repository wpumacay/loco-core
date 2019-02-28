
import os
from pytysoc.common import assets

PATHS = assets.CreatePathsData()
AGENTS = assets.CreateAgentsData()

def initialize() :
    print( 'Initialized internals' )

def createRuntime( physicsBackendLib = '', renderingBackendLib = '' ) :
    pass

def createAgent() :
    pass

def createScenario() :
    pass

initialize()