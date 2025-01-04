#include "../Include/ED_MATFile.h"
#include "Constants.h"
#include <gtest/gtest.h>

namespace
{

TEST(ED_MAT, Create) {
    auto handle = ED_createMAT("../Examples/test_v7.3.mat", verbose_on);
    ASSERT_NE(nullptr, handle);
    ED_destroyMAT(handle);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
