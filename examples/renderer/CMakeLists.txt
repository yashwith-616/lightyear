find_package(glfw3 CONFIG REQUIRED)

set(SUPPORT_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/support/LogScope.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/support/Window.hpp)

ly_add_example(
        NAME ExampleSimpleTest
        SOURCES ExampleSimple.cpp ${SUPPORT_FILES}
        LIBS logging VulkanRenderer glfw
)

target_compile_definitions(ExampleSimpleTest PRIVATE GLFW_INCLUDE_NONE)

add_custom_command(TARGET ExampleSimpleTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:logging>
        $<TARGET_FILE_DIR:ExampleSimpleTest>
)

add_custom_command(TARGET ExampleSimpleTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:VulkanRenderer>
        $<TARGET_FILE_DIR:ExampleSimpleTest>
)

#-------------------------------------------------
# Shader Compilation Setup
#-------------------------------------------------
add_slang_shader(Shaders DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets" SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/assets/default.slang")