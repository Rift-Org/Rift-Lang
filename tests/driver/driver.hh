#pragma once
#include "../../external/CLI11.hpp"

namespace rift
{
    namespace test 
    {
        class Driver
        {
            public:
                Driver() = default;
                ~Driver() = default;

                /// @brief  Runs the driver.
                /// @param argc The number of arguments.
                /// @param argv The arguments.
                void run(int argc, char **argv);
            private:
                CLI::App app{"rift"};
                void version();
        };
}   
}
