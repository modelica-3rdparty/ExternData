/* Test_ED_JSON.cc - JSON functions tester
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "../Include/ED_JSONFile.h"
#include "Constants.h"
#include <gtest/gtest.h>

namespace
{

TEST(ED_JSON, Create) {
    auto handle = ED_createJSON("../Examples/test.json", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    ED_destroyJSON(handle);
}

TEST(ED_JSON, GetDouble) {
    auto handle = ED_createJSON("../Examples/test.json", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    int exist;
    auto val = ED_getDoubleFromJSON(handle, "set1.gain.k", &exist);
    EXPECT_EQ(2, val);
    EXPECT_EQ(1, exist);
    ED_destroyJSON(handle);
}

TEST(ED_JSON, GetInt) {
    auto handle = ED_createJSON("../Examples/test.json", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    int exist;
    auto val = ED_getIntFromJSON(handle, "set1.gain.k", &exist);
    EXPECT_EQ(2, val);
    EXPECT_EQ(1, exist);
    ED_destroyJSON(handle);
}

TEST(ED_JSON, GetString) {
    auto handle = ED_createJSON("../Examples/test.json", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    int exist;
    auto val = ED_getStringFromJSON(handle, "set1.gain.k", &exist);
    EXPECT_STREQ("2", val);
    EXPECT_EQ(1, exist);
    ED_destroyJSON(handle);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
