message( "LOCO::CORE::examples::cpp >>> Configuring C/C++ examples" )

include_directories( "${LOCO_CORE_INCLUDE_DIRS}" )

function( FcnBuildExampleLoco pSourcesList pExecutableName )
    add_executable( ${pExecutableName} ${pSourcesList} )
    target_link_libraries( ${pExecutableName} loco_core )
endfunction()

add_subdirectory( single_bodies )