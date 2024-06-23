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
#include <ast/parser.hh>
#include <utils/macros.hh>

namespace rift
{
    namespace ast
    {

        // i fucking hate c++ macros...
        // template <typename T = void>
        // class StmtVisitor;

        __DEFAULT_FORWARD_NONE_VA(
            StmtExpr,
            StmtPrint,
            StmtVar
        );


        class Stmt: public rift::ast::Accept<void>
        {
            public:
                virtual void accept(const Visitor &visitor) const = 0;
                virtual ~Stmt() = default;
        };
        
        /// @class StmtExpr
        /// @brief Represents an expression statement
        /// @details expressions are more common that thought of
        ///          For example: function calls `something();`
        /// @code 
        /// // Code from MDN
        /// // Using control flow statements
        ///    function range(start, end) {
        ///     if (start > end) {
        ///       [start, end] = [end, start];
        ///     }
        /// // Using expression statements
        /// function range2(start, end) {
        ///   start > end && ([start, end] = [end, start]);
        /// }
        class StmtExpr: public Stmt
        {
            public:
                StmtExpr(Expr *expr) : expr(expr) {};
                ~StmtExpr() = default;
                std::unique_ptr<Expr> expr;

                void accept(const Visitor &visitor) const override { visitor.visit_expr_stmt(*this); };
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { "unimplemented"; }
        };

        class StmtPrint : public Stmt
        {
            public:
                StmtPrint(Expr *expr) : expr(expr) {};
                ~StmtPrint() = default;
                std::unique_ptr<Expr> expr;

                void accept(const Visitor &visitor) const override { visitor.visit_print_stmt(*this); };
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { "unimplemented"; }
        };

        class StmtVar : public Stmt
        {
            public:
                StmtVar(Expr *expr) : expr(expr) {};
                ~StmtVar() = default;
                std::unique_ptr<Expr> expr;

                void accept(const Visitor &visitor) const override { visitor.visit_var_stmt(*this); };
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { "unimplemented"; }
        };
    }
}