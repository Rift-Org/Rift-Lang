#pragma once

#include <string>
#include "expr.hh"

using namespace rift::ast::Expr;
namespace rift
{
    namespace ast
    {
        class Printer : public Expr<std::string>
        {
            public:
                Printer() = default;
                ~Printer() = default;

                /// Prints the given expression as a string.
                /// @param expr The expression to print.
                /// @return The string representation of the expression.
                std::string print(Expr<std::string> *expr);

                /// @brief  Wraps the given expression in parentheses.
                /// @param name The name of the expression.
                /// @param exprs The expressions to wrap.
                /// @return The wrapped expression.
                std::string parenthesize(std::string name, std::vector<Expr> expr);

                # pragma mark - Visitor Methods

                /// @brief Visits a binary expression.
                std::string Printer::visit_binary_expr(Binary<std::string> *expr);

                /// @brief Visits a grouping expression.
                std::string Printer::visit_grouping_expr(Grouping<std::string> *expr);

                /// @brief Visits a literal expression.
                std::string Printer::visit_literal_expr(Literal<std::string> *expr);

                /// @brief Visits a unary expression.
                std::string Printer::visit_unary_expr(Unary<std::string> *expr);
        };
    }
}