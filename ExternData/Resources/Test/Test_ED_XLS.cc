#include "../Include/ED_XLSFile.h"
#include "Constants.h"
#include <gtest/gtest.h>

namespace
{

TEST(ED_XLS, Create) {
    auto handle = ED_createXLS("../Examples/test.xls", "UTF-8", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    ED_destroyXLS(handle);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
