/////////////////////////////////////////////////////////////
///                                                       ///
///     ██████╗ ██╗███████╗████████╗                      ///
///     ██╔══██╗██║██╔════╝╚══██╔══╝                      ///
///     ██████╔╝██║█████╗     ██║                         ///
///     ██╔══██╗██║██╔══╝     ██║                         ///
///     ██║  ██║██║██║        ██║                         ///
///     ╚═╝  ╚═╝╚═╝╚═╝        ╚═╝                         ///
///     * RIFT CORE - The official compiler for Rift.     ///
///     * Copyright (c) 2024, Rift-Org                    ///
///     * License terms may be found in the LICENSE file. ///
///                                                       ///
/////////////////////////////////////////////////////////////


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

            private:
                CLI::App app{"rift"};

                #pragma mark - Command Line Options
                /// @brief The version of the program.
                void version();

                /// @brief Runs the compiler
                void runFile(std::string path);

                /// @brief Runs the interpreter
                void runPrompt();
        };
    }
}