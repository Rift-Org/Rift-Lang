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
#include <unordered_set>
#include <iostream>
#include <error/error.hh>

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
                static Environment& getInstance(bool parser) {
                    static Environment eval_instance;
                    static Environment parser_instance;
                    return parser ? parser_instance : eval_instance;
                }
                /// @brief add a child to the current environment (recursive-checks)
                void addChild() {
                    Environment* curr = this;
                    while(curr->child) {
                        curr = curr->child;
                    }
                    curr->child = new Environment();
                }
                /// @brief remove the child from the current environment
                void removeChild() {
                    Environment* curr = this;
                    Environment prev;
                    while(curr->child != nullptr) {
                        prev = curr;
                        curr = curr->child;
                    }
                    // want it to trap for debug info if we ever hit this case
                    prev.child = nullptr;
                }

                /// @brief clear all enviroments
                /// @note usefull for switching from compile-time to runtime
                void clear(bool parser) {
                    Environment* curr = this;
                    while(curr->child != nullptr) {
                        Environment *tmp = curr->child;
                        curr->child = curr->child->child;
                        delete tmp;
                    }
                    getInstance(parser).values.clear();
                    getInstance(parser).const_keys.clear();
                }

                Environment() : child(nullptr) {}
                Environment(Environment *child) : child(child) {}
                ~Environment() = default;

                Environment(const Environment& other) {
                    values = other.values;
                    const_keys = other.const_keys;
                    child = other.child;
                }

                template <typename T>
                T getEnv(const str_t& name) const;

                template <typename T>
                void setEnv(const str_t& name, T value, bool is_const);

                Environment* at(int dist) {
                    Environment *curr = this;
                    for (int i=0; i<dist; i++) {
                        if (curr == nullptr) rift::error::runTimeError("went past boundary");
                        curr = curr->child;
                    }
                    return curr;
                }

                void printState();
                Environment *child;
            protected:
                // absl::flat_hash_map<str_t, rift::scanner::Token> values;
                std::unordered_map<str_t, rift::scanner::Token> values = {};
                std::unordered_set<str_t> const_keys = {};
        };
    }
}