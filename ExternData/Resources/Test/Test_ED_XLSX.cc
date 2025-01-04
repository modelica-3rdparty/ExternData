#include "../Include/ED_XLSXFile.h"
#include "Constants.h"
#include <gtest/gtest.h>

namespace
{

TEST(ED_XLSX, Create) {
    auto handle = ED_createXLSX("../Examples/test.xlsx", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    ED_destroyXLSX(handle);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
