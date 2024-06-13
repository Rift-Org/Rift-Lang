#include <ast/printer.hh>

#include <vector>
#include <initializer_list>

using namespace rift::ast;
using namespace rift::ast::Expr;
using vec = std::vector<ExprStr*>;
using string = std::string;
using ExprStr = rift::ast::Expr::Expr<string>;

namespace rift
{
    namespace ast
    {
        #pragma mark - Printer

        Printer::Printer()
        {
            visitor = new VisitorPrinter(*this);
        }

        string Printer::print(ExprStr *expr)
        {
            return expr->accept(*this->visitor);
        }

        string Printer::parenthesize(string name, vec expr)
        {
            string result = "(" + name;
            for (auto &e : expr)
                result += " " + e->accept(*this->visitor);
            result += ")";
            return result;
        }

        string Printer::group(vec expr)
        {
            string result = "[";
            for (auto &e : expr)
                result += " " + e->accept(*this->visitor);
            result += "]";
            return result;
        }

        #pragma mark - Visitor

        VisitorPrinter::VisitorPrinter(Printer &printer)
        {
            this->printer = &printer;
        }

        string VisitorPrinter::visit_binary(Binary<string> *expr) const
        {
            vec v;
            v.push_back(expr->left.get());
            v.push_back(expr->right.get());
            return printer->parenthesize(expr->op.lexeme, v);
        }

        string VisitorPrinter::visit_unary(Unary<string> *expr) const
        {
            vec v;
            v.push_back(expr->expr.get());
            return printer->parenthesize(expr->op.lexeme, v);
        }

        string VisitorPrinter::visit_grouping(Grouping<string> *expr) const
        {
            vec v;
            v.push_back(expr->expr.get());
            return printer->group(v);
        }

        string VisitorPrinter::visit_literal(Literal<string> *expr) const
        {
            return expr->value.type().name();
        }
    }
}

