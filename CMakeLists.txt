cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    maths_static_compiler
    VERSION 0.1.0
    DESCRIPTION "A generic static compiler for simple mathematical expressions"
    HOMEPAGE_URL "https://molchanovia.dev/maths_static_compiler"
    LANGUAGES CXX
)

# Primarily use of C++20 in all compilers
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -std=c++20")

cmake_policy(SET CMP0167 OLD)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----


add_library(
    maths_static_compiler_lib OBJECT
    source/exceptions.h 
    source/frontend/scanning/token.h 
    source/frontend/scanning/lexer.h 
    source/frontend/scanning/lexer.cc
    source/frontend/parsing/expression.h
    source/frontend/parsing/expression.cc
    source/frontend/parsing/parser.h
    source/backend/control_flow_builder.h
    source/backend/control_flow_builder.cc
    source/backend/executor.h
)

target_include_directories(
  maths_static_compiler_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(maths_static_compiler_lib PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
find_package(Boost 1.87.0 COMPONENTS program_options json)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(maths_static_compiler_lib PRIVATE fmt::fmt Boost::program_options Boost::json)
set_target_properties(maths_static_compiler_lib PROPERTIES LINKER_LANGUAGE CXX)
# ---- Declare executable ----

add_definitions(-DPROJECT_VERSION_STRING="${PROJECT_VERSION}")
add_definitions(-DPROJECT_HOMEPAGE_URL="${PROJECT_HOMEPAGE_URL}")

add_executable(maths_static_compiler_exe source/main.cc)
add_executable(maths_static_compiler::exe ALIAS maths_static_compiler_exe)

set_property(TARGET maths_static_compiler_exe PROPERTY OUTPUT_NAME maths_static_compiler)

target_compile_features(maths_static_compiler_exe PRIVATE cxx_std_20)

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
