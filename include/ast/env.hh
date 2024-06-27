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
                /// @note top level
                static Environment& getInstance() {
                    static Environment instance;
                    return instance;
                }
                /// @brief add a child to the current environment (recursive-checks)
                static void addChild() {
                    Environment &curr = getInstance();
                    while(curr.child != nullptr) {
                        curr = curr.child;
                    }
                    curr.child = new Environment();
                }
                /// @brief remove the child from the current environment
                static void removeChild() {
                    Environment prev;
                    Environment curr = getInstance();
                    while(curr.child != nullptr) {
                        prev = curr;
                        curr = *curr.child;
                    }
                    prev.child = nullptr; // remove the child
                }

                /// @brief clear all enviroments
                /// @note usefull for switching from compile-time to runtime
                static void clear() {
                    Environment &curr = getInstance();
                    while(curr.child != nullptr) {
                        Environment *tmp = curr.child;
                        curr.child = curr.child->child;
                        delete tmp;
                    }
                    getInstance().values.clear();
                }

                Environment() : child(nullptr) {}
                Environment(Environment *child) : child(child) {}
                ~Environment() = default;
                Token getEnv(const str_t& name) const;
                void setEnv(const str_t& name, const Token& value);
                void printState();
                Environment *child;
            protected:
                // absl::flat_hash_map<str_t, rift::scanner::Token> values;
                std::unordered_map<str_t, rift::scanner::Token> values = {};
        };
    }
}