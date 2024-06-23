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

#include <scanner/tokens.hh>
#include <utils/macros.hh>

using Token = rift::scanner::Token;
using string = std::string;

namespace rift
{
    namespace ast
    {
        ///         RIFT Grammar
        ///         ------------
        /// program        → statement * EOF
        /// statement      → exprStmt | printStmt ";"
        /// printStmt      → "print" "(" expression ");" 
        /// exprStmt       → expression ";"
        /// expression     → equality ";"
        /// equality       → comparison ( ( "!=" | "==" ) comparison )* ";"
        /// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ";"
        /// term           → factor ( ( "-" | "+" ) factor )* ";"
        /// factor         → unary ( ( "/" | "*" ) unary )* ";"
        /// unary          → ( "!" | "-" ) unary | primary ";"
        /// primary        → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ";"
        __DEFAULT_FORWARD_NONE_VA(
            Binary,
            Grouping,
            Literal,
            Unary,
            Printer
        )

        __DEFAULT_FORWARD_NONE_VA(StmtPrint, StmtExpr, StmtVar)
        /// @class Visitor 
        /// @brief implementation of the statements and expressions
        class Visitor
        {
            public:
                /* Evaluator */
                    /* expr */
                    virtual Token visit_binary(const Binary& expr) const;
                    virtual Token visit_grouping(const Grouping& expr) const;
                    virtual Token visit_literal(const Literal& expr) const;
                    virtual Token visit_unary(const Unary& expr) const;

                    /* stmt */
                    virtual void visit_expr_stmt(const StmtExpr& expr) const;
                    virtual void visit_print_stmt(const StmtPrint& expr) const;
                    virtual void visit_var_stmt(const StmtVar& expr) const;

                /* Printer */
                    /* expr */
                    virtual string print_binary(const Binary& expr) const;
                    virtual string print_grouping(const Grouping& expr) const;
                    virtual string print_unary(const Unary& expr) const;
                    virtual string print_literal(const Literal& expr) const;

                    /* stmt */
                    // virtual void print_expr_stmt(const StmtExpr& expr) const;
                    // virtual void print_print_stmt(const StmtPrint& expr) const;
                    // virtual void print_var_stmt(const StmtVar& expr) const;

                virtual ~Visitor() = default;
        };

        /// @class Accept
        /// @brief Acceptor in the visitor pattern usefull for accepting Expr/Stmt's
        ///        to be evaluated via the visitor
        template <typename T>
        class Accept
        {
            public:
                // template <typename = std::enable_if_t<std::is_base_of_v<Visitor, V>>>
                virtual T accept(const Visitor& visitor) const = 0; /* evaluator */
                virtual string accept_printer(const Visitor& visitor) const = 0; /* printer */
                virtual ~Accept() = default;
        };
    }
}