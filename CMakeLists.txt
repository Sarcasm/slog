cmake_minimum_required(VERSION 3.9)

project(slog VERSION 0.1)

include(CMakePackageConfigHelpers)
include(CTest)
include(CheckCXXCompilerFlag)
include(GNUInstallDirs)

# Turn this on for local development or continuous integration
option(SLOG_WARNINGS_AS_ERRORS "Whether or not to treat warnings as errors" OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra)
  if (SLOG_WARNINGS_AS_ERRORS)
    add_compile_options(-Werror)
  endif()
endif()

# Setup the flag to enable colored diagnostics if any.
#
# For now this option is enforced only for Ninja builds,
# where compiler output is redirected to pipes.
#
# Clang has '-fcolor-diagnostics' for a long time now.
# Since GCC 4.9, a similar flag has been added '-fdiagnostics-color'
# (somehow they managed to use another syntax than Clang's one...).
# Recent version of Clang will support both
# as they added support for GCC's -fdiagnostics-color.
function(enable_colored_diagnotics)
  if (${CMAKE_GENERATOR} MATCHES "Ninja")
    # Clang
    check_cxx_compiler_flag("-fcolor-diagnostics" HAS_FCOLOR_DIAGNOSTICS_FLAG)
    if (HAS_FCOLOR_DIAGNOSTICS_FLAG)
      add_compile_options(-fcolor-diagnostics)
    else() # GCC (and Clang for compatibility with GCC)
      check_cxx_compiler_flag("-fdiagnostics-color" HAS_DIAGNOSTICS_FCOLOR_FLAG)
      if (HAS_DIAGNOSTICS_FCOLOR_FLAG)
        add_compile_options(-fdiagnostics-color)
      endif()
    endif()
  endif()
endfunction()

enable_colored_diagnotics()

find_package(FMT 4 REQUIRED)

add_library(libslog
  include/slog/slog.hpp
  src/slog.cpp)
add_library(slog::libslog ALIAS libslog)
set_target_properties(libslog PROPERTIES
  # libslog is used just to disambiguates the slog executable,
  # however this would result in a library named liblibslog.so
  # so give proper OUTPUT_NAME to the library
  OUTPUT_NAME slog
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
target_compile_features(libslog PUBLIC cxx_std_11)
target_include_directories(libslog
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(libslog PUBLIC fmt::fmt)

add_executable(slog tool/slog.cpp)
set_target_properties(slog PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
target_compile_features(slog PRIVATE cxx_std_11)
target_link_libraries(slog libslog)

set(CMAKE_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# install targets, courtesy of
# https://cmake.org/cmake/help/v3.9/manual/cmake-packages.7.html#creating-packages
install(DIRECTORY include/slog DESTINATION include)
install(TARGETS libslog
        EXPORT ${PROJECT_NAME}-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE slog::
        DESTINATION ${CMAKE_CONFIG_INSTALL_DIR})

# add valid configuration files to the build tree as well,
# https://cmake.org/cmake/help/v3.9/manual/cmake-packages.7.html#creating-a-package-configuration-file-for-the-build-tree
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-config-version.cmake"
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY SameMajorVersion)
export(EXPORT ${PROJECT_NAME}-targets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-targets.cmake"
  NAMESPACE slog::)
configure_file(cmake/${PROJECT_NAME}-config.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-config.cmake"
  COPYONLY)

install(
  FILES
    cmake/${PROJECT_NAME}-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-config-version.cmake"
  DESTINATION
    ${CMAKE_CONFIG_INSTALL_DIR})

if (BUILD_TESTING)
  add_subdirectory(test)
endif()
