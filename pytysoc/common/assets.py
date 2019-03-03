
import os
from collections import namedtuple

# path to the root of the whole package
_PACKAGE_DIR = os.path.abspath( os.path.dirname( os.path.dirname( os.path.dirname( __file__ ) ) ) ) + '/'
# path to the resourced directory
_RESOURCES_DIR = os.path.join( _PACKAGE_DIR, 'res/' )
# path to the folder where to place temporal/working data
_WORKING_DIR = os.path.join( _PACKAGE_DIR, 'res/xml/' )
# paths to the templates folders
_TEMPLATES_MJCF_DIR     = os.path.join( _PACKAGE_DIR, 'res/templates/mjcf/' )
_TEMPLATES_URDF_DIR     = os.path.join( _PACKAGE_DIR, 'res/templates/urdf/' )
_TEMPLATES_RLSIM_DIR    = os.path.join( _PACKAGE_DIR, 'res/templates/rlsim/' )

PathsData = namedtuple( 'PathsData', 
                        [ 'PACKAGE_DIR',
                          'WORKING_DIR',
                          'RESOURCES_DIR',
                          'TEMPLATES_MJCF_DIR',
                          'TEMPLATES_URDF_DIR',
                          'TEMPLATES_RLSIM_DIR' ] )

def getAgentTemplates( templatesFolder, formatSuffix = '.xml' ) :
    _agentTemplates = []

    # check for all possible candidates in the given folder
    _candidateTemplates = os.listdir( templatesFolder )
    for _candidate in _candidateTemplates :
        # just skip folders
        if os.path.isdir( os.path.join( templatesFolder, _candidate ) ) :
            continue
        # and files not containing the required suffix
        if formatSuffix not in _candidate :
            continue

        _suffixPos = _candidate.find( formatSuffix )
        _agentFile = os.path.join( templatesFolder, _candidate )
        _agentName = _candidate[:_suffixPos]

        # store tuple (name,file) for key-value usage in named tuples
        _agentTemplates.append( (_agentName, _agentFile) )

    return _agentTemplates

_agentTemplatesMjcf = getAgentTemplates( _TEMPLATES_MJCF_DIR, '.xml' )
_agentTemplatesUrdf = getAgentTemplates( _TEMPLATES_URDF_DIR, '.urdf' )
_agentTemplatesRlsim = getAgentTemplates( _TEMPLATES_RLSIM_DIR, '.json' )

AgentIdsMjcf = namedtuple( 'AgentIdsMjcf',
                           [ agentName for agentName, _ in _agentTemplatesMjcf ] )
AgentIdsUrdf = namedtuple( 'AgentIdsUrdf',
                           [ agentName for agentName, _ in _agentTemplatesUrdf ] )
AgentIdsRlsim = namedtuple( 'AgentIdsRlsim',
                            [ agentName for agentName, _ in _agentTemplatesRlsim ] )

_agentsMjcf = AgentIdsMjcf( *[ agentFile for _, agentFile in _agentTemplatesMjcf ] )
_agentsUrdf = AgentIdsUrdf( *[ agentFile for _, agentFile in _agentTemplatesUrdf ] )
_agentsRlsim = AgentIdsRlsim( *[ agentFile for _, agentFile in _agentTemplatesRlsim ] )

AgentsTemplates = namedtuple( 'AgentsTemplates', ['MJCF', 'URDF', 'RLSIM'] )

# exposed function to initialize PATHS
def CreatePathsData() :
    return PathsData( _PACKAGE_DIR,
                      _WORKING_DIR,
                      _RESOURCES_DIR,
                      _TEMPLATES_MJCF_DIR,
                      _TEMPLATES_URDF_DIR,
                      _TEMPLATES_RLSIM_DIR )

# exposed function to initialize AGENTS
def CreateAgentsData() :
    return AgentsTemplates( _agentsMjcf, _agentsUrdf, _agentsRlsim )