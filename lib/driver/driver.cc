
#include <iostream>
#include <fstream> // file stream
#include <sstream> // string stream
#include <vector>
#include <driver/driver.hh>
#include <error/error.hh>

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

        void runFile(char* path)
        {
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

        void runPrompt()
        {
            // left, op, right
            rift::scanner::Token token = rift::scanner::Token(TokenType::PLUS,"+", "", 1);
            rift::ast::Expr::Binary<std::string> expr = rift::ast::Expr::Binary<std::string>(
                std::make_unique<rift::ast::Expr::Unary<std::string>>(
                    rift::scanner::Token(TokenType::MINUS,"-", "", 1),
                    std::make_unique<rift::ast::Expr::Literal<std::string>>("1")
                ),
                rift::scanner::Token(TokenType::PLUS,"+", "", 1),
                std::make_unique<rift::ast::Expr::Literal<std::string>>("2")
            );

            rift::ast::Printer printer = rift::ast::Printer();
            std::cout << printer.print(&expr) << std::endl;

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

        # pragma mark - Driver

        void Driver::run(int argc, char **argv) 
        {
            if (argc > 2) {
                std::cout << "Usage: rift" << std::endl;
            } else if (argc == 2) {
                // file
                runFile(argv[1]);
            } else {
                // prompt
                runPrompt();
            }
        }
    }
}