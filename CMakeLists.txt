cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    maths_static_compiler
    VERSION 0.1.0
    DESCRIPTION "A generic static compiler for simple mathematical expressions"
    HOMEPAGE_URL "https://molchanovia.dev/maths_static_compiler"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    maths_static_compiler_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    maths_static_compiler_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(maths_static_compiler_lib PUBLIC cxx_std_17)

find_package(fmt REQUIRED)
target_link_libraries(maths_static_compiler_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(maths_static_compiler_exe source/main.cpp)
add_executable(maths_static_compiler::exe ALIAS maths_static_compiler_exe)

set_property(TARGET maths_static_compiler_exe PROPERTY OUTPUT_NAME maths_static_compiler)

target_compile_features(maths_static_compiler_exe PRIVATE cxx_std_17)

target_link_libraries(maths_static_compiler_exe PRIVATE maths_static_compiler_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT maths_static_compiler_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of maths_static_compiler"
  )
endif()

include(cmake/dev-mode.cmake)
