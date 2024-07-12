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

#include <any>
#include <iostream>
#include <stdlib.h>
#include <memory>
#include <ast/grmr.hh>

using namespace rift::scanner;

namespace rift
{
    namespace ast
    {

        __DEFAULT_FORWARD_VA(
            Assign,
            Binary,
            Grouping,
            Literal,
            Unary
        );

        __DEFAULT_FORWARD_VA(
            Call,
            Ternary,
            VarExpr
        )

        /// @class Visitor
        /// @brief Visitor pattern for expressions
        template <typename T>
        class ExprVisitor
        {
            public:
                /// @example x = 1 + 2;
                virtual T visit_assign(const Assign<T>& expr) const;
                /// @example 1 + 2
                virtual T visit_binary(const Binary<T>& expr) const;
                /// @example (1 + 2)
                virtual T visit_grouping(const Grouping<T>& expr) const;
                /// @example 1
                virtual T visit_literal(const Literal<T>& expr) const;
                /// @example x
                virtual T visit_var_expr(const VarExpr<T>& expr) const;
                /// @example -1
                virtual T visit_unary(const Unary<T>& expr) const;
                /// @example (true) ? 1 : 2
                virtual T visit_ternary(const Ternary<T>& expr) const;
                /// @example x = foo(1, 2)
                virtual T visit_call(const Call<T>& expr) const;
        };

        /// @class Expr
        /// @tparam T: <Token(Eval), string(ASTPrinter)>
        /// @brief Base class for all expressions 
        ///        Acceptor in the visitor pattern usefull for accepting Expr/Stmt/Decl
        ///        to be evaluated via the visitor
        /// @details Types of expression include
        ///          - Binary: An expression with two operands and an operator
        ///            - Example: 1 + 2
        ///          - Grouping: An expression with a single subexpression
        ///            - Example: (1 + 2)
        ///          - Literal: An expression with a single value
        ///            - Example: 1
        ///          - Unary: An expression with a single operator and a single operand
        ///            - Example: -1
        template <typename T>
        class Expr
        {
            public:
                virtual T accept(const ExprVisitor& visitor) const = 0;
                virtual ~Expr() = default;
        };
        

        #pragma mark - Concrete Expressions

        template <typename T>
        class Call : public Expr
        {
            public:
                using Exprs = std::unordered_map<std::string, std::unique_ptr<Expr<T>>>;
                Call(Token name, Exprs&& args): name(name), args(std::move(args)) {};

                Token name; // expr -> Literal::Identifier
                Exprs args;

                inline T accept(const ExprVisitor& visitor) const override { return visitor.visit_call(*this); }
        };

        template <typename T>
        class Ternary : public Expr
        {
            public:
                Ternary(std::unique_ptr<Expr<Token>> condition, std::unique_ptr<Expr<Token>> left, std::unique_ptr<Expr<Token>> right): condition(std::move(condition)), left(std::move(left)), right(std::move(right)) {};
                std::unique_ptr<Expr> condition;
                std::unique_ptr<Expr> left;
                std::unique_ptr<Expr> right;

                inline T accept(const ExprVisitor& visitor) const override { return visitor.visit_ternary(*this); }
        };

        template <typename T>
        class Assign : public Expr
        {
            public:
                Assign(Token name, std::unique_ptr<Expr<Token>> value): name(name), value(std::move(value)) {};
                Token name;
                std::unique_ptr<Expr> value;

                virtual inline T accept(const ExprVisitor& visitor) const override { return visitor.visit_assign(*this); }
        };

        /// @class Binary
        /// @param left The left operand
        /// @param op The operator
        /// @param right The right operand
        // template <typename T = Token, typename V = Token>
        template <typename T>
        class Binary : public Expr
        {
            public:
                Binary(std::unique_ptr<Expr<Token>> left, Token op, std::unique_ptr<Expr<Token>> right): op(op), left(std::move(left)), right(std::move(right)) {};
                Token op;
                std::unique_ptr<Expr> left;
                std::unique_ptr<Expr> right;

                inline T accept(const ExprVisitor& visitor) const override { return visitor.visit_binary(*this); }
        };

        /// @class Grouping
        /// @param expr The subexpression
        template <typename T>
        class Grouping : public Expr
        {
            public:
                Grouping(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
                std::unique_ptr<Expr> expr;

                inline T accept(const ExprVisitor& visitor) const override {return visitor.visit_grouping(*this);}
        };

        /// @class Unary
        /// @param op The operator
        /// @param expr The operand
        template <typename T>
        class Unary : public Expr
        {
            public:
                Unary(Token op, std::unique_ptr<Expr<T>> expr): op(op), expr(std::move(expr)) {};
                Token op;
                std::unique_ptr<Expr> expr;

                inline T accept(const ExprVisitor& visitor) const override {return visitor.visit_unary(*this);}
        };

        /// @class VarExpr
        /// @param value The value of the variable
        template <typename T>
        class VarExpr: public Expr
        {
            public:
                VarExpr(Token value): value(value) {};
                Token value;

                inline T accept(const ExprVisitor &visitor) const override {return visitor.visit_var_expr(*this);}
        };

        /// @class Literal
        /// @param value The value of the literal
        template <typename T>
        class Literal: public Expr
        {
            public:
                Literal(Token value): value(value) {};
                Token value;

                inline T accept(const ExprVisitor &visitor) const override {return visitor.visit_literal(*this);}
        };
    }
};