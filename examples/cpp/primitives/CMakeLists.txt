
message( "Configuring tysocCore::examples::primitives" )

FILE( GLOB PrimitivesCoreExampleSources *.cpp )

foreach( exampleFile ${PrimitivesCoreExampleSources} )
    
    string( REPLACE ".cpp" "" executableLongName ${exampleFile} )
    get_filename_component( execName ${executableLongName} NAME )

    add_executable( ${execName} 
                    ${exampleFile} )

    target_link_libraries( ${execName}
                           tysocCore )

endforeach( exampleFile )
