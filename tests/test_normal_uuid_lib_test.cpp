#include "gtest/gtest.h"
#include "normal_uuid_lib_test.hpp"
#include <uuid.h>

TEST(NormalUuidLibTest, GenerateUuidProducesNonNil)
{
    uuid_t uu;
    generate_uuid(uu);
    // Check that the generated UUID is not nil (all zeros)
    uuid_t nil_uuid = {0};
    bool is_nil = (memcmp(uu, nil_uuid, sizeof(uuid_t)) == 0);
    EXPECT_FALSE(is_nil);
}