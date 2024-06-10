
#pragma once
#include <cli/cli.hh>

namespace rift
{
    namespace driver
    {
        class Driver
        {
            public:
                Driver() = default;
                ~Driver() = default;

                /// @brief Parses the command line arguments.
                /// @param argc The number of arguments.
                /// @param argv The arguments.
                /// @return 0 if the program exits successfully.
                int parse(int argc, char **argv);

                /// @brief Runs the driver.
                void init();
            private:
                CLI::App app{"rift"};

                #pragma mark - Command Line Options
                /// @brief The version of the program.
                void version();

                /// @brief Runs the compiler
                void runFile();

                /// @brief Runs the interpreter
                void runPrompt();
        };
    }
}