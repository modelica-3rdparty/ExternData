/* Test_ED_XML2.cc - XML2 functions tester
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "../Include/ED_XML2File.h"
#include "Constants.h"
#include <gtest/gtest.h>

namespace
{

TEST(ED_XML2, Create) {
    const char* ns = "";
    auto handle = ED_createXML2("../Examples/test.xml", &ns, 0, verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    ED_destroyXML2(handle);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
