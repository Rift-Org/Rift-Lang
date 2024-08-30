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

        __DEFAULT_FORWARD_VA(Decl);

        /// @class Visitor
        /// @brief Visitor pattern for expressions
        template <typename T>
        class StmtVisitor
        {
            public:
                StmtVisitor() = default;
                virtual ~StmtVisitor() = default;
                /// @example [start, end] = [end, start];
                virtual T visit_expr_stmt(const StmtExpr<T>& stmt) const = 0;
                /// @example  print(x);
                virtual T visit_print_stmt(const StmtPrint<T>& stmt) const = 0;
                /// @example  if (cond) {stmt}
                virtual T visit_if_stmt(const StmtIf<T>& stmt) const = 0;
                /// @example  return x;
                virtual T visit_return_stmt(const StmtReturn<T>& stmt) const = 0;
                /// @example  { x = 3; }
                virtual T visit_block_stmt(const Block<T>& block) const = 0;
                /// @example for i in arr {} 
                virtual T visit_for_stmt(const For<T>& decl) const = 0;
        };

        /// @class Stmt
        /// @tparam T <Token,Tokens,void>
        template <typename T>
        class Stmt
        {
            public:
                virtual T accept(const StmtVisitor<T> &visitor) const = 0;
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
        class StmtExpr: public Stmt<T>
        {
            public:
                StmtExpr(std::unique_ptr<Expr<Token>> expr) : expr(std::move(expr)) {};
                ~StmtExpr() = default;
                std::unique_ptr<Expr<Token>> expr;


                T accept(const StmtVisitor<T> &visitor) const override { return visitor.visit_expr_stmt(*this); };
        };

        template <typename T>
        class StmtPrint : public Stmt<T>
        {
            public:
                StmtPrint(std::unique_ptr<Expr<Token>>& expr) : expr(std::move(expr)) {};
                ~StmtPrint() = default;
                std::unique_ptr<Expr<Token>> expr;

                T accept(const StmtVisitor<T> &visitor) const override { return visitor.visit_print_stmt(*this); };
        };

        template <typename T>
        class StmtIf : public Stmt<T>
        {
            public:
                struct Stmt {
                    public:
                        Stmt() : expr(nullptr), stmt(nullptr), blk(nullptr) {};
                        Stmt(std::unique_ptr<Expr<Token>> expr): expr(std::move(expr)), stmt(nullptr), blk(nullptr) {}
                        Stmt(std::unique_ptr<Expr<Token>> expr, std::unique_ptr<rift::ast::Stmt<T>> stmt): expr(std::move(expr)), stmt(std::move(stmt)) {}
                        Stmt(std::unique_ptr<Expr<Token>> expr, std::unique_ptr<Block<T>> blk): expr(std::move(expr)), blk(std::move(blk)) {}

                        std::unique_ptr<Expr<Token>> expr;
                        std::unique_ptr<rift::ast::Stmt<T>> stmt;
                        std::unique_ptr<Block<T>> blk;
                };

            public:
                StmtIf(): if_stmt(nullptr),  else_stmt(nullptr), elif_stmts({}) {};
                StmtIf(Stmt* if_stmt): if_stmt(if_stmt) {};
                StmtIf(Stmt* if_stmt, Stmt* else_stmt): if_stmt(if_stmt), else_stmt(else_stmt) {};
                StmtIf(Stmt* if_stmt, Stmt* else_stmt, std::vector<Stmt*> elif_stmts): if_stmt(if_stmt), else_stmt(else_stmt), elif_stmts(elif_stmts) {};

                Stmt* if_stmt;
                Stmt* else_stmt;
                std::vector<Stmt*> elif_stmts;

                T accept(const StmtVisitor<T> &visitor) const override { return visitor.visit_if_stmt(*this); };
        };

        template <typename T>
        class StmtReturn : public Stmt<T>
        {
            public:
                StmtReturn(std::unique_ptr<Expr<Token>> expr): expr(std::move(expr)) {};
                ~StmtReturn() = default;
                std::unique_ptr<Expr<Token>> expr;

                T accept(const StmtVisitor<T> &visitor) const override { return visitor.visit_return_stmt(*this); };
        };

        template <typename T>
        class Block : public Stmt<T>
        {
            public:
                using vec_prog = std::vector<std::unique_ptr<Decl<Token>>>;
                vec_prog decls = {};

                Block() = default;
                ~Block() = default;
                Block(vec_prog&& decls) : decls(std::move(decls)) {};
                Block(const Block<T>& other) { decls = other.decls; }

                T accept(const StmtVisitor<T> &visitor) const override { return visitor.visit_block_stmt(*this); };

                Block &operator=(const Block<T> &other)
                {
                    if (this != &other)
                    {
                        decls.clear();
                        decls.reserve(other.decls.size());
                        for (const auto &decl : other.decls) {
                            decls.push_back(decl);
                        }
                    }
                    return *this;
                }
        };

        template <typename T>
        class For : public Stmt<T>
        {
            public:
                For(): expr(nullptr), stmt_l(nullptr), stmt_r(nullptr), decl(nullptr), blk(nullptr), stmt_o(nullptr) {};
                ~For() = default;
                                                    // for
                                                    // 1)
                std::unique_ptr<Decl<Token>> decl;  //    mut i = 0; 
                std::unique_ptr<Stmt<void>> stmt_l; //    i = 0;
                                                    // 2)
                std::unique_ptr<Expr<Token>> expr;  //    i < 10;
                                                    // 3)
                std::unique_ptr<Stmt<void>> stmt_r; //    i+=1;
                                                    // 4)
                std::unique_ptr<Block<void>> blk;   //    {}
                std::unique_ptr<Stmt<void>> stmt_o; //    todo: used for lambdas <future impl>

                T accept(const StmtVisitor<T> &visitor) const override { return visitor.visit_for_stmt(*this); };
        };
    }
}