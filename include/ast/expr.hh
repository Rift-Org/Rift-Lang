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
        /// @note forward declrations
        __DEFAULT_FORWARD_NONE_VA(
            Binary,
            Grouping,
            Literal,
            Unary
        );

        /// @class Expr
        /// @brief Base class for all expressions 
        /// @details Types of expression include
        ///          - Binary: An expression with two operands and an operator
        ///            - Example: 1 + 2
        ///          - Grouping: An expression with a single subexpression
        ///            - Example: (1 + 2)
        ///          - Literal: An expression with a single value
        ///            - Example: 1
        ///          - Unary: An expression with a single operator and a single operand
        ///            - Example: -1
        class Expr : public Accept<Token>
        {
            public:
                virtual Token accept(const Visitor& visitor) const override = 0;
                virtual string accept_printer(const Visitor& visitor) const override = 0;
                virtual ~Expr() = default;
        };

        #pragma mark - Concrete Expressions

        using Exprs = std::unordered_map<std::string, std::unique_ptr<Expr>>;
        class Call : public Expr
        {
            public:
                Call(Token name, Exprs&& args): name(name), args(std::move(args)) {};

                Token name; // expr -> Literal::Identifier
                Exprs args;

                inline Token accept(const Visitor& visitor) const override { return visitor.visit_call(*this); }
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                inline string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };

        class Ternary : public Expr
        {
            public:
                Ternary(std::unique_ptr<Expr> condition, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right): condition(std::move(condition)), left(std::move(left)), right(std::move(right)) {};
                std::unique_ptr<Expr> condition;
                std::unique_ptr<Expr> left;
                std::unique_ptr<Expr> right;

                inline Token accept(const Visitor& visitor) const override { return visitor.visit_ternary(*this); }
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                inline string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };

        class Assign : public Expr
        {
            public:
                Assign(Token name, std::unique_ptr<Expr> value): name(name), value(std::move(value)) {};
                Token name;
                std::unique_ptr<Expr> value;

                inline Token accept(const Visitor& visitor) const override { return visitor.visit_assign(*this); }
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wunused-parameter"
                inline string accept_printer(const Visitor& visitor) const override { return "unimplemented"; }
                #pragma clang diagnostic pop
        };

        /// @class Binary
        /// @param left The left operand
        /// @param op The operator
        /// @param right The right operand
        // template <typename T = Token, typename V = Token>
        class Binary : public Expr
        {
            public:
                Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right): op(op), left(std::move(left)), right(std::move(right)) {};
                Token op;
                std::unique_ptr<Expr> left;
                std::unique_ptr<Expr> right;

                inline Token accept(const Visitor& visitor) const override { return visitor.visit_binary(*this); }
                inline string accept_printer(const Visitor& visitor) const override { return visitor.print_binary(*this); }
        };

        /// @class Grouping
        /// @param expr The subexpression
        class Grouping : public Expr
        {
            public:
                Grouping(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
                std::unique_ptr<Expr> expr;

                inline Token accept(const Visitor& visitor) const override {return visitor.visit_grouping(*this);}
                inline string accept_printer(const Visitor& visitor) const override {return visitor.print_grouping(*this);}
        };

        /// @class Literal
        /// @param value The value of the literal
        class Literal: public Expr
        {
            public:
                Literal(Token value): value(value) {};
                Token value;

                inline Token accept(const Visitor &visitor) const override {return visitor.visit_literal(*this);}
                inline string accept_printer(const Visitor& visitor) const override {return visitor.print_literal(*this);}
        };

        /// @class Unary
        /// @param op The operator
        /// @param expr The operand
        class Unary : public Expr
        {
            public:
                Unary(Token op, std::unique_ptr<Expr> expr): op(op), expr(std::move(expr)) {};
                Token op;
                std::unique_ptr<Expr> expr;

                inline Token accept(const Visitor& visitor) const override {return visitor.visit_unary(*this);}
                inline string accept_printer(const Visitor& visitor) const override {return visitor.print_unary(*this);}
        };
    }
};