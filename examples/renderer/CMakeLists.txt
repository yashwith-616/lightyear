find_package(glfw3 CONFIG REQUIRED)

ly_add_example(
        NAME ExampleRendererTest
        SOURCES ExampleRendererTest.cpp
        LIBS logging VulkanRenderer glfw
)

target_compile_definitions(ExampleRendererTest PRIVATE GLFW_INCLUDE_NONE)

add_custom_command(TARGET ExampleRendererTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:logging>
        $<TARGET_FILE_DIR:ExampleRendererTest>
)

add_custom_command(TARGET ExampleRendererTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:VulkanRenderer>
        $<TARGET_FILE_DIR:ExampleRendererTest>
)

#-------------------------------------------------
# Shader Compilation Setup
#-------------------------------------------------
add_slang_shader(Shaders DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets" SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/assets/default.slang")