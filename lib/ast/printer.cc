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

        string VisitorPrinter::visit_binary(const Binary<string>& expr) const
        {
            vec v;
            v.push_back(expr.left.get());
            v.push_back(expr.right.get());
            return printer->parenthesize(expr.op.lexeme, v);
        }

        string VisitorPrinter::visit_unary(const Unary<string>& expr) const
        {
            vec v;
            v.push_back(expr.expr.get());
            return printer->parenthesize(expr.op.lexeme, v);
        }

        string VisitorPrinter::visit_grouping(const Grouping<string>& expr) const
        {
            vec v;
            v.push_back(expr.expr.get());
            return printer->group(v);
        }

        string VisitorPrinter::visit_literal(const Literal<string>& expr) const
        {
            Token val = expr.value;
            std::any literal = val.getLiteral();
            if (literal.type() == typeid(std::string))
                return std::any_cast<std::string>(literal);
            else if (literal.type() == typeid(double))
                return std::to_string(std::any_cast<double>(literal));
            else if (literal.type() == typeid(int))
                return std::to_string(std::any_cast<int>(literal));
            else if (literal.type() == typeid(char**))
                return "";
            else if (literal.type() == typeid(char*))
                return std::string(std::any_cast<char*>(literal));
            else if (literal.type() == typeid(char))
                return std::string(1, std::any_cast<char>(literal));
            else if (literal.type() == typeid(const char*))
                return std::string(std::any_cast<const char*>(literal));

            return "";
        }
    }
}

