
#include "driver/driver.hh"

using namespace rift::test;

/// @brief The entry of RiftLangTest
/// @return 0 if the program exits successfully
int main(int argc, char **argv)
{
    Driver driver;
    driver.run(argc, argv);
    return 0;
}