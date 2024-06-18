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

#pragma once

// #include <iostream>
#include <string>
#include <scanner/tokens.hh>

namespace rift
{
    namespace error
    {
        [[maybe_unused]] static bool errorOccured = false;
        [[maybe_unused]] static bool runtimeErrorOccured = false;

        /// @brief Used to report an error.
        void report(int line, const std::string& where, const std::string& msg, const rift::scanner::Token& token, std::exception e);
        /// @brief Used to report an runtime error.
        void runTimeError(const std::string& msg);
    }
}