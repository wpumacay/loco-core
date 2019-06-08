
message( "Configuring tysocCore::examples::Agents" )

FILE( GLOB AgentsCoreExampleSources *.cpp )

foreach( exampleFile ${AgentsCoreExampleSources} )
    
    string( REPLACE ".cpp" "" executableLongName ${exampleFile} )
    get_filename_component( execName ${executableLongName} NAME )

    FcnBuildCppCoreExample( ${exampleFile} ${execName} )

endforeach( exampleFile )
