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
// #include <ast/grmr.hh>
#include <ast/expr.hh>

/// not in use now
/// need futher re-evaluation
/// an idea is to accept the usual Token-void and have
// an internal variable keeping tracking of the string
// the template meta is too far now...
namespace rift
{
    namespace ast
    {
        using vec = std::vector<Expr*>;
        using string = std::string;
        // using ExprStr  = rift::ast::Expr<string>;
        // using VisitStr = rift::ast::Visitor<string>;

        /// @class Printer
        /// @brief This class is used to print the expression.
        class Printer
        {
            public:
                Printer();
                ~Printer() = default;
                friend class Visitor;

                /// Prints the given expression string.
                /// @param expr The expression to print.
                /// @return string representation of the expression.
                string print(Expr *expr) const;

                std::unique_ptr<const Visitor> visitor;
                
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

        /// only one printer available (alleivate dependencies<workaround>)
        static Printer* printer = new Printer();
    }
}