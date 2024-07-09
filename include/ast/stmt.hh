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

#include <ast/expr.hh>
#include <ast/grmr.hh>
#include <ast/parser.hh>
#include <utils/macros.hh>

namespace rift
{
    namespace ast
    {

        // i fucking hate c++ macros...
        // template <typename T = void>
        // class StmtVisitor;

        __DEFAULT_FORWARD_VA(
            StmtExpr,
            StmtPrint,
            StmtIf,
            StmtReturn,
            Block,
            For
        );

        /// @class Visitor
        /// @brief Visitor pattern for expressions
        template <typename T>
        class StmtVisitor
        {
            public:
                /// @example [start, end] = [end, start];
                virtual T visit_expr_stmt(const StmtExpr<T>& stmt) const;
                /// @example  print(x);
                virtual T visit_print_stmt(const StmtPrint<T>& stmt) const;
                /// @example  if (cond) {stmt}
                virtual T visit_if_stmt(const StmtIf<T>& stmt) const;
                /// @example  return x;
                virtual T visit_return_stmt(const StmtReturn<T>& stmt) const;
                /// @example  { x = 3; }
                virtual T visit_block_stmt(const Block<T>& block) const;
                /// @example for i in arr {} 
                virtual T visit_for_stmt(const For<T>& decl) const;
        };

        /// @class Stmt
        /// @tparam T 
        template <typename T>
        class Stmt: public rift::ast::Accept<Token>
        {
            public:
                virtual T accept(const Visitor &visitor) const = 0;
                virtual ~Stmt() = default;
        };
        
        /// @class StmtExpr
        /// @brief Represents an expression statement
        /// @details expressions are more common that thought of
        ///          For example: function calls `something();`
        /// @code 
        /// // Code from MDN
        /// // Using control flow statements
        ///    function range(start, end) {
        ///     if (start > end) {
        ///       [start, end] = [end, start];
        ///     }
        /// // Using expression statements
        /// function range2(start, end) {
        ///   start > end && ([start, end] = [end, start]);
        /// }
        template <typename T>
        class StmtExpr: public Stmt
        {
            public:
                StmtExpr(std::unique_ptr<Expr<Token>> expr) : expr(std::move(expr)) {};
                ~StmtExpr() = default;
                std::unique_ptr<Expr<Token>> expr;


                T accept(const Visitor &visitor) const override { return visitor.visit_expr_stmt(*this); };
        };

        template <typename T>
        class StmtPrint : public Stmt
        {
            public:
                StmtPrint(std::unique_ptr<Expr<Token>>& expr) : expr(std::move(expr)) {};
                ~StmtPrint() = default;
                std::unique_ptr<Expr<Token>> expr;

                T accept(const Visitor &visitor) const override { return visitor.visit_print_stmt(*this); };
        };

        template <typename T>
        class StmtIf : public Stmt
        {
            public:
                struct Stmt {
                    public:
                        Stmt() : expr(nullptr), stmt(nullptr), blk(nullptr) {};
                        Stmt(std::unique_ptr<Expr<Token>> expr): expr(std::move(expr)), stmt(nullptr), blk(nullptr) {}
                        Stmt(std::unique_ptr<Expr<Token>> expr, std::unique_ptr<rift::ast::Stmt> stmt): expr(std::move(expr)), stmt(std::move(stmt)) {}
                        Stmt(std::unique_ptr<Expr<Token>> expr, std::unique_ptr<Block> blk): expr(std::move(expr)), blk(std::move(blk)) {}

                        std::unique_ptr<Expr<Token>> expr;
                        std::unique_ptr<rift::ast::Stmt> stmt;
                        std::unique_ptr<Block> blk;
                };

            public:
                StmtIf(): if_stmt(nullptr),  else_stmt(nullptr), elif_stmts({}) {};
                StmtIf(Stmt* if_stmt): if_stmt(if_stmt) {};
                StmtIf(Stmt* if_stmt, Stmt* else_stmt): if_stmt(if_stmt), else_stmt(else_stmt) {};
                StmtIf(Stmt* if_stmt, Stmt* else_stmt, std::vector<Stmt*> elif_stmts): if_stmt(if_stmt), else_stmt(else_stmt), elif_stmts(elif_stmts) {};

                Stmt* if_stmt;
                Stmt* else_stmt;
                std::vector<Stmt*> elif_stmts;

                T accept(const Visitor &visitor) const override { return visitor.visit_if_stmt(*this); };
        };

        template <typename T>
        class StmtReturn : public Stmt
        {
            public:
                StmtReturn(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
                ~StmtReturn() = default;
                std::unique_ptr<Expr> expr;

                T accept(const Visitor &visitor) const override { return visitor.visit_return_stmt(*this); };
        };

        template <typename T>
        class Block : public Stmt
        {
            public:
                Block(vec_prog decls) : decls(std::move(decls)) {};
                ~Block() = default;
                vec_prog decls = nullptr;

                T accept(const Visitor &visitor) const override { return visitor.visit_block_stmt(*this); };
        };

        template <typename T>
        class For : public Stmt
        {
            public:
                For(): expr(nullptr), stmt_l(nullptr), stmt_r(nullptr), decl(nullptr), blk(nullptr), stmt_o(nullptr) {};
                ~For() = default;

                std::unique_ptr<Expr> expr;
                std::unique_ptr<Stmt> stmt_l;
                std::unique_ptr<Stmt> stmt_r;
                std::unique_ptr<Decl> decl;

                std::unique_ptr<Block> blk;
                std::unique_ptr<Stmt> stmt_o;

                T accept(const Visitor &visitor) const override { return visitor.visit_for_stmt(*this); };
        };
    }
}