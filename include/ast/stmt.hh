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

namespace rift
{
    namespace ast
    {

        class StmtVisitor;

        class Stmt
        {
            public:
                virtual void accept(StmtVisitor &visitor) = 0;
        };
        
        class StmtVisitor
        {

            public:
                void visit_expression_stmt(const Stmt& expr) const;
                void visit_print_stmt(const Stmt& expr) const;
                void visit_var_stmt(const Stmt& expr) const;
        };

        using VoidExpr = rift::ast::Expr::Expr<void>;
        class Expression : public Stmt
        {
            public:
                Expression(VoidExpr *expr) : expr(expr) {};
                ~Expression() = default;
                VoidExpr *expr;

                void accept(StmtVisitor &visitor) override { visitor.visit_expression_stmt(*this); };

        };

        class Print : public Stmt
        {
            public:
                Print(VoidExpr *expr) : expr(expr) {};
                ~Print() = default;
                VoidExpr *expr;

                void accept(StmtVisitor &visitor) override { visitor.visit_print_stmt(*this); };
        };
    }
}