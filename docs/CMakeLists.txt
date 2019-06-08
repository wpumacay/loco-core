
################################################################################
#                           Configuring Doxygen
################################################################################

find_package( Doxygen REQUIRED )

# Configure the docs output
set( DOXYGEN_INPUT_DIRS "${CMAKE_SOURCE_DIR}/src/ ${CMAKE_SOURCE_DIR}/include/" )
set( DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen" )
set( DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/html/index.html" )
set( DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile" )
set( DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" )

# replace variables inside @@, e.g. @VARIABLE_NAME@ with their actual values
configure_file( ${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY )

# create the output directory for our docs
file( MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR} )

# create the command to build our docs
add_custom_command( OUTPUT ${DOXYGEN_INDEX_FILE}
                    DEPENDS tysocCore
                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                    MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                    COMMENT "Generating docs with Doxygen"
                    VERBATIM )

# create a target for cmake to use when configuring
add_custom_target( Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE} )

################################################################################
#                           Configuring Doxygen
################################################################################

find_package( Sphinx REQUIRED )

set( SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR} )
set( SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx )
set( SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html )

add_custom_command( OUTPUT ${SPHINX_INDEX_FILE}
                    COMMAND 
                        ${SPHINX_EXECUTABLE} -b html 
                        # Tell Breathe where to find the Doxygen output
                        -Dbreathe_projects.tysoc="${DOXYGEN_OUTPUT_DIR}/xml"
                        ${SPHINX_SOURCE} ${SPHINX_BUILD}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    DEPENDS
                        ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
                        ${DOXYGEN_INDEX_FILE}
                    MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                    COMMENT "Generating documentation with Sphinx" )

add_custom_target( Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE} )