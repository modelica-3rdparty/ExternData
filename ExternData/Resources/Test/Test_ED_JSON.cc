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

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
