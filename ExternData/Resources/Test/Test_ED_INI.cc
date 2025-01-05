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

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
