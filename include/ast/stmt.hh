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

        __DEFAULT_FORWARD_NONE_VA(
            StmtExpr,
            StmtPrint,
            StmtVar
        );

    
        class Stmt: public rift::ast::Accept<Token>
        {
            public:
                virtual Token accept(const Visitor &visitor) const = 0;
                virtual string accept_printer(const Visitor& visitor) const = 0;
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
        class StmtExpr: public Stmt
        {
            public:
                StmtExpr(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {};
                ~StmtExpr() = default;
                std::unique_ptr<Expr> expr;


                Token accept(const Visitor &visitor) const override { return visitor.visit_expr_stmt(*this); };
                
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };

        class StmtPrint : public Stmt
        {
            public:
                StmtPrint(std::unique_ptr<Expr>& expr) : expr(std::move(expr)) {};
                ~StmtPrint() = default;
                std::unique_ptr<Expr> expr;

                Token accept(const Visitor &visitor) const override { return visitor.visit_print_stmt(*this); };
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };

        class StmtIf : public Stmt
        {
            public:
                struct Stmt {
                    public:
                        Stmt() : expr(nullptr), stmt(nullptr), blk(nullptr) {};
                        Stmt(std::unique_ptr<Expr> expr): expr(std::move(expr)), stmt(nullptr), blk(nullptr) {}
                        Stmt(std::unique_ptr<Expr> expr, std::unique_ptr<rift::ast::Stmt> stmt): expr(std::move(expr)), stmt(std::move(stmt)) {}
                        Stmt(std::unique_ptr<Expr> expr, std::unique_ptr<Block> blk): expr(std::move(expr)), blk(std::move(blk)) {}

                        std::unique_ptr<Expr> expr;
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

                Token accept(const Visitor &visitor) const override { return visitor.visit_if_stmt(*this); };
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };

        class StmtReturn : public Stmt
        {
            public:
                StmtReturn(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
                ~StmtReturn() = default;
                std::unique_ptr<Expr> expr;

                Token accept(const Visitor &visitor) const override { return visitor.visit_return_stmt(*this); };
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };
    }
}