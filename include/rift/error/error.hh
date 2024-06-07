#pragma once

#include <iostream>
#include <string>

namespace rift
{
    namespace error
    {
        static bool errorOccured = false;

        void report(int line, std::string where, std::string msg)
        {
            std::cout << "[line " << line << "] Error" << where << ": " + msg << std::endl;
            errorOccured = true;
        }
    }
}