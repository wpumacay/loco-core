message( "LOCO::CORE::examples::cpp::single-bodies >>> Configuring C/C++ examples" )

FILE( GLOB ExampleSources *.cpp )

foreach( exampleFile ${ExampleSources} )
    string( REPLACE ".cpp" "" executableLongName ${exampleFile} )
    get_filename_component( execName ${executableLongName} NAME )
    FcnBuildExampleLoco( ${exampleFile} ${execName} )
endforeach( exampleFile )