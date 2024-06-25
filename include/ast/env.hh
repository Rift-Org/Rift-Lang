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
// #include "../../external/abseil/absl/container/flat_hash_map.h"
// #include <absl/container/flat_hash_map.h>
#include <scanner/tokens.hh>
#include <map>
#include <iostream>

using Token = rift::scanner::Token;
using str_t = std::string;
using strv_t = std::string_view;

namespace rift
{
    namespace ast
    {
        class Environment
        {
            public:
                Environment() = default;
                ~Environment() = default;

                Token getEnv(const str_t& name) const;
                void setEnv(const str_t& name, const Token& value);
            protected:
                // absl::flat_hash_map<str_t, rift::scanner::Token> values;
                std::unordered_map<str_t, rift::scanner::Token> values = {};
        };
    }
}