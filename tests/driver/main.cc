
#include "driver/driver.hh"

using namespace rift::driver;

namespace rift
{
    namespace test
    {
        int main(int argc, char **argv)
        {
            Driver driver;
            driver.run(argc, argv);
            return 0;
        }
    }
}