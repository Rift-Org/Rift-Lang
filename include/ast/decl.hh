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

#include <ast/expr.hh>
#include <ast/grmr.hh>
#include <ast/env.hh>

namespace rift
{
    namespace ast
    {
        __DEFAULT_FORWARD_VA(
            DeclStmt,
            DeclVar,
            DeclFunc
        );

        /// @class Visitor
        /// @brief Visitor pattern for expressions
        template <typename T>
        class DeclVisitor
        {
            virtual T visit_decl_stmt(const DeclStmt<T>& decl) const;
            virtual T visit_decl_var(const DeclVar<T>& decl) const;
            virtual T visit_decl_func(const DeclFunc<T>& decl) const;
        };

        /// @class Decl
        /// @brief Declarations acceptor
        template <typename T>
        class Decl
        {
            public:
                virtual T accept(const Visitor &visitor) const = 0;
                virtual ~Decl() = default;
        };

        template <typename T>
        class DeclStmt: public Decl
        {
            public:
                DeclStmt(std::unique_ptr<Stmt<void>> stmt) : stmt(std::move(stmt)) {};
                T accept(const Visitor &visitor) const override { return visitor.visit_decl_stmt(*this); }

                std::unique_ptr<Stmt<void>> stmt;
        };

        template <typename T>
        class DeclVar: public Decl
        {
            public:
                DeclVar(const Token &identifier): identifier(identifier), expr(nullptr) {};
                DeclVar(const Token &identifier, std::unique_ptr<Expr<Token>> expr): identifier(identifier), expr(std::move(expr)) {};
                T accept(const Visitor &visitor) const override { return visitor.visit_decl_var(*this); }

                const Token& identifier;
                std::unique_ptr<Expr<Token>> expr;
        };

        template <typename T>
        class DeclFunc : public Decl
        {
            public:
                typedef struct {
                    Token name;
                    Tokens params;
                    Environment closure;
                    std::unique_ptr<Block<void>> blk;
                } Func;

                DeclFunc(): func(nullptr) {};
                DeclFunc(std::unique_ptr<Func> func): func(std::move(func)) {};
                ~DeclFunc() = default;

                std::unique_ptr<Func> func;

                T accept(const Visitor &visitor) const override { return visitor.visit_decl_func(*this); };
        };
    }
}