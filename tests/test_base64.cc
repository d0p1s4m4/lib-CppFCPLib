#include <catch2/catch_test_macros.hpp>

#include <fcp++/codec/base64.hpp>

TEST_CASE("encode empty string", "[codec::base64]")
{
  REQUIRE(fcp::codec::base64::encode("") == "");
}

TEST_CASE("encode \"Many hands make light work.\"", "[codec::base64]")
{
  REQUIRE(fcp::codec::base64::encode("Many hands make light work.") ==
          "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");
}

TEST_CASE("decode empty string", "[codec::base64]")
{
  REQUIRE(fcp::codec::base64::decode("") == "");
}