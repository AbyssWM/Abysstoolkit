# AbyssConfig.cmake
# CMake integration for #include.abyss config system

function(abyss_add_config_support TARGET APP_NAME)
    # Find abyss-preprocess tool
    find_program(ABYSS_PREPROCESS abyss-preprocess
        PATHS ${CMAKE_SOURCE_DIR}/../Abysstoolkit/tools/abyss-preprocess
              ${CMAKE_SOURCE_DIR}/../../Abysstoolkit/tools/abyss-preprocess
              ${CMAKE_CURRENT_SOURCE_DIR}/../Abysstoolkit/tools/abyss-preprocess
        NO_DEFAULT_PATH
    )
    
    if(NOT ABYSS_PREPROCESS)
        message(WARNING "abyss-preprocess not found, skipping config preprocessing for ${TARGET}")
        return()
    endif()
    
    message(STATUS "Found abyss-preprocess: ${ABYSS_PREPROCESS}")
    
    # Set paths
    set(CONFIG_DIR "$ENV{HOME}/.config/abyss")
    set(CACHE_DIR "/tmp/abyss-config-cache/${APP_NAME}")
    set(GENERATED_HEADER "${CMAKE_BINARY_DIR}/generated/${TARGET}Configs.hpp")
    
    # Custom command to process configs
    add_custom_command(
        OUTPUT ${GENERATED_HEADER}
        COMMAND ${ABYSS_PREPROCESS}
            --app ${APP_NAME}
            --config-dir "${CONFIG_DIR}"
            --output-dir "${CACHE_DIR}"
            --generate-header "${GENERATED_HEADER}"
        COMMENT "Processing #include.abyss configs for ${APP_NAME}"
        VERBATIM
    )
    
    # Custom target that depends on the generated header
    add_custom_target(${TARGET}_preprocess_configs
        DEPENDS ${GENERATED_HEADER}
    )
    
    # Make main target depend on preprocessing
    add_dependencies(${TARGET} ${TARGET}_preprocess_configs)
    
    # Add generated header to include path
    target_include_directories(${TARGET} PRIVATE
        ${CMAKE_BINARY_DIR}/generated
    )
    
    # Define ABYSS_CONFIG_AVAILABLE so code knows configs are enabled
    target_compile_definitions(${TARGET} PRIVATE ABYSS_CONFIG_AVAILABLE)
    
    message(STATUS "Added config support for ${TARGET} (app: ${APP_NAME})")
endfunction()
