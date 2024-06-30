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
#include <ast/env.hh>

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
        /// blk            → "{" decl* "}"
        /// for            → "for" "(" (decl|stmt)? ";" expr ";" (stmt)? ")" stmt|blk
        /// decl           → varDecl | stmt
        /// varDecl        → "mut" IDENTIFIER ( "=" stmt ) ";"
        /// constDecl      → "mut!" IDENTIFIER ( "=" expr ) ";"
        /// funcDecl       → "func" function ";"
        /// function       → IDENTIFIER "(" params? ")" blk // TODO: allow stmt to emulate lambdas
        /// params         → IDENTIFIER ( "," IDENTIFIER )*
        /// stmt           → exprStmt | printStmt ";"
        /// printStmt      → "print" "(" expression ");"
        /// exprStmt       → expression ";"
        /// ifStmt         → "if" "(" expression ")" stmt|blk ( "elif" statment|blk )* ( "else" stmt|blk )?
        /// returnStmt     → "return" expression ";"
        /// ternary        → expression "?" expression ":" expression ";"
        /// expression     → equality ";"
        /// assignment     → IDENTIFIER "=" assignment | equality
        /// equality       → comparison ( ( "!=" | "==" ) comparison )* ";"
        /// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ";"
        /// term           → factor ( ( "-" | "+" ) factor )* ";"
        /// factor         → unary ( ( "/" | "*" ) unary )* ";"
        /// unary          → ( "!" | "-" ) unary | primary ";"
        /// call           → primary ( "(" arguments? ")" )* ";"
        /// primary        → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ";"

        __DEFAULT_FORWARD_NONE_VA(
            Assign,
            Binary,
            Grouping,
            Literal,
            Unary
        )

        __DEFAULT_FORWARD_NONE_VA(
            Printer,
            Ternary,
            For,
            Call
        )

        __DEFAULT_FORWARD_NONE_VA(
            Stmt, 
            StmtPrint, 
            StmtExpr,
            StmtIf,
            StmtReturn
        )

        __DEFAULT_FORWARD_NONE_VA(
            Decl, 
            DeclStmt, 
            DeclVar,
            DeclFunc
        )

        __DEFAULT_FORWARD_NONE_VA(
            Program,
            Block
        )

        using vec_prog = std::unique_ptr<std::vector<std::unique_ptr<Decl>>>;
        /// @class Visitor 
        /// @brief implementation of the statements and expressions
        class Visitor
        {
            public:
                /* Evaluator */
                    /* expr */
                    virtual Token visit_assign(const Assign& expr) const;
                    virtual Token visit_binary(const Binary& expr) const;
                    virtual Token visit_grouping(const Grouping& expr) const;
                    virtual Token visit_literal(const Literal& expr) const;
                    virtual Token visit_unary(const Unary& expr) const;
                    virtual Token visit_ternary(const Ternary& expr) const;
                    virtual Token visit_call(const Call& expr) const;

                    /* stmt */
                    /// @note TokenType::IGNORE is used to ignore statements returning void
                    virtual Token visit_expr_stmt(const StmtExpr& stmt) const;
                    virtual Token visit_print_stmt(const StmtPrint& stmt) const;
                    virtual Token visit_if_stmt(const StmtIf& stmt) const;
                    [[noreturn]] virtual Token visit_return_stmt(const StmtReturn& stmt) const;

                    /* decl */
                    virtual Tokens visit_decl_stmt(const DeclStmt& decl) const;
                    virtual Tokens visit_decl_var(const DeclVar& decl) const;
                    virtual Tokens visit_decl_func(const DeclFunc& decl) const;
                    /* c-flow */
                    virtual Tokens visit_for(const For& decl) const;
                    /* block */
                    virtual Tokens visit_block_stmt(const Block& block) const;
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