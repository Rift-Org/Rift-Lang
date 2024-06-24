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

        class StmtVar : public Stmt
        {
            public:
                StmtVar(std::unique_ptr<Expr>& expr) : expr(std::move(expr)) {};
                ~StmtVar() = default;
                std::unique_ptr<Expr> expr;

                Token accept(const Visitor &visitor) const override { return visitor.visit_var_stmt(*this); };
                
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                // must uncomment visit_printer in printer.hh
                string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };

        using vec_prog = std::unique_ptr<std::vector<std::unique_ptr<Stmt>>>;
        using Tokens = std::vector<Token>;

        class Program : public Accept<Tokens>
        {
            public:
                Program(vec_prog statements) : statements(std::move(statements)) {}
                virtual ~Program() = default;
                friend class Visitor;

                Tokens accept(const Visitor &visitor) const override { return visitor.visit_program(*this); }

                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                std::string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop

            protected:
                vec_prog statements = nullptr;
        };
    }
}