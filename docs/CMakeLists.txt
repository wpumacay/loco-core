message( "LOCO::CORE::DOCS >>> Configuring build process for documentation generation" )

################################################################################
#                           Configuring Doxygen                                #
################################################################################

find_package( Doxygen )

if ( DOXYGEN_FOUND )
    # Find all loco-core related headers for the documentation
    get_property( LOCO_PUBLIC_INCS_DIR GLOBAL PROPERTY LOCO_INCS_DIR )
    file( GLOB_RECURSE LOCO_PUBLIC_INCS_FILES "${LOCO_PUBLIC_INCS_DIR}/*.h" )
    # Configure doxygen
    set( DOXYGEN_INPUT_DIR "${LOCO_PUBLIC_INCS_DIR}" )
    set( DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen" )
    set( DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/html/index.html" )
    set( DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" )
    set( DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" )
    # Replace variables inside @@ with the current values
    configure_file( ${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY )
    # Create the directory where to send the documentation
    file( MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR} )
    # Configure doxygen external command
    add_custom_command( OUTPUT ${DOXYGEN_INDEX_FILE}
                        DEPENDS ${LOCO_PUBLIC_INCS_FILES}
                        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                        COMMENT "LOCO::CORE::DOCS >>> Generating docs (Doxygen) ..." )
    add_custom_target( loco_cpp_docs_doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE} )
    message( "LOCO::CORE::DOCS >>> Successfully setup the build process for the cpp-docs(Doxygen)!" )
else()
    message( "LOCO::CORE::DOCS >>> Couldn't find Doxygen; skipping docs generation" )
endif()

################################################################################
#                           Configuring Doxygen
################################################################################

find_package( Sphinx )

if ( SPHINX_FOUND AND DOXYGEN_FOUND )
    # configure sphinx
    set( SPHINX_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}" )
    set( SPHINX_BUILD "${CMAKE_CURRENT_BINARY_DIR}/sphinx" )
    set( SPHINX_INDEX_FILE "${SPHINX_BUILD}/index.html" )
    # Configure sphinx external command
    add_custom_command( OUTPUT ${SPHINX_INDEX_FILE}
                        COMMAND 
                            ${SPHINX_EXECUTABLE} -b html 
                            # Tell Breathe where to find the Doxygen output
                            -Dbreathe_projects.Loco=${DOXYGEN_OUTPUT_DIR}/xml
                            ${SPHINX_SOURCE} ${SPHINX_BUILD}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        DEPENDS
                            ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
                            ${DOXYGEN_INDEX_FILE}
                        MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                        COMMENT "LOCO::CORE::DOCS >>> Generating docs (sphinx) ..." )
    add_custom_target( loco_cpp_docs_sphinx ALL DEPENDS ${SPHINX_INDEX_FILE} )
    message( "LOCO::CORE::DOCS >>> Successfully setup the build process for the cpp|python docs(Sphinx)!" )
else()
    message( "LOCO::CORE::DOCS >>> Couldn't find Sphinx; skipping docs generation" )
endif()