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


#include <iostream>
#include <fstream> // file stream
#include <sstream> // string stream
#include <vector>
#include <driver/driver.hh>
#include <error/error.hh>


#include <ast/expr.hh>
#include <ast/parser.hh>
#include <ast/printer.hh>
#include <scanner/scanner.hh>
#include <ast/eval.hh>
#include <string>

using namespace rift::error;
using namespace rift::scanner;
using namespace rift::ast;

namespace rift
{
    namespace driver
    {
        # pragma mark - Driver Tools

        void run(std::string lines)
        {
            std::istringstream scanner(lines);
            std::shared_ptr<std::vector<char>> source = std::make_shared<std::vector<char>>(std::istreambuf_iterator<char>(scanner), std::istreambuf_iterator<char>()); 
            
            Scanner riftScanner(source);
            riftScanner.scan_source();

            std::shared_ptr<std::vector<Token>> tokensPtr = std::make_shared<std::vector<Token>>(riftScanner.tokens);
            Parser riftParser(tokensPtr);
            std::unique_ptr<GenExpr> statements = riftParser.parse(); 

            Eval riftEvaluator;
            riftEvaluator.evaluate(*statements);
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
                    if (errorOccured) exit(42);
                    if (runtimeErrorOccured) exit(69);
                    // reset
                    errorOccured = false; 
                    runtimeErrorOccured = false;
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
            runPrompt();
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