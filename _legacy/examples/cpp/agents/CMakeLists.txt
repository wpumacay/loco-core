
message( "Configuring tysocCore::examples::agents" )

FILE( GLOB AgentsCoreExampleSources *.cpp )

foreach( exampleFile ${AgentsCoreExampleSources} )
    
    string( REPLACE ".cpp" "" executableLongName ${exampleFile} )
    get_filename_component( execName ${executableLongName} NAME )

    add_executable( ${execName} 
                    ${exampleFile} )

    target_link_libraries( ${execName}
                           tysocCore )

endforeach( exampleFile )
