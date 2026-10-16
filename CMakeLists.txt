cmake_minimum_required(VERSION 3.19)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VER_RAW)
string(STRIP ${VER_RAW} ABYSSTOOLKIT_VERSION)

add_compile_definitions(ABYSSTOOLKIT_VERSION="${ABYSSTOOLKIT_VERSION}")

project(
  abysstoolkit
  VERSION ${ABYSSTOOLKIT_VERSION}
  DESCRIPTION "A modern C++ Wayland-native GUI toolkit")

include(CTest)
include(CheckIncludeFile)
include(GNUInstallDirs)

set(PREFIX ${CMAKE_INSTALL_PREFIX})
set(INCLUDE ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set(LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR})

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS "GLES3")
find_package(abyss-scanner 0.4.0 REQUIRED)

set(ABYSSAL_MINIMUM_VERSION 0.10.0)
set(ABYSSPARSE_MINIMUM_VERSION 0.6.0)
set(ABYSSUTILS_MINIMUM_VERSION 0.14.2)
set(ABYSSGRAPHICS_MINIMUM_VERSION 0.3.0)

# quotes and the spaces around '>=' in the list below are necessary for compability with pkg-config grama
set(PUBLIC_PKG_DEPS
    "abyssal >= ${ABYSSAL_MINIMUM_VERSION}"
    "abyssgraphics >= ${ABYSSGRAPHICS_MINIMUM_VERSION}"
    "abyssutils >= ${ABYSSUTILS_MINIMUM_VERSION}"
)
set(PRIVATE_PKG_DEPS
    wayland-client
    wayland-protocols
    egl
    "abyssparse >= ${ABYSSPARSE_MINIMUM_VERSION}"
    pixman-1
    libdrm
    gbm
    xkbcommon
    pango
    cairo
    pangocairo
    iniparser
    absl_flat_hash_map)

set(ALL_PKG_DEPS ${PUBLIC_PKG_DEPS} ${PRIVATE_PKG_DEPS})
set(ABYSSTOOLKIT_PUBLIC_REQUIRES ${PUBLIC_PKG_DEPS})
set(ABYSSTOOLKIT_PRIVATE_REQUIRES ${PRIVATE_PKG_DEPS})

pkg_check_modules(deps REQUIRED IMPORTED_TARGET ${ALL_PKG_DEPS})

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unused-value
                    -Wno-missing-field-initializers -Wpedantic)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES DEBUG)
  message(STATUS "Configuring abysstoolkit in Debug")
  add_compile_definitions(ABYSSTOOLKIT_DEBUG)
  set(BUILD_TESTING ON)
else()
  add_compile_options(-O3)
  message(STATUS "Configuring abysstoolkit in Release")
  set(BUILD_TESTING OFF)
endif()

add_compile_definitions(AT_HIDDEN=public)

file(GLOB_RECURSE SRCFILES CONFIGURE_DEPENDS "src/*.cpp" "include/*.hpp")
file(GLOB_RECURSE PUBLIC_HEADERS CONFIGURE_DEPENDS "include/*.hpp")

execute_process(COMMAND ./scripts/generateShaderIncludes.sh
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_library(abysstoolkit SHARED ${SRCFILES})
target_include_directories(
  abysstoolkit
  PUBLIC "./include"
  PRIVATE "./src" "./src/include" "./protocols" "${CMAKE_BINARY_DIR}")
set_target_properties(abysstoolkit PROPERTIES VERSION ${ABYSSTOOLKIT_VERSION}
                                              SOVERSION 6)
target_link_libraries(abysstoolkit PUBLIC OpenGL::EGL OpenGL::OpenGL
                                         PkgConfig::deps)

check_include_file("sys/inotify.h" HAS_INOTIFY)
pkg_check_modules(inotify IMPORTED_TARGET libinotify)
if(NOT HAS_INOTIFY AND inotify_FOUND)
  target_link_libraries(abysstoolkit PUBLIC PkgConfig::inotify)
  list(APPEND ABYSSTOOLKIT_PRIVATE_REQUIRES "libinotify")
endif()

# convert cmake list to pkg-config `Requires` string
string(REPLACE ";" ", " ABYSSTOOLKIT_PUBLIC_REQUIRES "${ABYSSTOOLKIT_PUBLIC_REQUIRES}")
string(REPLACE ";" ", " ABYSSTOOLKIT_PRIVATE_REQUIRES "${ABYSSTOOLKIT_PRIVATE_REQUIRES}")

# configure pkg-config after all actual dependencies have been determined,
# so that conditional dependencies can be correctly included.
configure_file(abysstoolkit.pc.in abysstoolkit.pc @ONLY)

# Protocols
pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)
message(STATUS "Found wayland-protocols at ${WAYLAND_PROTOCOLS_DIR}")
pkg_get_variable(WAYLAND_SCANNER_PKGDATA_DIR wayland-scanner pkgdatadir)
message(
  STATUS "Found wayland-scanner pkgdatadir at ${WAYLAND_SCANNER_PKGDATA_DIR}")

function(protocolNew protoPath protoName external)
  if(external)
    set(path ${CMAKE_SOURCE_DIR}/${protoPath})
  else()
    set(path ${WAYLAND_PROTOCOLS_DIR}/${protoPath})
  endif()
  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/protocols/${protoName}.cpp
           ${CMAKE_SOURCE_DIR}/protocols/${protoName}.hpp
    COMMAND abyss-scanner --client ${path}/${protoName}.xml
            ${CMAKE_SOURCE_DIR}/protocols/
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
  target_sources(abysstoolkit PRIVATE protocols/${protoName}.cpp
                                     protocols/${protoName}.hpp)
