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

        void run(std::string lines, bool interactive)
        {
            std::istringstream scanner(lines);
            std::shared_ptr<std::vector<char>> source = std::make_shared<std::vector<char>>(std::istreambuf_iterator<char>(scanner), std::istreambuf_iterator<char>()); 
            
            Scanner riftScanner(source);
            riftScanner.scan_source();

            std::shared_ptr<std::vector<Token>> tokensPtr = std::make_shared<std::vector<Token>>(riftScanner.tokens);
            Parser riftParser(tokensPtr);
            std::unique_ptr<Program<Tokens>> statements = riftParser.parse(); 

            Eval riftEvaluator;
            riftEvaluator.evaluate(*statements, interactive);
            rift::ast::Environment::getInstance(true).printState();
            rift::ast::Environment::getInstance(false).printState();
        }

        void Driver::runFile(std::string path)
        {
            std::ifstream file(std::string(path), std::ios::binary | std::ios::ate);
            if (file) {
                std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);
                std::vector<char> buffer(size);
                if (file.read(buffer.data(), size)) {
                    run(buffer.data(), false);
                    if (errorOccured) exit(42);
                    if (runtimeErrorOccured) exit(69);
                }
            }   
        }

        void Driver::runPrompt()
        {
            rl_bind_key('\t', rl_complete);
            while(true) {
                char* input = readline("🦊 ＞ ");
                if (input == nullptr) break;
                add_history(input);

                run(input, true);
                
                // reset
                errorOccured = false;
                free(input);
            }
        }

        void Driver::version()
        {
            std::cout << "Rift version 0.0.1" << std::endl;
            std::cout << "(c) Rift-Team 2024" << std::endl;
            exit(1);
        }

        void Driver::help()
        {
            std::cout << "Rift Lang" << std::endl;
            std::cout << "Usage: rift [options] [file]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  [file]            Run the compiler" << std::endl;
            std::cout << "  -h, --help        Display this information" << std::endl;
            std::cout << "  -v, --version     Display the version of the program" << std::endl;
            std::cout << "  -i, --interactive Run the interpreter" << std::endl;
            exit(1);
        }

        # pragma mark - Driver

        int Driver::parse(int argc, char **argv) 
        {
            bool passed = false;
            int opt = 0, idx = 0;
            while ((opt = getopt_long(argc, argv, "", opts, &idx)) != -1) {
                passed = true;
                switch (opt) {
                    case 'h':
                        help();
                        exit(1);
                    case 'v':
                        version();
                    case 'i':
                        runPrompt();
                    default:
                        std::cout << "Invalid option" << std::endl;
                        break;

                }
            }

            if (!passed) {
                if (optind < argc) {
                    runFile(argv[optind]);
                } else {
                    help();
                }
            }

            return 0;
        }
    }
}