#pragma once

#include <iostream>
#include <string>

namespace rift
{
    namespace error
    {
        static bool errorOccured = false;

        /// @brief Used to report an error.
        void report(int line, std::string where, std::string msg);
    }
}