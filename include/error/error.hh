#pragma once

#include <iostream>
#include <string>

namespace rift
{
    namespace error
    {
        [[maybe_unused]] static bool errorOccured = false;

        /// @brief Used to report an error.
        void report(int line, std::string where, std::string msg);
    }
}