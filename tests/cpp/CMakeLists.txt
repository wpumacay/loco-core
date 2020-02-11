message( "LOCO::CORE::tests::cpp >>> Configuring C/C++ tests" )

include_directories( "${LOCO_CORE_INCLUDE_DIRS}" )
#### include_directories( "${CMAKE_SOURCE_DIR}/ext/googletest/googletest/include" )

function( FcnBuildTest pSourcesList pExecutableName )
    add_executable( ${pExecutableName} ${pSourcesList} )
    target_link_libraries( ${pExecutableName} loco_core gtest_main )
    add_test( NAME "${pExecutableName}_test" COMMAND "${pExecutableName}" )
endfunction()

FILE( GLOB TestSources *.cpp )

foreach( testFile ${TestSources} )
    string( REPLACE ".cpp" "" executableLongName ${testFile} )
    get_filename_component( execName ${executableLongName} NAME )
    FcnBuildTest( ${testFile} ${execName} )
endforeach( testFile )