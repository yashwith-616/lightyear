ly_add_example(
        NAME ExampleBasic
        SOURCES ExampleBasic.cpp
        LIBS logging
)

add_custom_command(TARGET ExampleBasic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:logging>
        $<TARGET_FILE_DIR:ExampleBasic>
)