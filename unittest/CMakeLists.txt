
message( "Configuring tysocCore::unittest" )

function( FcnBuildUnitTest pSourcesList pExecutableName )

    add_executable( ${pExecutableName}
                    ${pSourcesList} )

    target_link_libraries( ${pExecutableName}
                           tysocCore )

endfunction()

FILE( GLOB UnitTestSources *.cpp )

foreach( testFile ${UnitTestSources} )
    
    string( REPLACE ".cpp" "" executableLongName ${testFile} )
    get_filename_component( execName ${executableLongName} NAME )

    FcnBuildUnitTest( ${testFile} ${execName} )

endforeach( testFile )

message( "tysocCore::unittest::UnitTestSources: ${UnitTestSources}" )