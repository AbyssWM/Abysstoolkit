# AbyssConfig.cmake
# CMake integration for #include.abyss config system

# Capture the module location when included, not the caller's source directory.
set(_ABYSS_CONFIG_MODULE_DIR "${CMAKE_CURRENT_LIST_DIR}")

function(abyss_add_config_support TARGET APP_NAME)
    # Find abyss-preprocess tool
    find_program(ABYSS_PREPROCESS abyss-preprocess
        HINTS "${_ABYSS_CONFIG_MODULE_DIR}/../tools/abyss-preprocess"
    )
    
    if(NOT ABYSS_PREPROCESS)
        message(FATAL_ERROR "abyss-preprocess not found for ${TARGET}")
    endif()
    
    message(STATUS "Found abyss-preprocess: ${ABYSS_PREPROCESS}")
    
    # Set paths
    if(DEFINED ABYSS_CONFIG_DIR AND NOT ABYSS_CONFIG_DIR STREQUAL "")
        if(NOT IS_ABSOLUTE "${ABYSS_CONFIG_DIR}")
            message(FATAL_ERROR "ABYSS_CONFIG_DIR must be an absolute path")
        endif()
        set(CONFIG_DIR "${ABYSS_CONFIG_DIR}")
    elseif(IS_ABSOLUTE "$ENV{XDG_CONFIG_HOME}")
        set(CONFIG_DIR "$ENV{XDG_CONFIG_HOME}/abyss")
    else()
        set(CONFIG_DIR "$ENV{HOME}/.config/abyss")
    endif()
    set(CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/abyss-config/${TARGET}")
    set(GENERATED_HEADER "${CMAKE_CURRENT_BINARY_DIR}/generated/${TARGET}Configs.hpp")
    
    # Custom command to process configs
    # Scan at every build: additions/removals and environment expansion cannot
    # be represented by a static list of input files. Unchanged outputs retain
    # their timestamps so consumers do not rebuild unnecessarily.
    add_custom_target(${TARGET}_preprocess_configs
        BYPRODUCTS "${GENERATED_HEADER}"
        COMMAND ${ABYSS_PREPROCESS}
            --app ${APP_NAME}
            --config-dir "${CONFIG_DIR}"
            --output-dir "${CACHE_DIR}"
            --generate-header "${GENERATED_HEADER}"
        COMMENT "Processing #include.abyss configs for ${APP_NAME}"
        VERBATIM
    )
    
    # Make main target depend on preprocessing
    add_dependencies(${TARGET} ${TARGET}_preprocess_configs)
    
    # Add generated header to include path
    target_include_directories(${TARGET} PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/generated"
    )
    
    # Define ABYSS_CONFIG_AVAILABLE so code knows configs are enabled
    target_compile_definitions(${TARGET} PRIVATE ABYSS_CONFIG_AVAILABLE)
    
    message(STATUS "Added config support for ${TARGET} (app: ${APP_NAME})")
endfunction()
