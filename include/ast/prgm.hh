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
        __DEFAULT_FORWARD_VA(
            Program
        )

        template <typename T>
        class ProgramVisitor
        {
            public:
                ProgramVisitor() = default;
                virtual ~ProgramVisitor() = default;
                virtual T visit_program(const Program<T>& prgm) const = 0;
        };

        template <typename T>
        class Program
        {
            public:
                using vec_t = std::vector<std::unique_ptr<Decl<Token>>>;
                // Program(vec_t decls) : decls(std::move(decls)) {}
                Program(vec_t&& decls): decls(std::move(decls)) {}
                virtual ~Program() = default;
                friend class Eval;

                T accept(const ProgramVisitor<T> &visitor) { return visitor.visit_program(*this); }

            protected:
                vec_t decls = {};
        };
    }
}