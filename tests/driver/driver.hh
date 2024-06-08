#pragma once
#include <cli.hh>

namespace rift
{
    namespace test 
    {
        class Driver
        {
            public:
                Driver() = default;
                ~Driver() = default;

                /// @fn run
                /// @param argc The number of arguments.
                /// @param argv The arguments.
                /// @return 0 if the program exits successfully
                int run(int argc, char **argv);
            private:
                CLI::App app{"rift"};
                void version();
        };
}   
}
