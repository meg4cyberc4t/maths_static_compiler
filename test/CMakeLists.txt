# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(maths_static_compilerTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(
    maths_static_compiler_test 
    source/lexer_test.cc
    source/parser_test.cc
)
target_link_libraries(
    maths_static_compiler_test PRIVATE
    maths_static_compiler_lib
    Catch2::Catch2WithMain
)
target_compile_features(maths_static_compiler_test PRIVATE cxx_std_20)

catch_discover_tests(maths_static_compiler_test DISCOVERY_MODE PRE_TEST)

# ---- End-of-file commands ----

add_folders(Test)
