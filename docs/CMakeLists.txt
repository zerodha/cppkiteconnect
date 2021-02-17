
find_package(Doxygen)

if (DOXYGEN_FOUND)

    # set input, output & config files
    set(DOXY_FILE config.doxygen)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/${DOXY_FILE})
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

    # configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Note: Don't put 'ALL'. Doing so will build build docs everytime.
    add_custom_target( docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen.."
        VERBATIM )

else (DOXYGEN_FOUND)
    message("Doxygen is required for generating documentation.")
endif (DOXYGEN_FOUND)