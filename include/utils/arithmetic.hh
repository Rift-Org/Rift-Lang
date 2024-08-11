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

#include <any>
#include <string>
#include <scanner/tokens.hh>
#include <utils/macros.hh>


using any = std::any;
using string = std::string;
using Token = rift::scanner::Token;
using TokenType = rift::scanner::TokenType;

namespace rift
{
    /// @brief evaluates the given values with operation
    extern any any_arithmetic(any left, any right, const Token& op);
}