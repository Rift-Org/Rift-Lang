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
#include <ast/parser.hh>
#include <utils/macros.hh>

namespace rift
{
    namespace ast
    {

        __DEFAULT_FORWARD_NONE_VA(
            StmtVisitor,
            StmtPrint,
            StmtVar
        );

        class Stmt
        {
            public:
                virtual void accept(StmtVisitor &visitor) = 0;
        };
        
        class StmtVisitor
        {
            public:
                virtual void visit_print_stmt(const StmtPrint& expr) const = 0;
                virtual void visit_var_stmt(const StmtVar& expr) const = 0;
        };

        class StmtPrint : public Stmt
        {
            public:
                StmtPrint(GenExpr *expr) : expr(expr) {};
                ~StmtPrint() = default;
                GenExpr *expr;

                void accept(StmtVisitor &visitor) override { visitor.visit_print_stmt(*this); };
        };

        class StmtVar : public Stmt
        {
            public:
                StmtVar(GenExpr *expr) : expr(expr) {};
                ~StmtVar() = default;
                GenExpr *expr;

                void accept(StmtVisitor &visitor) override { visitor.visit_var_stmt(*this); };
        };
    }
}