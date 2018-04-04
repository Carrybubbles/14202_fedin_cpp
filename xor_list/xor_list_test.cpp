#include <iostream>

#include "deps/include/gtest/gtest.h"
#include "xor_list_test.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
