# SEOS Core API

project(seos_core_api C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        .
        include
        os_util
)

# if invoked from the SEOS sandbox, provide a documentation build target
if (COMMAND seos_create_doxygen_target)

    # there are no pre or post steps
    seos_create_doxygen_target(seos_core_api_doc ":" ":")

endif()
