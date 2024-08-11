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
#include <error/error.hh>
namespace rift
{
    namespace error
    {

        void report(int line, std::string_view where, std::string msg, const rift::scanner::Token& token, std::exception e) {
            std::cout << "🛑 [line " << line << "] Error " << where << ": " << msg;
            if (token.type != rift::scanner::TokenType::EOFF) {
                std::cout << " (token: " << token.to_string();
            }
            std::cout << ")" << std::endl;
            errorOccured = true;

            if (e.what() != nullptr) {
                exit(1);
            }
        }

        void runTimeError(std::string_view msg)
        {
            std::cout << "⛔️ Runtime Error: " << msg << std::endl;
            runtimeErrorOccured = true;
            exit(1);
        }
    }
}