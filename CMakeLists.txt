# OS Core API


if (SDK_USE_CAMKES)

    CAmkESAddCPPInclude(${OS_LIBS_DIR}/os_core_api/camkes)
    CAmkESAddImportPath(${OS_LIBS_DIR}/os_core_api/camkes)

endif()


#-------------------------------------------------------------------------------
project(os_core_api C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)
