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

#include <exception>
#include <ast/grmr.hh>
#include <ast/expr.hh>
#include <ast/stmt.hh>
#include <utils/arithmetic.hh>
#include <utils/literals.hh>

using any = std::any;
using string = std::string;

namespace rift
{
    namespace ast
    {
        class EvalVisitor;

        class Eval
        {
            public:
                Eval();
                ~Eval() = default;
                friend class EvalVisitor;

                /// @brief Evaluates the given *expression*
                std::string evaluate(const rift::ast::Expr& expr);
                /// @brief Evaluates the given *statement*
                void evaluate(const rift::ast::Stmt& expr);

            private:
                std::unique_ptr<Visitor> visitor;
        };


        // class EvalVisitor : public rift::ast::Visitor
        // {
        //     public:
        //         EvalVisitor(Eval &eval);
        //         virtual ~EvalVisitor();
        //         Eval* eval;

        //         /* expr.hh */
        //         Token visit_binary(const Binary& expr) const override;
        //         Token visit_grouping(const Grouping& expr) const override;
        //         Token visit_literal(const Literal& expr) const override;
        //         Token visit_unary(const Unary& expr) const override;

        //         /* stmt.hh */
        //         void visit_expr_stmt(const StmtExpr& expr) const override;
        //         void visit_print_stmt(const StmtPrint& expr) const override;
        //         void visit_var_stmt(const StmtVar& expr) const override;
        // };

        /// @class EvaluatorException
        /// @brief The base exception for the evaluator
        class EvaluatorException: public std::exception
        {
            public:
                EvaluatorException(const std::string &message) : message(message) {}
                ~EvaluatorException() = default;

                const char *what() const noexcept override { return message.c_str(); }

            private:
                std::string message;
        };
    }
}