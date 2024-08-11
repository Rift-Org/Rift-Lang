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
#include <ast/stmt.hh>
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
            public:
                DeclVisitor() = default;
                virtual ~DeclVisitor() = default;
                virtual T visit_decl_stmt(const DeclStmt<T>& decl) const = 0;
                virtual T visit_decl_var(const DeclVar<T>& decl) const = 0;
                virtual T visit_decl_func(const DeclFunc<T>& decl) const = 0;
        };

        /// @class Decl
        /// @brief Declarations acceptor
        template <typename T>
        class Decl
        {
            public:
                virtual T accept(const DeclVisitor<T> &visitor) const = 0;
                virtual ~Decl() = default;
        };

        template <typename T>
        class DeclStmt: public Decl<T>
        {
            public:
                DeclStmt(std::unique_ptr<Stmt<void>> stmt) : stmt(std::move(stmt)) {};
                T accept(const DeclVisitor<T> &visitor) const override { return visitor.visit_decl_stmt(*this); }

                std::unique_ptr<Stmt<void>> stmt;
        };

        template <typename T>
        class DeclVar: public Decl<T>
        {
            public:
                DeclVar(const Token &identifier): identifier(identifier), expr(nullptr) {};
                DeclVar(const Token &identifier, std::unique_ptr<Expr<Token>> expr): identifier(identifier), expr(std::move(expr)) {};
                T accept(const DeclVisitor<T> &visitor) const override { return visitor.visit_decl_var(*this); }

                const Token& identifier;
                std::unique_ptr<Expr<Token>> expr;
        };

        template <typename T>
        class DeclFunc : public Decl<T>
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

                T accept(const DeclVisitor<T> &visitor) const override { return visitor.visit_decl_func(*this); };
        };
    }
}