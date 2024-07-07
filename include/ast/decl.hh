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
        class Decl: public Accept<Tokens>
        {
            public:
                virtual Tokens accept(const Visitor &visitor) const = 0;
                virtual ~Decl() = default;
                friend class Visitor;
                friend class DeclStmt;
                friend class DeclVar;
        };

        class DeclStmt: public Decl
        {
            public:
                DeclStmt(std::unique_ptr<Stmt> stmt) : stmt(std::move(stmt)) {};
                Tokens accept(const Visitor &visitor) const override { return visitor.visit_decl_stmt(*this); }

                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop

                std::unique_ptr<Stmt> stmt;
        };

        class DeclVar: public Decl
        {
            public:
                DeclVar(const Token &identifier): identifier(identifier), expr(nullptr) {};
                DeclVar(const Token &identifier, std::unique_ptr<Expr> expr): identifier(identifier), expr(std::move(expr)) {};
                Tokens accept(const Visitor &visitor) const override { return visitor.visit_decl_var(*this); }

                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop

                const Token& identifier;
                std::unique_ptr<Expr> expr;
        };

        class DeclFunc : public Decl
        {
            public:
                typedef struct {
                    Token name;
                    Tokens params;
                    Environment closure;
                    std::unique_ptr<Block> blk;
                } Func;

                DeclFunc(): func(nullptr) {};
                DeclFunc(std::unique_ptr<Func> func): func(std::move(func)) {};
                ~DeclFunc() = default;

                std::unique_ptr<Func> func;

                Tokens accept(const Visitor &visitor) const override { return visitor.visit_decl_func(*this); };
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };
    }
}