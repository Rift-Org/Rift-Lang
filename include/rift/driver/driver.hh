
#pragma once

namespace rift
{
    namespace driver
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
        };
    }
}