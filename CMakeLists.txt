# OS Core API

project(os_core_api C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

# if invoked from the OS sandbox, provide a documentation build target
if (COMMAND os_create_doxygen_target)

    # there are no pre or post steps
    os_create_doxygen_target(os_core_api_doc ":" ":")

endif()
