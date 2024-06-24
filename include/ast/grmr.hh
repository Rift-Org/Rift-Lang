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
using Tokens = std::vector<Token>;
using string = std::string;

namespace rift
{
    namespace ast
    {
        ///         RIFT Grammar
        ///         ------------
        /// program        → decl * EOF
        /// decl           → varDecl | statement
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

        __DEFAULT_FORWARD_NONE_VA(
            Stmt, 
            StmtPrint, 
            StmtExpr
        )

        __DEFAULT_FORWARD_NONE_VA(
            Decl, 
            DeclStmt, 
            DeclVar, 
            Program
        )

        using vec_prog = std::unique_ptr<std::vector<std::unique_ptr<Decl>>>;
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
                    /// @note TokenType::IGNORE is used to ignore statements returning void
                    virtual Token visit_expr_stmt(const StmtExpr& expr) const;
                    virtual Token visit_print_stmt(const StmtPrint& expr) const;

                    /* decl */
                    virtual Token visit_decl_stmt(const DeclStmt& decl) const;
                    virtual Token visit_decl_var(const DeclVar& decl) const;
                    
                    /* prgm */
                    virtual Tokens visit_program(const Program& prgm) const;


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

                    /* virtual void print_program(const Program& prgm) const; */

                

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