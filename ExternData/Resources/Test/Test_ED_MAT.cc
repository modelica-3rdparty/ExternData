/* Test_ED_MAT.cc - MAT functions tester
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "../Include/ED_MATFile.h"
#include "Constants.h"
#include <array>
#include <gtest/gtest.h>

namespace
{

class MATFiles
    : public ::testing::TestWithParam<const char *> {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_P(MATFiles, Create) {
    auto handle = ED_createMAT(GetParam(), verbose_on);
    ASSERT_NE(nullptr, handle);
    ED_destroyMAT(handle);
}

TEST_P(MATFiles, GetArray2DDimensions) {
    auto handle = ED_createMAT(GetParam(), verbose_on);
    ASSERT_NE(nullptr, handle);
    int m, n;
    ED_getArray2DDimensionsFromMAT(handle, "table1", &m, &n);
    EXPECT_EQ(m, 3);
    EXPECT_EQ(n, 2);
    ED_destroyMAT(handle);
}

TEST_P(MATFiles, GetDoubleArray2D) {
    auto handle = ED_createMAT(GetParam(), verbose_on);
    ASSERT_NE(nullptr, handle);
    auto table = std::array<double, 6>{};
    ED_getDoubleArray2DFromMAT(handle, "table1", table.data(), 3, 2);
    EXPECT_EQ(table[0], 0);
    EXPECT_EQ(table[1], 0);
    EXPECT_EQ(table[2], 0.5);
    EXPECT_EQ(table[3], 0.25);
    EXPECT_EQ(table[4], 1);
    EXPECT_EQ(table[5], 1);
    ED_destroyMAT(handle);
}

INSTANTIATE_TEST_SUITE_P(
    ED_MAT, MATFiles,
    ::testing::Values(
        "../Examples/test_v4.mat",
        "../Examples/test_v6.mat",
        "../Examples/test_v7.mat",
        "../Examples/test_v7.3.mat")
);

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
