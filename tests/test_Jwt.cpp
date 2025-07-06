#include "gtest/gtest.h"
#include "Jwt.hpp"

TEST(JwtTest, TestEncodeAndDecode) {
    Jwt jwt_obj("my_secret", 3600, "my_issuer");
    std::string token = jwt_obj.encode("id", 42);
    auto decoded = jwt_obj.decode(token);
    EXPECT_EQ(decoded.get_issuer(), "my_issuer");
    EXPECT_EQ(decoded.get_payload_claim("id").as_string(), "42");
}