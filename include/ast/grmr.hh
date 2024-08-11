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
#include <vector>

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
        /// returnStmt     → "return" (expression)? ";"
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

        // __DEFAULT_FORWARD_VA(
        //     Assign,
        //     Binary,
        //     Grouping,
        //     Literal,
        //     VarExpr,
        //     Unary
        // )

        // __DEFAULT_FORWARD_VA(
        //     Printer,
        //     Ternary,
        //     For,
        //     Call
        // )

        // __DEFAULT_FORWARD_VA(
        //     Stmt, 
        //     StmtPrint, 
        //     StmtExpr,
        //     StmtIf,
        //     StmtReturn
        // )

        // __DEFAULT_FORWARD_VA(
        //     Decl, 
        //     DeclStmt, 
        //     DeclVar,
        //     DeclFunc
        // )

        // __DEFAULT_FORWARD_VA(
        //     Program,
        //     Block
        // )

        /// @class Visitor 
        /// @brief implementation of the statements and expressions
        // class Visitor
        // {
        //     public:
        //         virtual ~Visitor() = default;
        // };

        /// @class Accept
        /// @brief Acceptor in the visitor pattern usefull for accepting Expr/Stmt's
        ///        to be evaluated via the visitor
        // template <typename T>
        // class Accept
        // {
        //     public:
        //         // template <typename = std::enable_if_t<std::is_base_of_v<Visitor, V>>>
        //         virtual T accept(const Visitor& visitor) const = 0; /* evaluator */
        //         virtual string accept_printer(const Visitor& visitor) const = 0; /* printer */
        //         virtual ~Accept() = default;
        // };
    }
}