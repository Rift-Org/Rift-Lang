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
#include "arithmetic.hh"


using any = std::any;
using string = std::string;
using Token = rift::scanner::Token;
using TokenType = rift::scanner::TokenType;

namespace rift
{
    /// @brief evaluates token for truthiness
    extern bool truthy(const Token& tok);
    /// @brief evaluates tokens for equality
    extern bool equal(const Token& tl, const Token& tr);
    /// @brief parses token's literal to check if its a number
    extern bool isNumber(const Token& tok);
    /// @brief parses token's literal to check if its a string
    extern bool isString(const Token& tok);
    /// @brief casts token to a number
    extern any castNumber(const Token& tok);
    /// @brief casts token to a number then to a string for printing
    extern std::string castNumberString(const any& val, bool err = true);
    /// @brief casts token to a string
    extern std::string castString(const Token& tok, bool err = true);
    /// @brief casts token to a string for printing
    extern std::string castAnyString(const Token& token);
}