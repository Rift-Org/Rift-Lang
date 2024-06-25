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
#include <unordered_map>
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
                static Environment& getInstance() {
                    static Environment instance;
                    return instance;
                }
                Token getEnv(const str_t& name) const;
                void setEnv(const str_t& name, const Token& value);
                void printState();
            protected:
                // absl::flat_hash_map<str_t, rift::scanner::Token> values;
                std::unordered_map<str_t, rift::scanner::Token> values = {};
                Environment(Environment &other) = delete;
                Environment &operator=(const Environment &other) = delete;
            
            private:
                Environment() = default;
                ~Environment() = default;
        };
    }
}