endfunction()
function(protocolWayland)
  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/protocols/wayland.cpp
           ${CMAKE_SOURCE_DIR}/protocols/wayland.hpp
    COMMAND
      abyss-scanner --wayland-enums --client
      ${WAYLAND_SCANNER_PKGDATA_DIR}/wayland.xml ${CMAKE_SOURCE_DIR}/protocols/
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
  target_sources(abysstoolkit PRIVATE protocols/wayland.cpp
                                     protocols/wayland.hpp)
endfunction()

protocolWayland()

protocolnew("stable/xdg-shell" "xdg-shell" false)
protocolnew("stable/linux-dmabuf" "linux-dmabuf-v1" false)
protocolnew("staging/fractional-scale" "fractional-scale-v1" false)
protocolnew("stable/viewporter" "viewporter" false)
protocolnew("staging/cursor-shape" "cursor-shape-v1" false)
protocolnew("staging/ext-session-lock" "ext-session-lock-v1" false)
protocolnew("stable/tablet" "tablet-v2" false)
protocolnew("unstable/text-input" "text-input-unstable-v3" false)
protocolnew("staging/linux-drm-syncobj" "linux-drm-syncobj-v1" false)
protocolnew("protocols" "wlr-layer-shell-unstable-v1" true)
protocolnew("unstable/keyboard-shortcuts-inhibit" "keyboard-shortcuts-inhibit-unstable-v1" false)

# Tests
if(BUILD_TESTING)
  enable_testing()

  find_package(Python3 COMPONENTS Interpreter QUIET)
  if(Python3_Interpreter_FOUND)
    add_test(NAME config_preprocess
             COMMAND ${Python3_EXECUTABLE} -B
                     ${CMAKE_CURRENT_SOURCE_DIR}/tests/config/test_preprocess.py)
    set_tests_properties(config_preprocess PROPERTIES
                         ENVIRONMENT "ABYSS_TEST_WORK_DIR=${CMAKE_CURRENT_BINARY_DIR}/config-tests")
  endif()

  # test apps
  add_custom_target(tests)

  add_executable(simpleWindow "tests/SimpleWindow.cpp")
  target_link_libraries(simpleWindow PRIVATE PkgConfig::deps abysstoolkit)
  add_dependencies(tests simpleWindow)

  add_executable(dialog "tests/Dialog.cpp")
  target_link_libraries(dialog PRIVATE PkgConfig::deps abysstoolkit)
  add_dependencies(tests dialog)

  add_executable(controls "tests/Controls.cpp")
  target_link_libraries(controls PRIVATE PkgConfig::deps abysstoolkit)
  add_dependencies(tests controls)

  add_executable(autosizeTest "tests/AutosizeTest.cpp")
  target_link_libraries(autosizeTest PRIVATE PkgConfig::deps abysstoolkit)
  add_dependencies(tests autosizeTest)

  add_executable(simpleSessionLock "tests/SimpleSessionLock.cpp")
  target_link_libraries(simpleSessionLock PRIVATE PkgConfig::deps abysstoolkit)
  add_dependencies(tests simpleSessionLock)

  add_executable(shortcutsInhibit "tests/ShortcutsInhibit.cpp")
  target_link_libraries(shortcutsInhibit PRIVATE PkgConfig::deps abysstoolkit)
  add_dependencies(tests shortcutsInhibit)

  add_executable(accentButton "tests/AccentButton.cpp")
  target_link_libraries(accentButton PRIVATE PkgConfig::deps abysstoolkit)
  add_dependencies(tests accentButton)

  add_executable(animations "tests/Animations.cpp")
  target_link_libraries(animations PRIVATE PkgConfig::deps abysstoolkit)
  add_dependencies(tests animations)


  # GTest
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
  file(GLOB_RECURSE TESTFILES CONFIGURE_DEPENDS "tests/unit/*.cpp")
  add_executable(abysstoolkit_tests ${TESTFILES})
  target_compile_options(abysstoolkit_tests PRIVATE --coverage)
  target_link_options(abysstoolkit_tests PRIVATE --coverage)
  target_include_directories(
    abysstoolkit_tests
    PUBLIC "./include"
    PRIVATE "./src" "./src/include" "./protocols" "${CMAKE_BINARY_DIR}")
  target_link_libraries(
    abysstoolkit_tests PRIVATE abysstoolkit GTest::gtest_main OpenGL::EGL
                                     OpenGL::OpenGL PkgConfig::deps)

  gtest_discover_tests(abysstoolkit_tests)

  add_executable(embeddedRendererTest "tests/EmbeddedRenderer.cpp")
  target_link_libraries(embeddedRendererTest PRIVATE abysstoolkit GTest::gtest_main OpenGL::EGL OpenGL::OpenGL PkgConfig::deps)
  gtest_discover_tests(embeddedRendererTest)

  # Add coverage to abysstoolkit
  target_compile_options(abysstoolkit PRIVATE --coverage)
  target_link_options(abysstoolkit PRIVATE --coverage)
endif()

# Installation
install(TARGETS abysstoolkit)
install(DIRECTORY "include/abysstoolkit" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/abysstoolkit.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(PROGRAMS tools/abyss-preprocess/abyss-preprocess
        DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES cmake/AbyssConfig.cmake
        DESTINATION ${CMAKE_INSTALL_DATADIR}/abysstoolkit/cmake)
