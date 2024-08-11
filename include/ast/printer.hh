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

#include <string>
#include <vector>
#include <ast/expr.hh>
#include <ast/stmt.hh>
#include <ast/decl.hh>
#include <ast/prgm.hh>

/// not in use now
/// need futher re-evaluation
/// an idea is to accept the usual Token-void and have
// an internal variable keeping tracking of the string
// the template meta is too far now...
namespace rift
{
    namespace ast
    {
        using string = std::string;
        // using ExprStr  = rift::ast::Expr<string>;
        // using VisitStr = rift::ast::Visitor<string>;

        /// @class Printer
        /// @brief This class is used to print the expression.
        class Printer: public ExprVisitor<string>, public StmtVisitor<string>, 
                       public DeclVisitor<string>, public ProgramVisitor<string>
        {
            public:
                using vec = std::vector<Expr<string>*>;
                Printer() = default;
                ~Printer() = default;

                /// Prints the given expression string.
                /// @param expr The expression to print.
                /// @return string representation of the expression.
                string print(Expr<string> *expr) const;

                // expressions
                string visit_assign(const Assign<string>& expr) const override;
                string visit_binary(const Binary<string>& expr) const override;
                string visit_grouping(const Grouping<string>& expr) const override;
                string visit_literal(const Literal<string>& expr) const override;
                string visit_var_expr(const VarExpr<string>& expr) const override;
                string visit_unary(const Unary<string>& expr) const override;
                string visit_ternary(const Ternary<string>& expr) const override;
                string visit_call(const Call<string>& expr) const override;

                // statements
                string visit_expr_stmt(const StmtExpr<string>& stmt) const override;
                string visit_print_stmt(const StmtPrint<string>& stmt) const override;
                string visit_if_stmt(const StmtIf<string>& stmt) const override;
                string visit_return_stmt(const StmtReturn<string>& stmt) const override;
                string visit_block_stmt(const Block<string>& block) const override;
                string visit_for_stmt(const For<string>& decl) const override;

                // declarations
                string visit_decl_stmt(const DeclStmt<string>& decl) const override;
                string visit_decl_var(const DeclVar<string>& decl) const override;
                string visit_decl_func(const DeclFunc<string>& decl) const override;

                // program
                string visit_program(const Program<string>& prgm) const override;
            protected:

                /// @brief  Wraps the given expression in parentheses.
                /// @param name The name of the expression.
                /// @param exprs The expressions to wrap.
                /// @return The wrapped expression.
                string parenthesize(string name, vec expr) const;

                /// @brief  Wraps the given expression in square brackets.
                /// @param exprs The expressions to wrap.
                /// @return The wrapped expression.
                string group(vec expr) const;
        };
    }
}