message( "LOCO::CORE::tests::cpp >>> Configuring C/C++ tests" )

include_directories( "${LOCO_CORE_INCLUDE_DIRS}" )

function( FcnBuildTest pSourcesList pExecutableName )

    add_executable( ${pExecutableName}
                    ${pSourcesList} )

    target_link_libraries( ${pExecutableName}
                           tysocCore )

endfunction()

FILE( GLOB TestSources *.cpp )

foreach( testFile ${TestSources} )
    
    string( REPLACE ".cpp" "" executableLongName ${testFile} )
    get_filename_component( execName ${executableLongName} NAME )

    FcnBuildTest( ${testFile} ${execName} )

endforeach( testFile)