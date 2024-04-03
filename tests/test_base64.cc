#include <catch2/catch.hpp>

#include "Base64.h"

TEST_CASE("Base64 encode", "[Base64]") {
    REQUIRE(FCPLib::Base64::base64Encode((const unsigned char *)"Many hands make light work.", 27) == "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");
}