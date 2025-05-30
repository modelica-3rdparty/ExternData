/* Test_ED_XLS.cc - XLS functions tester
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "../Include/ED_XLSFile.h"
#include "Constants.h"
#include <array>
#include <gtest/gtest.h>

namespace
{

TEST(ED_XLS, Create) {
    auto handle = ED_createXLS("../Examples/test.xls", "UTF-8", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    ED_destroyXLS(handle);
}

TEST(ED_XLS, GetArray2DDimensions) {
    auto handle = ED_createXLS("../Examples/test.xls", "UTF-8", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    int m, n;
    ED_getArray2DDimensionsFromXLS(handle, "table1", &m, &n);
    EXPECT_EQ(m, 3);
    EXPECT_EQ(n, 2);
    ED_destroyXLS(handle);
}

TEST(ED_XLS, GetDoubleArray2D) {
    auto handle = ED_createXLS("../Examples/test.xls", "UTF-8", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    auto table = std::array<double, 6>{};
    ED_getDoubleArray2DFromXLS(handle, "A1", "table1", table.data(), 3, 2);
    EXPECT_EQ(table[0], 0);
    EXPECT_EQ(table[1], 0);
    EXPECT_EQ(table[2], 0.5);
    EXPECT_EQ(table[3], 0.25);
    EXPECT_EQ(table[4], 1);
    EXPECT_EQ(table[5], 1);
    ED_destroyXLS(handle);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
