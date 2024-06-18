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

using any = std::any;

namespace rift
{
    namespace ast
    {
        class EvalVisitor;

        class Eval
        {
            public:
                Eval();
                ~Eval();
                friend class EvalVisitor;

                /// @brief Evaluates the given expression. 
                void evaluate(const rift::ast::Expr::Expr<Token>& expr);

            private:
                EvalVisitor *visitor;
        };


        class EvalVisitor : public rift::ast::Expr::Visitor<Token>
        {
            using BinaryExpr = rift::ast::Expr::Binary<Token>;
            using GroupingExpr = rift::ast::Expr::Grouping<Token>;
            using LiteralExpr = rift::ast::Expr::Literal<Token>;
            using UnaryExpr = rift::ast::Expr::Unary<Token>;
            public:
                EvalVisitor(Eval &eval);
                virtual ~EvalVisitor();
                Eval* eval;

                Token visit_binary(const BinaryExpr& expr) const override;
                Token visit_grouping(const GroupingExpr& expr) const override;
                Token visit_literal(const LiteralExpr& expr) const override;
                Token visit_unary(const UnaryExpr& expr) const override;

                /* helpers */
                bool truthy(Token val) const;
                bool equal(Token left, Token right) const;
                bool isNumber(Token val) const;
                bool isString(Token val) const;
                double castNumber(Token val) const;
                std::string castString(Token val) const;
        };
    }
}