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

#include <memory>
#include <scanner/tokens.hh>
#include <ast/grmr.hh>

using Tokens = std::vector<rift::scanner::Token>;
namespace rift
{
    namespace ast
    {
        class Stmt;

        class Program : public Accept<Tokens>
        {
            public:
                Program(vec_prog statements) : statements(std::move(statements)) {}
                virtual ~Program() = default;
                friend class Visitor;

                Tokens accept(const Visitor &visitor) const override { return visitor.visit_program(*this); }

                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                std::string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop

            protected:
                vec_prog statements = nullptr;
        };
    }
}