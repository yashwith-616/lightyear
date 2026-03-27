#-------------------------------------------------
# Add Slang Shader
#-------------------------------------------------
find_package(Slang CONFIG REQUIRED)
function (add_slang_shader TARGET)
    cmake_parse_arguments("SHADER" "" "DIRECTORY" "SOURCES" ${ARGN})
    if(NOT SHADER_DIRECTORY)
        set(SHADER_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
    endif()

    set (ENTRY_POINTS -entry vertMain -entry fragMain)
    set(OUTPUT_FILE ${SHADER_DIRECTORY}/${TARGET}.spv)

    if(NOT SLANGC_EXECUTABLE)
        message(FATAL_ERROR "Could not find Slang executable.")
    endif()

    add_custom_command(
            OUTPUT  ${OUTPUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_DIRECTORY}
            # Note: Added -o ${OUTPUT_FILE} to ensure it goes exactly where we want
            COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES}
            -target spirv -profile spirv_1_4
            -emit-spirv-directly -fvk-use-entrypoint-name
            ${ENTRY_POINTS} -o ${OUTPUT_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${SHADER_SOURCES}
            COMMENT "Compiling Slang Shader: ${TARGET}"
            VERBATIM
    )

    add_custom_target(${TARGET} DEPENDS ${OUTPUT_FILE})
endfunction(add_slang_shader)
