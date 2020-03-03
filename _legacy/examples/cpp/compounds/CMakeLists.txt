
message( "Configuring tysocCore::examples::compounds" )

FILE( GLOB CompoundsCoreExampleSources *.cpp )

foreach( exampleFile ${CompoundsCoreExampleSources} )
    
    string( REPLACE ".cpp" "" executableLongName ${exampleFile} )
    get_filename_component( execName ${executableLongName} NAME )

    add_executable( ${execName} 
                    ${exampleFile} )

    target_link_libraries( ${execName}
                           tysocCore )

endforeach( exampleFile )
