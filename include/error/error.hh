#pragma once

// #include <iostream>
#include <string>
#include <scanner/tokens.hh>

namespace rift
{
    namespace error
    {
        [[maybe_unused]] static bool errorOccured = false;

        /// @brief Used to report an error.
        void report(int line, const std::string& where, const std::string& msg, const rift::scanner::Token& token);
    }
}