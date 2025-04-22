/* Test_ED_INI.cc - INI functions tester
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "../Include/ED_INIFile.h"
#include "Constants.h"
#include <gtest/gtest.h>

namespace
{

TEST(ED_INI, Create) {
    auto handle = ED_createINI("../Examples/test.ini", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    ED_destroyINI(handle);
}

TEST(ED_INI, GetDoubleStrict) {
    auto handle = ED_createINI("../Examples/test.ini", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    int exist;
    auto val = ED_getDoubleFromINI(handle, "gain.k", "", strict_on, &exist);
    EXPECT_EQ(1.0, val);
    EXPECT_EQ(1, exist);
    val = ED_getDoubleFromINI(handle, "gain.k", "set2", strict_on, &exist);
    EXPECT_EQ(-2.0, val);
    EXPECT_EQ(1, exist);
    ED_destroyINI(handle);
}

TEST(ED_INI, GetIntStrict) {
    auto handle = ED_createINI("../Examples/test.ini", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    int exist;
    auto val = ED_getIntFromINI(handle, "gain.k", "", strict_on, &exist);
    EXPECT_EQ(1, val);
    EXPECT_EQ(1, exist);
    val = ED_getIntFromINI(handle, "gain.k", "set2", strict_on, &exist);
    EXPECT_EQ(-2, val);
    EXPECT_EQ(1, exist);
    ED_destroyINI(handle);
}

TEST(ED_INI, GetString) {
    auto handle = ED_createINI("../Examples/test.ini", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    int exist;
    auto val = ED_getStringFromINI(handle, "gain.k", "", &exist);
    EXPECT_STREQ("1", val);
    EXPECT_EQ(1, exist);
    val = ED_getStringFromINI(handle, "gain.k", "set2", &exist);
    EXPECT_STREQ("-2", val);
    EXPECT_EQ(1, exist);
    ED_destroyINI(handle);
}

TEST(ED_INI, GetInt) {
    auto handle = ED_createINI("../Examples/sample.tir", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    int exist;
    auto val = ED_getIntFromINI(handle, "USE_MODE", "MODEL", strict_off, &exist);
    EXPECT_EQ(14, val);
    EXPECT_EQ(1, exist);
    ED_destroyINI(handle);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
