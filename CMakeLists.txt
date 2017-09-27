cmake_minimum_required(VERSION 3.9)

project(slog VERSION 0.1)

include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra)
endif()

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

add_library(libslog src/slog.cpp include/slog/slog.hpp)
set_target_properties(libslog PROPERTIES OUTPUT_NAME slog)
target_include_directories(libslog PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  )
install(TARGETS libslog
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

add_executable(slog tool/slog.cpp)
target_link_libraries(slog PRIVATE libslog)
install(TARGETS slog RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/slog DESTINATION include)
