
# Create a simple build helper for our core examples
function( FcnBuildCppCoreExample pSourcesList pExecutableName )

    add_executable( ${pExecutableName}
                    ${pSourcesList} )

    target_link_libraries( ${pExecutableName}
                           tysocCore )

endfunction()

# add the actual c++ core examples
add_subdirectory( cpp )
