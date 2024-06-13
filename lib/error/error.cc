#include <iostream>
#include <error/error.hh>
namespace rift
{
    namespace error
    {
        /// @brief Used to report an error.
        void report(int line, const std::string& where, const std::string& msg, const rift::scanner::Token& token) {
            std::cout << "[line " << line << "] Error " << where << ": " << msg;
            if (token.type != rift::scanner::TokenType::EOFF) {
                std::cout << " (token: " << token.to_string();
            }
            std::cout << ")" << std::endl;
            errorOccured = true;
        }
    }
}