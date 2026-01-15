option(LIGHTYEAR_BUILD_EXAMPLES "Build Lightyear examples" ON)

if (NOT LIGHTYEAR_BUILD_EXAMPLES)
    return()
endif()

#------------------------------------------------------------
# Function that makes it easier to create example executables
#------------------------------------------------------------
function(ly_add_example)
    cmake_parse_arguments(EXAMPLE
            ""
            "NAME"
            "SOURCES;LIBS"
            ${ARGN}
    )

    if (NOT EXAMPLE_NAME)
        message(FATAL_ERROR "ly_add_example requires NAME")
    endif()

    add_executable(${EXAMPLE_NAME}
            ${EXAMPLE_SOURCES}
    )

    target_link_libraries(${EXAMPLE_NAME}
            PRIVATE ${EXAMPLE_LIBS}
    )

    set_target_properties(${EXAMPLE_NAME}
            PROPERTIES
            FOLDER "Examples"
    )
endfunction()


#------------------------------------------------------------
# Add all example subdirectory
#------------------------------------------------------------
add_subdirectory(logging)