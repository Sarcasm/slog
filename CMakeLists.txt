cmake_minimum_required(VERSION 3.9)

project(slog VERSION 0.1)

include(GNUInstallDirs)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra)
endif()

include(CTest)

include(CheckCXXCompilerFlag)

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

find_package(FMT REQUIRED)

add_library(libslog
  include/slog/slog.hpp
  src/slog.cpp)
set_target_properties(libslog PROPERTIES
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
add_library(slog::slog ALIAS libslog)
install(TARGETS libslog
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(slog tool/slog.cpp)
set_target_properties(slog PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
target_compile_features(slog PRIVATE cxx_std_11)
target_link_libraries(slog libslog)
install(TARGETS slog RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/slog DESTINATION include)

if (BUILD_TESTING)
  add_subdirectory(test)
endif()
