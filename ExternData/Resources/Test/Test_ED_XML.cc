#include "../Include/ED_XMLFile.h"
#include "Constants.h"
#include <gtest/gtest.h>

namespace
{

TEST(ED_XML, Create) {
    auto handle = ED_createXML("../Examples/test.xml", verbose_on, log_debug);
    ASSERT_NE(nullptr, handle);
    ED_destroyXML(handle);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
