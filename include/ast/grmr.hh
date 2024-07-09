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

        __DEFAULT_FORWARD_NONE_VA(
            Assign,
            Binary,
            Grouping,
            Literal,
            VarExpr,
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
            ResolverAssign,
            ResolverBinary,
            ResolverGrouping,
            ResolverLiteral
        )

        __DEFAULT_FORWARD_NONE_VA(
            Resolver,
            ResolverUnary,
            ResolverTernary,
            ResolverCall,
            ResolverVarExpr
        )

        __DEFAULT_FORWARD_NONE_VA(
            ResolverStmtExpr,
            ResolverPrint,
            ResolverIf,
            ResolverReturn,
            ResolverDeclVar
        )

        __DEFAULT_FORWARD_NONE_VA(
            ResolverDeclFunc,
            ResolverDeclStmt,
            ResolverBlock,
            ResolverFor
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

                    /* stmt */
                    /// @note TokenType::IGNORE is used to ignore statements returning void

                    /* decl */

                    /* prgm */
                    virtual Tokens visit_program(const Program& prgm) const;

                /* Resolver */
                    /* expr */
                        virtual Token resolve_assign(const ResolverAssign& expr) const;
                        virtual Token resolve_binary(const ResolverBinary& expr) const;
                        virtual Token resolve_grouping(const ResolverGrouping& expr) const;
                        virtual Token resolve_literal(const ResolverLiteral& expr) const;
                        virtual Token resolve_unary(const ResolverUnary& expr) const;
                        virtual Token resolve_ternary(const ResolverTernary& expr) const;
                        virtual Token resolve_call(const ResolverCall& expr) const;
                        virtual Token resolve_var_expr(const ResolverVarExpr& expr) const;

                    /* stmt */
                        virtual Token resolve_expr_stmt(const ResolverStmtExpr& stmt) const;
                        virtual Token resolve_print_stmt(const ResolverPrint& stmt) const;
                        virtual Token resolve_if_stmt(const ResolverIf& stmt) const;
                        virtual Token resolve_return_stmt(const ResolverReturn& stmt) const;
                        virtual Token resolve_block_stmt(const ResolverBlock& block) const;
                        virtual Token resolve_for_stmt(const ResolverFor& decl) const;

                    /* decl */
                        virtual Tokens resolve_decl_stmt(const ResolverDeclStmt& decl) const;
                        virtual Tokens resolve_decl_var(const ResolverDeclVar& decl) const;
                        virtual Tokens resolve_decl_func(const ResolverDeclFunc& decl) const;

                    /* prgm */
                        virtual Tokens resolve_program(const Program& prgm) const;
                

                virtual ~Visitor() = default;
                friend class Resolver;
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