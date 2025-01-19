#include <catch2/catch_test_macros.hpp>

#include "lib.hpp"

TEST_CASE("Name is maths_static_compiler", "[library]")
{
  auto const lib = library {};
  REQUIRE(lib.name == "maths_static_compiler");
}
