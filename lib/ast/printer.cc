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

namespace rift
{
    namespace ast
    {
        #pragma mark - Printer

        Printer::Printer()
        {
            // this->visitor = std::unique_ptr<Visitor>(new Visitor());
            this->visitor = std::make_unique<const Visitor>();
        }

        string Printer::print(Expr *expr) const
        {
            return expr->accept_printer(*this->visitor);
        }

        string Printer::parenthesize(string name, vec expr) const
        {
            string result = "(" + name;
            for (auto &e : expr)
                result += " " + e->accept_printer(*this->visitor);
            result += ")";
            return result;
        }

        string Printer::group(vec expr) const
        {
            string result = "[";
            for (auto &e : expr)
                result += " " + e->accept_printer(*this->visitor);
            result += "]";
            return result;
        }

        #pragma mark - Visitor

        string Visitor::print_binary(const Binary& expr) const
        {
            vec v;
            v.push_back(expr.left.get());
            v.push_back(expr.right.get());
            return printer->parenthesize(expr.op.lexeme, v);
        }

        string Visitor::print_unary(const Unary& expr) const
        {
            vec v;
            v.push_back(expr.expr.get());
            return printer->parenthesize(expr.op.lexeme, v);
        }

        string Visitor::print_grouping(const Grouping& expr) const
        {
            vec v;
            v.push_back(expr.expr.get());
            return printer->group(v);
        }

        string Visitor::print_literal(const Literal& expr) const
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

