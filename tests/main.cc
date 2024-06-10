
#include <gtest/gtest.h>

using namespace::testing;

/// @brief The entry of RiftLangTest
/// @return 0 if the program exits successfully
int main(int argc, char **argv)
{
    ::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}