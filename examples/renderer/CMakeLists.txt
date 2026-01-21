ly_add_example(
        NAME ExampleRendererTest
        SOURCES ExampleRendererTest.cpp
        LIBS logging VulkanRenderer
)

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