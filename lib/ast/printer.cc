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

        /// only one printer available (alleivate dependencies<workaround>)
        static Printer* printer = new Printer();

        #pragma mark - Printer

        Printer::Printer()
        {
        }

        string Printer::print(Expr<string> *expr) const
        {
            return expr->accept(*this);
        }

        string Printer::parenthesize(string name, vec expr) const
        {
            string result = "(" + name;
            for (auto &e : expr)
                result += " " + e->accept(*this);
            result += ")";
            return result;
        }

        string Printer::group(vec expr) const
        {
            string result = "[";
            for (auto &e : expr)
                result += " " + e->accept(*this);
            result += "]";
            return result;
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - Expressions
        ////////////////////////////////////////////////////////////////////////

        string Printer::visit_binary(const Binary<string>& expr) const
        {
            Printer::vec v;
            v.push_back(expr.left.get());
            v.push_back(expr.right.get());
            return parenthesize(expr.op.lexeme, v);
        }

        string Printer::visit_unary(const Unary<string>& expr) const
        {
            Printer::vec v;
            v.push_back(expr.expr.get());
            return printer->parenthesize(expr.op.lexeme, v);
        }

        string Printer::visit_grouping(const Grouping<string>& expr) const
        {
            Printer::vec v;
            v.push_back(expr.expr.get());
            return printer->group(v);
        }

        string Printer::visit_var_expr(const VarExpr<string>& expr) const 
        {
            Token val = expr.value;
            std::any literal = val.getLiteral();
            return "";
        }

        string Printer::visit_literal(const Literal<string>& expr) const
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

        string Printer::visit_ternary(const Ternary<string>& expr) const
        {
            Printer::vec v;
            v.push_back(expr.condition.get());
            v.push_back(expr.left.get());
            v.push_back(expr.right.get());
            return parenthesize("?:", v);
        }

        string Printer::visit_call(const Call<string>& expr) const
        {
            return "";
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - Statements
        ////////////////////////////////////////////////////////////////////////

        string Printer::visit_expr_stmt(const StmtExpr<string>& stmt) const
        {
            return parenthesize("expr_stmt", {stmt.expr.get()});
        }

        string Printer::visit_print_stmt(const StmtPrint<string>& stmt) const
        {
            return parenthesize("print", {stmt.expr.get()});
        }

        string Printer::visit_if_stmt(const StmtIf<string>& stmt) const
        {
            return "";
        }

        string Printer::visit_return_stmt(const StmtReturn<string>& stmt) const
        {
            return "";
        }

        string Printer::visit_block_stmt(const Block<string>& block) const
        {
            return "";
        }

        string Printer::visit_for_stmt(const For<string>& decl) const
        {
            return "";
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - Declarations
        ////////////////////////////////////////////////////////////////////////

        string Printer::visit_decl_stmt(const DeclStmt<string>& decl) const
        {
            return "";
        }

        string Printer::visit_decl_var(const DeclVar<string>& decl) const
        {
            return "";
        }

        string Printer::visit_decl_func(const DeclFunc<string>& decl) const
        {
            return "";
        }
    }
}

