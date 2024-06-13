
#include <iostream>
#include <fstream> // file stream
#include <sstream> // string stream
#include <vector>
#include <driver/driver.hh>
#include <error/error.hh>

// ////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                      ///
///    ██████████████████████████████████████████████████████████████████████████████████████████████    ///
///   ████████▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀██████   ///
///   ██████▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▀█████   ///
///   ████▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▀████   ///
///   ██▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▀██   ///
///   █░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░█   ///
///   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░   ///
///   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
///   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░   ///
///                                                                                                      ///
///     ██████████████████████████████████████████████████████████████████████████████████████████████    ///
///    ████████████████████████████████████████████████████████████████████████████████████████████████   ///
///   ██████   ▀██████████████████▀   ▀██████████████████▀   ▀██████████████████▀   ▀█████████████████   ///
///   ████▀   ░░░▀██████████████▀   ░░░▀██████████████▀   ░░░▀██████████████▀   ░░░▀██████████████████   ///
///   ██▀   ░░░░░░░▀██████████▀   ░░░░░░░▀██████████▀   ░░░░░░░▀██████████▀   ░░░░░░░▀████████████████   ///
///   █   ░░░░░░░░░░░▀██████▀   ░░░░░░░░░░░▀██████▀   ░░░░░░░░░░░▀██████▀   ░░░░░░░░░░░▀██████████████   ///
///   ░░░░░░░░░░░░░░░░░████░░░░░░░░░░░░░░░░░░░███░░░░░░░░░░░░░░░░░░███░░░░░░░░░░░░░░░░░░▀█████████████   ///
///   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▀████████████   ///
///     

// REMOVE
#include <ast/expr.hh>
#include <ast/printer.hh>
#include <string>

using namespace rift::error;
using namespace rift::scanner;

namespace rift
{
    namespace driver
    {
        # pragma mark - Driver Tools

        void run(std::string lines)
        {
            std::istringstream scanner(lines);
            std::cout << lines << std::endl;
        }

        void Driver::runFile()
        {
            CLI::results_t args = app.remaining();
            std::string path;

            if (!args.empty()) {
                for (auto arg : args) path = arg;
            } else {
                std::cout << "File flag provided without arguments." << std::endl;
                return;
            }

            std::ifstream file(std::string(path), std::ios::binary | std::ios::ate);

            if (file) {
                std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);

                std::vector<char> buffer(size);
                if (file.read(buffer.data(), size)) {
                    run(buffer.data());                
                    errorOccured = false; // reset error
                }
            }   
        }

        void Driver::runPrompt()
        {
            while(true)
            {
                std::string input = "";
                std::cout << "> ";
                std::cin >> input;
                if (input.empty()) break;
                run(input);
                errorOccured = false; // reset error
            }
        }

        void Driver::version()
        {
            std::string versionString = std::string("Rift version ") + "0.0.1" + "\n\n(c) Aman 2024";
            app.set_version_flag("--version,-v", versionString);
            app.footer("(c) Aman 2024");
        }

        # pragma mark - Driver

        int Driver::parse(int argc, char **argv) 
        {
            try {
                app.parse(argc, argv);
            } catch (const CLI::ParseError &parseError) {
                return app.exit(parseError);
            }
            // return 0;
            // if (argc > 2) {
            //     std::cout << "Usage: rift" << std::endl;
            // } else if (argc == 2) {
            //     // file
            //     runFile(argv[1]);
            // } else {
            //     // prompt
            //     runPrompt();
            // }
            return 0;
        }

        void Driver::init()
        {
            app.add_flag_callback("--version,-v", [this]() { version(); }, "Print the version of the Rift language");
            app.add_flag_callback("--prompt,-p", [this]() { runPrompt(); }, "Run the Rift prompt");
            app.add_flag_callback("--file,-f", [this]() { runFile(); }, "Run a Rift source file");
        }
    }
}