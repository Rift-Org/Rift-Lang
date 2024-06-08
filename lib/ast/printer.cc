#include "../../include/ast/printer.hh"

#include <vector>
#include <initializer_list>

using namespace rift::ast;
using namespace rift::ast::Expr;

namespace rift
{
    namespace ast
    {
        std::string Printer::print(Expr<std::string> *expr)
        {
            return expr->accept(this);
        }

        std::string Printer::parenthesize(std::string name, std::vector<Expr> expr)
        {
            std::string result = "(" + name;
            for (auto &e : expr)
                result += " " + e.accept(this);
            result += ")";
            return result;
        }

        std::string Printer::visit_binary_expr(Binary<std::string> *expr)
        {
            return parenthesize(expr->op.lexeme, std::vector<Expr>{expr->left.get(), expr->right.get()});
        }

        std::string Printer::visit_unary_expr(Unary<std::string> *expr)
        {
            return parenthesize(expr->op.lexeme, std::vector<Expr>{expr->expr.get()});
        }

        std::string Printer::visit_grouping_expr(Grouping<std::string> *expr)
        {
            return parenthesize("group", std::vector<Expr>{expr->expr.get()});
        }

        std::string Printer::visit_literal_expr(Literal<std::string> *expr)
        {
            return expr->value;
        }
    }
}

